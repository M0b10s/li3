#ifndef rides_maker
#define rides_maker
#define _GNU_SOURCE //Usage of strptime

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>

typedef struct data_rides *DATA_RIDES;

int get_id_rides(DATA_RIDES ride);
void get_date_rides(DATA_RIDES ride, struct tm *dest);
int get_id_driver_rides(DATA_RIDES ride);
char *get_username_rides(DATA_RIDES ride);
int get_city_rides(DATA_RIDES ride);
int get_distance_rides(DATA_RIDES ride);
int get_score_user_rides(DATA_RIDES ride);
int get_score_driver_rides(DATA_RIDES ride);
double get_tip_rides(DATA_RIDES ride);
char *get_comment_rides(DATA_RIDES ride);

void free_rides(DATA_RIDES ride);
DATA_RIDES create_rides(char *rides_line);
void load_rides_to_DB(GHashTable *DB_rides,FILE *rides_file_pointer);


#endif