@echo off

echo "Do you really want to remove all .vs, Intermediate and x64 folders?"
choice /c yn /n /m "Yes, No [y/n]?"

If ErrorLevel 2 goto EndOfbatch
If ErrorLevel 1 goto Clean


:Clean
echo "Cleaing Everything!"
rmdir /s /q "%cd%\.vs\"
rmdir /s /q "%cd%\Intermediate\"
del /f "%cd%\Lib\Release\AudioEngine.*"
del /f "%cd%\Lib\Release\External-r.*"
del /f "%cd%\Lib\Release\Game-r.*"
del /f "%cd%\Lib\Release\GraphicsEngine-r.*"
del /f "%cd%\Lib\Debug\AudioEngine.*"
del /f "%cd%\Lib\Debug\External-d.*"
del /f "%cd%\Lib\Debug\Game-d.*"
del /f "%cd%\Lib\Debug\GraphicsEngine-d.*"
del /f "%cd%\Bin\GameEngine-Debug.pdb"
del /f "%cd%\Bin\GamerEngine-Release.pdb"
del /f "%cd%\Bin\GameEngine-Debug.exe"
del /f "%cd%\Bin\Modelviewer.exe"
del /f "%cd%\Bin\Modelviewer.pdb"

:EndOfbatch
echo "Bye!"
pause
exit

