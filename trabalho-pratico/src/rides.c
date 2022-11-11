#include <rides.h>

enum city{Lisboa,Porto,Faro,Braga,Setubal};

typedef struct data_rides{

	int id;
	struct tm date;
	int id_driver;
	char *username;
	enum city city;
	int distance;
	int score_user;
	int score_driver;
	double tip;
	char *comment;


}*DATA_RIDES;

//===============================================Get's=====================================================

int get_id_rides(DATA_RIDES ride){

	return ride->id;

}

void get_date_rides(DATA_RIDES ride, struct tm *dest){

	dest->tm_mday = ride->date.tm_mday;
	dest->tm_mon = ride->date.tm_mon;
	dest->tm_year = ride->date.tm_year;

}

int get_id_driver_rides(DATA_RIDES ride){

	return ride->id_driver;

}

char *get_username_rides(DATA_RIDES ride){

	return strdup(ride->username);

}

int get_city_rides(DATA_RIDES ride){

	return ride->city;

}

int get_distance_rides(DATA_RIDES ride){

	return ride->distance;

}

int get_score_user_rides(DATA_RIDES ride){

	return ride->score_user;

}

int get_score_driver_rides(DATA_RIDES ride){

	return ride->score_driver;

}

double get_tip_rides(DATA_RIDES ride){

	return ride->tip;

}

char *get_comment_rides(DATA_RIDES ride){

	return strdup(ride->comment);

}

//===============================================END Get's=================================================

void free_rides(DATA_RIDES ride){

	free(ride->username);
	free(ride->comment);
	free(ride);

}

DATA_RIDES create_rides(char *rides_line,GHashTable *users, GHashTable *drivers){


	DATA_RIDES ride = malloc(sizeof(struct data_rides));

	ride->username = NULL;
	ride->comment = NULL;

	char *token=NULL;
	char *rest = rides_line;

	int i=0,flag=0;

	while((token = strtok_r(rest,";\n",&rest))){


		switch(i){

			case 0: 
				
				if(!atoi(token)) flag=1;
				ride->id = atoi(token);
				
				break;
			
			case 1:

				if(strptime(token,"%d/%m/%Y",&ride->date)==NULL) flag=1;
				
				break;

			case 2:

				if(!atoi(token)) flag=1;
				
				else{

					ride->id_driver = atoi(token);

					// lookup on hashtable drivers for the id_driver and increment one to the number of rides

					gpointer key = GINT_TO_POINTER(ride->id_driver);

					DATA_DRIVER driver = g_hash_table_lookup(drivers,key);

					if(!driver) flag=1;

					set_increment_num_viagens_driver(driver);

					}

				break;
			
			case 3:

				ride->username = strdup(token);

				// lookup on hashtable users for username and increment one to the number of rides

				DATA_USER user = g_hash_table_lookup(users,ride->username);

				if(!user) flag=1;

				set_increment_num_viagens(user);


				break;
			
			case 4:

				if(!strcmp(token,"Lisboa")) ride->city = Lisboa;
				else if(!strcmp(token,"Porto")) ride->city = Porto;
				else if(!strcmp(token,"Faro")) ride->city = Faro;
				else if(!strcmp(token,"Braga")) ride->city = Braga;
				else if(!strcmp(token,"Setúbal")) ride->city = Setubal;
				break;

			case 5: 
				
				if(!atoi(token)) flag=1;
				ride->distance = atoi(token);

				break;

			case 6:

				if(!atoi(token)) flag=1;
				ride->score_user = atoi(token);

				break;

			case 7:

				if(!atoi(token)) flag=1;
				ride->score_driver = atoi(token);

				break;
			
			case 8:

				if(!atof(token)) flag=1;
				ride->tip = atof(token);

				break;
			
			case 9:

				ride->comment = strdup(token);

				break;

		}

		i++;

	}



	if(flag == 1 || i<9){
		// printf("ERROR IN RIDE DATA!!!\n");
		ride = NULL;
	}

	// printf("\n");


	free(token);


	return ride;


}

void load_rides_to_DB(GHashTable *DB_rides,FILE *rides_file_pointer,GHashTable *DB_users,GHashTable *DB_drivers){
	
	//load datab_rides to ghash table
	
	char *rides_line = NULL;
	size_t rides_line_size = 0;
	ssize_t rides_line_size_read;

	DATA_RIDES ride = NULL;
	gint id;

	printf("\n==================================> Loading rides to DB...\n\n");

	while((rides_line_size_read = getline(&rides_line,&rides_line_size,rides_file_pointer)) != -1){

		ride = create_rides(rides_line,DB_users,DB_drivers);

		if(ride){
			id = get_id_rides(ride);
			g_hash_table_insert(DB_rides,GINT_TO_POINTER(id),ride);
		}
 
	}
	
	printf("\n==================================> Rides loaded to DB ✅\n\n\n");

	free(rides_line);

}