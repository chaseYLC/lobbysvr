#pragma once

#ifdef _WIN32
	#ifndef _WINDOWS_
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#endif

	#include <tchar.h>
	#include <stdio.h>
	#include <conio.h>
	#include <stdlib.h>
	#include <new.h>
	#include <signal.h>
	#include <exception>
	#include <sys/stat.h>
	#include <psapi.h>
	#include <rtcapi.h>
	#include <Shellapi.h>
	#include <dbghelp.h>

namespace MLN
{
	class CCrashHandler
	{
	public:
		static void Init(const char* root_path, const char* header, const bool isFullDump);
		static void Init(const char* env_file);

	private:
		static bool InitFromXML(const char* env_file);

		// Sets exception handlers that work on per-process basis
		static void SetProcessExceptionHandlers();

		// Installs C++ exception handlers that function on per-thread basis
		static void SetThreadExceptionHandlers();

		static void GetExceptionPointers(DWORD dwExceptionCode, EXCEPTION_POINTERS** pExceptionPointers);
		static void CreateMiniDump(EXCEPTION_POINTERS* pExcPtrs);
		static void CreateFullDump(EXCEPTION_POINTERS* pExcPtrs);
		static void MyStackWalk(EXCEPTION_POINTERS* pExcPtrs);

		/* Exception handler functions. */

		static LONG WINAPI SehHandler(PEXCEPTION_POINTERS pExceptionPtrs);
		static void __cdecl TerminateHandler();
		static void __cdecl UnexpectedHandler();
		static void __cdecl PureCallHandler();
		static void __cdecl InvalidParameterHandler(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved);
		static int __cdecl NewHandler(size_t);

		static void SigabrtHandler(int);
		static void SigfpeHandler(int /*code*/, int subcode);
		static void SigintHandler(int);
		static void SigillHandler(int);
		static void SigsegvHandler(int);
		static void SigtermHandler(int);
	};

};//namespace MLN


#else

// ref from : https://github.com/Tragh/MemDump/blob/master/main.cc
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <regex>
#include <iterator>
#include <cassert>
#include <inttypes.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

struct MapsLine{
	bool valid;
	uint64_t start;
	uint64_t end;
	std::string permissions;
	uint64_t offset;
	std::string device;
	uint64_t inode;
	std::string pathname;
	
	MapsLine(const MapsLine&)=default;
	MapsLine(std::string line){
		//regex to split each line of the maps file
		std::regex re("^([^\\s-]+)-([^\\s]+)\\s([^\\s]+)\\s([^\\s]+)\\s([^\\s]+)\\s([^\\s]+)\\s*(.*)$");
		std::smatch sm;
		
		//match and store matches in sm
		std::regex_match(line,sm,re);
		
		//it should split into 8 parts
		//the first is just the whole string
		if(sm.size()!=8){
			valid=false;
			return;
		}
		
		//assign each part to a member variable
		start=std::stoull(sm[1],0,16);
		end=std::stoull(sm[2],0,16);
		permissions=sm[3];
		offset=std::stoull(sm[4],0,16);
		device=sm[5];
		inode=std::stoull(sm[6]);
		pathname=sm[7];
		valid=true;
	}
	
	std::string as_string(){
		std::stringstream ss;
		ss << std::hex << start << "-" << end << " " 
			<< permissions << " "
			<< offset << " "
			<< device << " "
			<< std::dec << inode << " "
			<< pathname;
		return ss.str();
	}
};

/*
int main(){
	
	int PID;
	std::cout << "Enter PID:" << std::endl;
	std::cin >> PID;

	std::vector<MapsLine> maps;


	struct PtraceStruct {
		int PID;
		int status;
		PtraceStruct(int PID_):PID(PID_){
			ptrace(PTRACE_ATTACH, PID, nullptr, 0);
			waitpid(PID, &status, WSTOPPED);
		}
		~PtraceStruct(){
			ptrace (PTRACE_DETACH, PID, nullptr, 0);
		}
	} ptraceStruct(PID);


	{
		//open the memory maping file for the PID
		std::ifstream infile("/proc/"+std::to_string(PID)+"/maps");
		if(!infile){
			std::cout << "Couldn't open /proc/PID/maps. Wrong PID? Aborting." << std::endl;
			return 0;
		}
		std::string line;
		while (std::getline(infile, line)){
			//parse each line of the file
			MapsLine m(line);
			if(!m.valid){
				std::cout << "Error parsing line in /proc/PID/maps file" << std::endl;
				return 0;
			}
			if(m.inode==0)
				maps.push_back(m); //and store in vector
		}
	}
	
	std::cout << "Select a region to dump:" << std::endl << std::endl;
	for(int i=0;i<maps.size();++i){
		std::cout << "[" << i << "] " <<  maps[i].as_string() << std::endl;
	}

	std::cout << std::endl << std::endl << "Select a region to dump:" << std::endl;
	
	int region;
	std::cin >> region;
	assert((region>=0) && (region < maps.size()));

	std::cout << "This will create a file: dump.bin in the current directory. \n"
		<< "its size will be " << (maps[region].end-maps[region].start) << " bytes. \n"
		<< "Continue? [y/N]" << std::endl;
	
	std::string yn;
	std::cin >> yn;
	if(yn.size()<0 || !(yn[0]=='Y' || yn[0]=='y')){
		std::cout << "Aborting." << std::endl;
		return 0;
	}

	//open output file
	std::ofstream outfile ("dump.bin",std::ios::binary);
	if(!outfile){
		std::cout << "Couldn't open output file. Aborting." << std::endl;
		return 0;
	}
	
	//open process memory
	std::ifstream infile ("/proc/"+std::to_string(PID)+"/mem",std::ios::binary);
	if(!infile){
		std::cout << "Couldn't open process memory. Aborting." << std::endl;
		return 0;
	}
	
	//seek to the correct position
	infile.seekg(maps[region].start);
	
	//create istreambuf iterator (starts at the place seeked to)
	std::istreambuf_iterator<char> infile_it(infile);
	std::ostreambuf_iterator<char> outfile_it(outfile);
	
	//copy the correct number of bytes to the outfile
	std::copy_n(infile_it, maps[region].end-maps[region].start, outfile_it );
	
	std::cout << "Done!" << std::endl;
	       
	return 0;
}
*/

	

#endif // #ifdef _WIN32