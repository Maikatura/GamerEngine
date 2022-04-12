@echo off

echo "Do you really want to remove all .vs, Intermediate and x64 folders?"
choice /c yn /n /m "Yes, No [y/n]?"

If ErrorLevel 2 goto EndOfbatch
If ErrorLevel 1 goto Clean


:Clean
echo "Cleaing Everything!"
PowerShell Remove-Item %cd%\* -include .vs, Intermediate, x64 -force -recurse 

:EndOfbatch
echo "Bye!"
exit