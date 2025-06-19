
cd .
set PATH=C:\cygwin64\bin;C:\Program Files (x86)\Common Files\Oracle\Java\javapath;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\WINDOWS\System32\OpenSSH\;C:\Program Files (x86)\Windows Kits\10\Windows Performance Toolkit\;C:\MinGW\bin;C:\Program Files\PuTTY\;C:\Program Files\Git\cmd;C:\Program Files\Bandizip\;C:\Program Files\dotnet\;C:\Program Files\Java\jdk-19\bin;C:\Program Files\Common Files\Oracle\Java\javapath;C:\Program Files\MATLAB\R2024b\runtime\win64;C:\Program Files\MATLAB\R2024b\bin;c:\Program Files\Vector SIL Kit 4.0.55\x86\bin;c:\Program Files\Vector SIL Kit 4.0.55\x86_64\bin;c:\Program Files (x86)\Vector CANdb++ 3.1\Exec32;C:\Users\USER\AppData\Local\Programs\Python\Python311\Scripts\;C:\Users\USER\AppData\Local\Programs\Python\Python311\;C:\HighTec\toolchains\tricore\v4.9.3.0-infineon-1.0\bin;C:\Users\USER\AppData\Local\Microsoft\WindowsApps;C:\Users\USER\AppData\Local\Programs\Microsoft VS Code\bin;C:\Users\USER\Infineon\LauncherService;c:\Program Files\Vector Informatik GmbH\VIO System Configuration Tool\

"C:\Program Files\MATLAB\R2024b\bin\win64\gmake" -f MCU.mk  MAT_FILE=1 GENERATE_MODEL_DESCRIPTION=1 MIP_CONFIG_XCP_TIMESTAMPS_DISABLE="off" DEBUG_BUILD=0 HAS_WES64_BUILD="off" HAS_WES_BUILD="off" HAS_ERT_BUILD="off" TARGET_ARCHITECTURE="Standard 64 Bit Real-Time DLL" GENERATE_VMODULE=1 COMBINE_OUTPUT_UPDATE_FCNS=0 INCLUDE_MDL_TERMINATE_FCN=1 MULTI_INSTANCE_CODE=0 OPTS="-DTID01EQ=0" TARGET_ARCH=\"x64\" XCP_MAX_MSG_LENGTH=-1 ANALYSIS_MODEL=0 HAS_CAPL_CALL=0 DO_COMPILER_OPTIMIZATION=0 BUILD_ERT=0 RTT_PE=0
@if errorlevel 1 goto error_exit
exit /B 0

:error_exit
echo The make command returned an error of %errorlevel%
An_error_occurred_during_the_call_to_make
