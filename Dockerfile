  
FROM chase81/lobbysvr:base20191116

RUN apt-get install -y pkg-config

RUN mkdir -p /usr/src/app/svc_lobbysvr

COPY CMakeLists.txt /usr/src/app/svc_lobbysvr
COPY mln_net_constants.dat /usr/src/app/svc_lobbysvr
ADD include /usr/src/app/svc_lobbysvr/include
ADD lobbysvr /usr/src/app/svc_lobbysvr/lobbysvr
ADD mln /usr/local/mln
ADD net /usr/src/app/svc_lobbysvr/net

ENV BOOST_LIB_PATH=/usr/local \
    AWS_REGION=ap-northeast-2 \
    AWS_DEFAULT_REGION=ap-northeast-2

ENV PATH /usr/local:$PATH
ENV LD_LIBRARY_PATH /usr/local/lib:$LD_LIBRARY_PATH

WORKDIR /usr/src/app/svc_lobbysvr

RUN ln -s /usr/include/jsoncpp/json/ /usr/include/json

# RUN apt-get -y install clang llvm

RUN cmake CMakeLists.txt
RUN make

CMD ["./bin/lobbysvr"]