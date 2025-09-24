# Sumo Robot #

### Purpose ###

An embedded systems project for a robot to track and attack enemies in a sumo platform. Built using the MSP430 microcontroller.

### Features ####

* Performs static analysis using CPPCHECK by doing `make cppcheck`
* Dockerized and uses GitHub Actions as CI/CD to build and deploy
* Uses Makefile to build and clean the project
* Formatted based on clang-format-18

## Assert
In this implementation, the assert is first triggered at a breakpoint
(if a debugger is attached) and traces the address of the assert and then
blinks the LED indefinitely. The components printed are the memory address,
program counter, and _add2line_ that retrieves file and line number, and there
is a makefile rule for it. 
