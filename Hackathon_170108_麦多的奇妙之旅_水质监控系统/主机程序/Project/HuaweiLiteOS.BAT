SET PATH=F:\MDK512\ARM\ARMCC\Bin;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\Program Files\Microsoft SQL Server\120\Tools\Binn\;C:\Program Files\Microsoft SQL Server\130\Tools\Binn\;C:\WINDOWS\system32\config\systemprofile\.dnx\bin;C:\Program Files\Microsoft DNX\Dnvm\;C:\Program Files (x86)\Windows Kits\8.1\Windows Performance Toolkit\;%USERPROFILE%\AppData\Local\Microsoft\WindowsApps;;F:\ModelSim\win64;F:\Modelsim\win64;f:\modelsim\modeltch_6.2b\win32
SET CPU_TYPE=STM32F103R8
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=HuaweiLiteOS
SET CPU_CLOCK=0x007A1200
"F:\MDK512\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\main.__i"
"F:\MDK512\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_it.__i"
"F:\MDK512\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\delay.__i"
"F:\MDK512\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\bsp_led.__i"
"F:\MDK512\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\bsp_usart.__i"
"F:\MDK512\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\bsp_key.__i"
