// hardware.h
// Used to include Arduino code in main

#ifndef HARDWARE_H
#define HARDWARE_H

void readCommand();
void runCommand(char command, bool log);
int detectDistance();
int scan();

#endif // HARDWARE_H