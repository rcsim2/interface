VxD Utilities (C) Blacksphere
=============================

These VxD utilities are provided as additional means to test your transmitter, the interface, and the parallel port interface driver.


Txint.exe
---------

This is a DOS utility and as such does not load the kernel mode driver (Vmyxd.vxd). It runs in Real Mode and installs the ISR (Interrupt Service Routine) by itself. It can not be used to test the driver but it can be used to test transmitter-to-interface communication.


Load.exe
--------

This is a Win32 console application and like all Win32 apps it needs a kernel mode driver to install an ISR. (Alternatively, it is possible to do this by means of a 32-bit DLL-to-16-bit DLL thunking procedure, but that would be faking it...). It can be used to test transmitter, interface, and driver.