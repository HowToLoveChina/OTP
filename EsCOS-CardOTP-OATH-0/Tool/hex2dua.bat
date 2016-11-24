cd tool
copy ..\obj\nz3180.H00 tmp.hex
hex2bin tmp.hex tmp.bin
bin2hex tmp.bin tmp2.hex
hex2dual tmp2.hex
copy ROM.DUA ..\obj\flash.DUA
del ROM.DUA
del tmp.bin
del tmp.hex
del tmp2.hex



