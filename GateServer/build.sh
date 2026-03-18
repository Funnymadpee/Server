#!/bin/bash
g++ -g main.cpp \
Private/*.cpp \
-o GateServer.exe \
-IC:/msys64/mingw64/include \
-LC:/msys64/mingw64/lib \
-static \
-static-libgcc \
-static-libstdc++ \
-lws2_32 \
-lmswsock \
-lwsock32 \
-lboost_thread-mt \
-lboost_chrono-mt \
-lboost_date_time-mt \
-lboost_context-mt