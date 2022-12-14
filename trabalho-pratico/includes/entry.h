#ifndef main_maker
#define main_maker
#define _GNU_SOURCE //Usage of strptime

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h> 
#include <unistd.h> 
#include <sys/ioctl.h>
#include <users.h>
#include <drivers.h>
#include <rides.h>
#include <queries.h>
#include <menu.h>

//=======================================================================================================================================

void printCenter(char *s, int size);
int Directory_verification(int argc, char const *argv[],int size);
int Start_interactive_mode(int size, char *input_dir);
int main(int argc, char const *argv[]);

//=======================================================================================================================================

#endif