project->Properties->C/C++ Build->Settings->MCU G++ Linker->Miscellaneous
Linker flags:
	-specs=nosys.specs -specs=nano.specs
	
	-specs=nosys.specs - игнорирует отсутствие функций требуемых для Semihosting, которые вызываются из printf, scanf и т.д.
	-specs=nano.specs - линкует специально разработанную либу NewLib-nano(замена libc) для ARM микроконтроллеров значительно сокращая размер
	скомпилированной прошивки. 
	
http://pabigot.github.io/bspacm/newlib.html

Newlib is an open-source implementation of the standard C library, and is used as the libc implementation in BSPACM's primary toolchain, 
GNU Tools for ARM Embedded Processors. It was designed for use in embedded systems, using a small set of externally-supplied
functions to interface with the system.