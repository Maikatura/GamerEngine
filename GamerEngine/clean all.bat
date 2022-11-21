@echo off

echo "Do you really want to remove all .vs, Intermediate and x64 folders and all Builds that are not release?"
choice /c yn /n /m "Yes, No [y/n]?"

If ErrorLevel 2 goto EndOfbatch
If ErrorLevel 1 goto Clean


:Clean
echo "Cleaing Everything!"
rmdir /s /q "%cd%\.vs\"
rmdir /s /q "%cd%\Intermediate\"
rmdir /s /q "%cd%\Lib\"
del /f "%cd%\Bin\GamerEngine-Debug.*"
del /f "%cd%\Bin\GamerEngine-Release.pdb"
del /f "%cd%\Bin\Modelviewer.*"

:EndOfbatch
echo "Bye!"
pause
exit

