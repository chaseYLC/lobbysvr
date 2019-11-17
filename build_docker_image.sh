#!/bin/bash
cp -rf /usr/local/mln .
docker build --tag=chase81/lobbysvr:master .
