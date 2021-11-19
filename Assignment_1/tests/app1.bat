if "%2%"=="" (
	ping -n 4 localhost > nul  
	echo %1
) else (
	echo %1 %2
)