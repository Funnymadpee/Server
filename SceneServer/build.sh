#!/bin/bash
g++ -g main.cpp \
Private/*.cpp \
Private/Scene/*.cpp \
Private/Battle/*.cpp \
Private/Network/*.cpp \
Private/Object/*.cpp \
-o SceneServer.exe \
-ID:/TrinityCore/Note/Projects/Server/SceneServer/Common \
-IC:/msys64/mingw64/include \
-LC:/msys64/mingw64/lib \
-static \
-static-libgcc \
-static-libstdc++ \
-lmswsock \
-lwsock32 \
-lboost_thread-mt \
-lboost_chrono-mt \
-lboost_date_time-mt \
-lboost_context-mt \
-I"D:/TrinityCore/Note/Projects/Server/nats.c/src" \
-L"D:/TrinityCore/Note/Projects/Server/nats.c/build/src" \
-lnats_static \
-lssl -lcrypto \
-lws2_32 \
-lcrypt32 \
-std=c++17