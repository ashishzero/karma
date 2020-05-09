# Karma Reflected Code Directory

This directory contains all the reflected code from Karma Reflection
Files with .typeinfo extension are reflected code, and should be ignored from version control

# How to add code for reflection and use the reflection
Add to `src/prebuild.bat` the following line:
```
..\tools\reflection\reflection.exe include: 'src/.' build: '{file_to_reflect}.h' output: 'src/.generated/{file_to_reflect}.typeinfo'
```
where {file_to_reflect} is the file you want to reflect.

You must include the reflected file after the header file before the usage of the reflection. For example, 
for the following command:
	"..\tools\reflection\reflection.exe include: 'src/.' build: 'tokenizer.h' output: 'src/.generated/tokenizer.typeinfo'"
The source file using this reflection should include reflected code in following order
```
#include "tokenizer.h"
#include ".generated/tokenizer.typeinfo"

// you can use the reflection here
// the docs for reflection is available in src/reflection.h file
```
