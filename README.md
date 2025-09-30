# Sumo Robot #

### Purpose ###

An embedded systems project for a robot to track and attack enemies in a sumo platform. Built using the MSP430 microcontroller.

### Features ####

* Performs static analysis using CPPCHECK by doing `make cppcheck`
* Dockerized and uses GitHub Actions as CI/CD to build and deploy
* Uses Makefile to build and clean the project
* Formatted based on clang-format (18.1.3)
* Utilized optimized, embedded version of printf() for logging paired with UART serial communication for terminal tracing from microcontroller to host
* Ring buffer implemented to store elements
* State machine to store and determine actions for robot

## Docker

Reference to the Docker container to build the latest image using Ubuntu 24.04:
https://hub.docker.com/repository/docker/samloveswater/msp430-gcc-9.3.1.11/general

## make (Makefile)

Code targets specifically the MSP430G2553 and uses TI's compiler msp430-gcc. Refer to [msp430-gcc-open](https://www.ti.com/tool/MSP430-GCC-OPENSOURCE) for what is utilized. The Makefile was built from scratch and used parts of the TI compiler for the project, however not CCStudioIDE.

## Assert
In this implementation, the assert is first triggered at a breakpoint
(if a debugger is attached) and traces the address of the assert and then
blinks the LED indefinitely. The components printed are the memory address,
program counter, and _add2line_ that retrieves file and line number, and there
is a makefile rule for it. 

## References

* [Datasheet for MSP430G2x53](https://www.ti.com/lit/ds/symlink/msp430g2553.pdf)
* [User's Guide for MSP430F2xx, MSP430G2xx Family](https://www.ti.com/lit/ug/slau144k/slau144k.pdf?ts=1758604666286&ref_url=http%253A%252F%252Fwww.ti.com%252Flit%252Fug%252Fslau144j%252Fslau144j.pdf)