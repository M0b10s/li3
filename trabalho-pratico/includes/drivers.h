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
int get_gender_driver(DATA_DRIVER driver);
int get_car_class(DATA_DRIVER driver);
char *get_license_plate(DATA_DRIVER driver);
int get_city_driver(DATA_DRIVER driver);
void get_account_creation_driver(DATA_DRIVER driver, struct tm *dest);
int get_account_status_driver(DATA_DRIVER driver);
int get_age_driver(DATA_DRIVER driver);
int get_num_viagens_driver(DATA_DRIVER driver);
int get_avaliacao_total_driver(DATA_DRIVER driver);
double get_total_auferido_driver(DATA_DRIVER driver);



void set_increment_num_viagens_driver(DATA_DRIVER driver);
void set_increment_total_avaliacao_driver(DATA_DRIVER driver,int avaliacao);
void set_increment_total_auferido_driver(DATA_DRIVER driver,double gasto);


void free_driver(DATA_DRIVER driver);
void print_driver(DATA_DRIVER driver);
DATA_DRIVER clone_driver(DATA_DRIVER driver);
DATA_DRIVER create_driver(char *drivers_line);
void load_drivers_to_DB(GHashTable *DB_drivers,FILE *drivers_file_pointer);



#endif