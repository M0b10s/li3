#ifndef drivers_maker
#define drivers_maker
#define _GNU_SOURCE //Usage of strptime

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>


typedef struct data_driver *DATA_DRIVER;

int get_id_driver(DATA_DRIVER driver);
char *get_name_driver(DATA_DRIVER driver);
void get_birth_day(DATA_DRIVER driver, struct tm *dest);
int get_car_class(DATA_DRIVER driver);
char *get_license_plate(DATA_DRIVER driver);
int get_city(DATA_DRIVER driver);
void get_account_creation_driver(DATA_DRIVER driver, struct tm *dest);
int get_account_status_driver(DATA_DRIVER driver);


void free_driver(DATA_DRIVER driver);
DATA_DRIVER create_driver(char *drivers_line);
void load_drivers_to_DB(GHashTable *DB_drivers,FILE *drivers_file_pointer);

#endif