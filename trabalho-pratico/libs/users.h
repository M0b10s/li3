#ifndef users_maker
#define users_maker
#define _GNU_SOURCE //Usage of strptime


#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>


typedef struct data_user *DATA_USER;


//========================================================GETTERS======================================================================
char *get_username(DATA_USER user);
char *get_name_user(DATA_USER user);
int get_gender_user(DATA_USER user);
void get_birth_date_user(DATA_USER user,struct tm *dest);
void get_account_creation_user(DATA_USER user, struct tm *dest);
int get_pay_method_user(DATA_USER user);
int get_account_status_user(DATA_USER user);
//=======================================================================================================================================


void free_user(DATA_USER user);
DATA_USER create_user(char *users_line);
void load_users_to_DB(GHashTable *DB_users,FILE *users_file_pointer);


#endif