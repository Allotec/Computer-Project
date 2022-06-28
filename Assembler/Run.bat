@ECHO off
g++ -o assembler .\main.cpp .\Assembler.cpp
.\assembler.exe
py .\fileParse.py