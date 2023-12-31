#include <rides.h>

enum city{Lisboa,Porto,Faro,Braga,Setubal,Vila,Coimbra};

#define BASIC_RIDE 3.25
#define GREEN_RIDE 4.00
#define PREMIUM_RIDE 5.20
#define BASIC_KM 0.62
#define GREEN_KM 0.79
#define PREMIUM_KM 0.94


typedef struct data_rides{

	int id;
	struct tm date;
	int id_driver;
	char *username;
	enum city city;
	int distance;
	double score_user;
	double score_driver;
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

void free_rides_void(void* ride){

  free_rides(ride);

}

void print_rides(DATA_RIDES ride){

	printf("ID: %d\n",ride->id);
	printf("Date: %d/%d/%d\n",ride->date.tm_mday,ride->date.tm_mon,ride->date.tm_year+1900);
	printf("ID Driver: %d\n",ride->id_driver);
	printf("Username: %s\n",ride->username);
	printf("City: %d\n",ride->city);
	printf("Distance: %d\n",ride->distance);
	printf("Score User: %f\n",ride->score_user);
	printf("Score Driver: %f\n",ride->score_driver);
	printf("Tip: %f\n",ride->tip);
	printf("Comment: %s\n",ride->comment);

}

void go_stats_rides(DATA_RIDES *ride, GHashTable *users, GHashTable *drivers){

	char *username = get_username_rides(*ride);
	DATA_USER user = g_hash_table_lookup(users,username);

	gpointer key = GINT_TO_POINTER(get_id_driver_rides(*ride));
	DATA_DRIVER driver = g_hash_table_lookup(drivers,key);
	
	if(!user || !driver) *ride = NULL;

	else {

		set_increment_num_viagens_user(user);
		set_increment_num_viagens_driver(driver);
		set_increment_total_avaliacao_user(user,get_score_user_rides(*ride));
		set_increment_total_avaliacao_driver(driver,get_score_driver_rides(*ride));
		set_increment_distancia_viajada_user(user,get_distance_rides(*ride));

		if(get_car_class(driver) == 0){
			double aux = BASIC_RIDE + (BASIC_KM * get_distance_rides(*ride))+get_tip_rides(*ride);
			set_increment_total_gasto_user(user,aux);
			set_increment_total_auferido_driver(driver,aux);
			}
		else if(get_car_class(driver) == 1){
			double aux = GREEN_RIDE + (GREEN_KM * get_distance_rides(*ride))+get_tip_rides(*ride);
			set_increment_total_gasto_user(user,aux);
			set_increment_total_auferido_driver(driver,aux);
			}
		else{
			double aux = PREMIUM_RIDE + (PREMIUM_KM * get_distance_rides(*ride))+get_tip_rides(*ride);
			set_increment_total_gasto_user(user,aux);
			set_increment_total_auferido_driver(driver,aux);
			}

	}
	
	free(username);

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
				
				if(token){

					if(!atoi(token)) flag=1;
					ride->id = atoi(token);
				
				}

				else flag=1;
				
				break;
			
			case 1:

				if(token){

					if(strptime(token,"%d/%m/%Y",&ride->date)==NULL) flag=1;
				
				}

				else flag=1;

				break;

			case 2:

				if(token){

					if(!atoi(token)) flag=1;
					
					else ride->id_driver = atoi(token);

				}

				else flag=1;

				break;
			
			case 3:

				if(token)	ride->username = strdup(token);

				else flag=1;

				break;
			
			case 4:

				if(token){

					if(!strcmp(token,"Lisboa")) ride->city = Lisboa;
					else if(!strcmp(token,"Porto")) ride->city = Porto;
					else if(!strcmp(token,"Faro")) ride->city = Faro;
					else if(!strcmp(token,"Braga")) ride->city = Braga;
					else if(!strcmp(token,"Setúbal")) ride->city = Setubal;
					else if(!strcmp(token,"Vila real")) ride->city = Vila;
					else if(!strcmp(token,"Coimbra")) ride->city = Coimbra;
					else flag=1;
				
				}

				else flag=1;

				break;

			case 5: 
				
				if(token){
					if(!atoi(token) || atoi(token) <= 0 || isnan(atof(token))) flag=1;
					ride->distance = atoi(token);
				}

				else flag=1;

				break;

			case 6:


				if(token){


				char *aux = token;

				for ( ; *aux; ++aux) if(*aux == ',') flag=1;

					if(!atof(token) || atof(token) <= 0 || isnan(atof(token))) flag=1;
					ride->score_user = atof(token);
				
				}

				else flag=1;

				break;

			case 7:

				if(token){


				char *aux = token;

				for ( ; *aux; ++aux) if(*aux == ',') flag=1;

					if(!atof(token) || atof(token) <= 0 || isnan(atof(token))) flag=1;
					ride->score_driver = atof(token);
				}

				else flag=1;

				break;
			
			case 8:

				if(token){


				char *aux = token;

				for ( ; *aux; ++aux) if(*aux == ',') flag=1;

					if(!atof(token) || atof(token) <= 0 || isnan(atof(token))) flag=1;
					ride->tip = atof(token);
				}

				else flag=1;

				break;
			
			case 9:

				if(token){
					ride->comment = strdup(token);
				}

				break;

		}

		i++;

	}

	//INITIALIZE OTHER VALUES
	ride->date.tm_mon += 1;

	if(flag == 1 || i<9){
		// printf("ERROR IN RIDE DATA!!!\n");
		ride = NULL;
	}

	// printf("\n");


	free(token);

	if (ride) go_stats_rides(&ride,users,drivers);

	if(ride){

		char *username = get_username_rides(ride);

		DATA_USER user_to_check = g_hash_table_lookup(users,username);
		DATA_DRIVER driver_to_check = g_hash_table_lookup(drivers,GINT_TO_POINTER(ride->id_driver));

		struct tm date_user;
		struct tm date_driver;

		get_data_ultima_ride_user(user_to_check,&date_user);
		get_data_ultima_ride_driver(driver_to_check,&date_driver);

			if(compare_tmdates(date_user,ride->date) == -1)
				set_data_ultima_ride_user(user_to_check,ride->date.tm_mday,ride->date.tm_mon,ride->date.tm_year);
			
			if(compare_tmdates(date_driver,ride->date) == -1)
				set_data_ultima_ride_driver(driver_to_check,ride->date.tm_mday,ride->date.tm_mon,ride->date.tm_year);

		free(username);

	}

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