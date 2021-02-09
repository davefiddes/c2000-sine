# c2000-sine

This is intended as a port of the Huebner inverter project
(https://github.com/jsphuebner/stm32-sine) for the Texas Instruments C2000
family of microcontrollers. Notably the TMS320F82377D is found in the Tesla
Model 3 inverter.

The core inverter code is from https://github.com/jsphuebner/stm32-sine
commit 8dcec8613847427cead0093dae97aabf8458d528.

## Compiling

Building the code requires TI Code Composer Studio 10.2.0.00009 from https://www.ti.com/tool/download/CCSTUDIO

There are currently no additional dependencies.

## Target Platform

The project currently targets the TI LAUNCHXL-F28379D development board.
