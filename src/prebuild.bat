@echo off

rem Generate reflection data
rem The generated file should be output in the directory *.generated* in the source directory
rem The header file is given entension *.typeinfo*

tools\reflection\reflection.exe include: 'src/' build: 'src/tokenizer.h' output: 'src/.generated/tokenizer.typeinfo'

rem Shader Code Generation
rem To add your shader to prebuild process, goto file tools/shader/main.cpp, and in the system_main function add your shader file
tools\shader\shader.exe
