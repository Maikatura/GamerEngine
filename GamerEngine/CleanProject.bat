@echo off

echo Do you want to clean project?
echo 'y' only temp files
echo 'n' dont clean anything
echo 'a' everything (editor settings, assets map and temp files)
choice /c yna /n /m "Yes, No or All [y/n/a]?"


If ErrorLevel 3 goto CleanAll
If ErrorLevel 2 goto EndOfbatch
If ErrorLevel 1 goto Clean

:CleanAll
rmdir /s /q "%cd%\Bin\Shaders\"
rmdir /s /q "%cd%\Bin\Assets\"
mkdir "%cd%\Bin\Assets"

:Clean
echo "Cleaing Everything!"
rmdir /s /q "%cd%\.vs\"
rmdir /s /q "%cd%\Temp\"
del /f "%cd%\Bin\*.pdb"
del /f "%cd%\Bin\*.exe"

:EndOfbatch

echo "Bye!"
pause
exit

