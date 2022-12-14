#ifndef queries_maker
#define queries_maker
#define _GNU_SOURCE //Usage of strptime


#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include "users.h"
#include "drivers.h"
#include "rides.h"
#include "menu.h"



typedef struct data_user *DATA_USER;


void start_queries(FILE *commands_file_pointer, GHashTable *DB_users, GHashTable *DB_drivers, GHashTable *DB_rides,int interactive_mode_index);
int compare_tmdates(struct tm date1, struct tm date2);


#endif