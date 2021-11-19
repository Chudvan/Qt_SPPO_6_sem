@echo off
set /a counter=0
:do
 rem any code
 ping –n 1 localhost > nul 
 echo %counter%
 set /a counter+=1
 sleep
:while
 if 1==0 (goto next) else (goto do)
:next
rem any code
echo next
pause&exit /b