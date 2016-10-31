ROVER
====================================
Robot is a program that runs on the
Stellaris Launchapd, cortex m4 ARM 
processor. The program operates a rover
by pulse width
modulation and accept commands related to 
speed and direction and sends sensor data 
through serial port to RASPBERRY PI, which is
further connected wirelessly to laptop
(see rover-transmitter repo).

Installation
------------
To build the cross-compiler tools for Linux for
ARM-development follow the tutorial
http://recursive-labs.com/blog/2012/10/28/stellaris-launchpad-gnu-linux-getting-started/ .

The startup_gcc.c and the linker file are not included in this repo. Pick them from the examples
from the above link. Add the interrupt handlers: 
motor_timer_interrupt();
UARTIntHandler(void);
IntGPIOd(void);
system_time(void);
 
to correct places in the startup_gcc.c file.

To build the program it is recommended to set up an
Eclipse project with integrated openocd and gdp debuggers.
A great tutorial to how to set up an Eclipse project and
open-ocd for
the stellaris launchpad can be found from the following link
http://kernelhacks.blogspot.fi/2012/11/


