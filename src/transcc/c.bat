@echo off

if exist "..\..\bin\transcc_winnt_original.exe" (
	echo transcc_winnt_original.exe already exists
) else (
	rename ..\..\bin\transcc_winnt.exe transcc_winnt_original.exe
)

..\..\bin\transcc_winnt_original.exe -target=C++_Tool -config=release transcc.monkey

copy /Y transcc.build\cpptool\main_winnt.exe ..\..\bin\transcc_winnt.exe

echo DONE!
pause
