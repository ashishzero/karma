@echo off

rem This file is used to generate reflection data
rem The generated file should be output in the directory *.generated* in the source directory
rem The header file is given entension *.typeinfo*

..\tools\reflection\reflection.exe include: '.' build: 'tokenizer.h' output: '.generated/tokenizer.typeinfo'
