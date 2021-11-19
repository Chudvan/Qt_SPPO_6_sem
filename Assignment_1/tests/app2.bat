@echo off
set /a counter=0
:do
 rem any code
 echo %counter%
 set /a counter+=1
:while
 if %counter% geq 4 (goto next) else (goto do)
:next
rem any code
echo next
pause&exit /b