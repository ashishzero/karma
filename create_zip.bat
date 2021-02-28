@echo off

@echo Setting up directories...
	if not exist "bin" mkdir "bin" 1>log.txt
	if exist "bin\zip" rd /q /s "bin\zip" 1>log.txt
	mkdir "bin\zip" 1>log.txt
	mkdir "bin\zip\charitra" 1>log.txt
	mkdir "bin\zip\charitra\data" 1>log.txt
	mkdir "bin\zip\charitra\resources" 1>log.txt
	mkdir "bin\zip\charitra\dev" 1>log.txt

@echo Setting up server ip...
	if %1.==. (
	    @echo Server ip not provided, using local ip address
	) else (
	    @echo Server ip provided: %1...saved.
	    @echo %1>bin\zip\charitra\dev\server.txt
	)

@echo Copying resource and data files...
	xcopy run_tree\data bin\zip\charitra\data /E 1>log.txt
	xcopy run_tree\resources bin\zip\charitra\resources /E 1>log.txt
	xcopy run_tree\dev\license bin\zip\charitra\dev /E 1>log.txt
	xcopy run_tree\dev\GeneraleStation-Regular.otf bin\zip\charitra\dev 1>log.txt

@echo Building server...
	msbuild karma_server.vcxproj -p:Configuration=Developer 1>log.txt

@echo Copying server exe...
	xcopy run_tree\karma_server_x64_Developer.exe bin\zip\charitra 1>log.txt
	ren bin\zip\charitra\karma_server_x64_Developer.exe karma_server_x64.exe 1>log.txt

@echo Building client exe...
	msbuild karma_client.vcxproj -p:Configuration=Developer 1>log.txt

@echo Copying client exe...
	xcopy run_tree\karma_client_x64_Developer.exe bin\zip\charitra 1>log.txt
	ren bin\zip\charitra\karma_client_x64_Developer.exe karma_client_x64.exe 1>log.txt

@echo Building developer mode exe...
	msbuild karma.vcxproj -p:Configuration=Developer 1>log.txt

@echo Copying developer mode exe...
	xcopy run_tree\karma_x64_Developer.exe bin\zip\charitra 1>log.txt
	ren bin\zip\charitra\karma_x64_Developer.exe karma_developer_x64.exe 1>log.txt

@echo Creating zip...
	pushd bin\zip
	tar.exe -cf charitra.zip charitra 1>log.txt
	popd

@echo Cleaning up...
	if exist "bin\zip\charitra" rd /q /s "bin\zip\charitra" 1>log.txt

@echo Output File=bin\zip\charitra.zip
