#ifndef menu_maker
#define menu_maker
#define _GNU_SOURCE 

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <glib.h>
#include <math.h>
#include <termios.h>
#include <unistd.h>
#include <entry.h>

typedef struct file{
	int n_request;
	int n_querie;
	char *arg1;
	char *arg2;
	char *arg3;
}*file;

//=======================================================================================================================================

void menu_principal(unsigned int size,unsigned int size_lines);
file input_level_1(char x,int *valid_run,GList *list_inputs,int size);
file do_queries(int *valid_run);
void free_file(file input);
void file_stand_print(file input);
void fill_file (file input,char *name);
void print_result_on_screen(int size_of_window,int file_index);
// int number_of_results_in_query(FILE *fp);
void wait_for_enter();
void wait_for_enter_v2();

//=======================================================================================================================================

#endif