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



typedef struct data_user *DATA_USER;


void start_queries(FILE *commands_file_pointer, GHashTable *DB_users, GHashTable *DB_drivers, GHashTable *DB_rides);
gint compare_drivers_orderby_score_id(gconstpointer a, gconstpointer b);
GList* get_rides_from_driver(GHashTable* rides, int id_driver);
double get_cost_ride(DATA_RIDES ride, GHashTable* drivers);


#endif