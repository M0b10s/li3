/*		Queries:

			1 - get details from user with username <username> or driver with id <id>
			1 output : name;gender;age;score;number_rides;total_spent/earned

			2 - list the n drivers with the highest average score
			2 output : id;name;average_score

			3 - list n users with highest travel distance
			3 output : username;name;total_distance

			4 - Average price of rides in city <city>
			4 output : average_price
			
			5 - Average price of rides in a time interval
			5 output : average_price
			
			6 - Average distance in a city in a time interval
			6 output : average_distance
			
			7 - list n drivers in city <city> ordered by average score descending if average score is equal order by id descending
			7 output : id;name;average_score

			8 - list all rides on witch user and driver have, X gender and Y age. output order by date of account_creation of driver. 
			if equal order by date of account creation of user, If still equals order by id of ride
			8 output : id_driver;name_driver;username_user;name_user

			9 - list all rides where user tip in a time interval. order results by distance descending if equal most recent first if still equal order by id of ride descending
			9 output : id_ride;date;distance;city;tip
			*/

#include "queries.h"

gint compare_drivers_orderby_score_id(gconstpointer a, gconstpointer b){

	DATA_DRIVER driver1 = (DATA_DRIVER) a;
	DATA_DRIVER driver2 = (DATA_DRIVER) b;

	int non_div_zero_a = 0;
	int non_div_zero_b = 0;
	if(get_num_viagens_driver(driver1) == 0) non_div_zero_a = 1;
	if(get_num_viagens_driver(driver2) == 0) non_div_zero_b = 1;

	if((double)get_avaliacao_total_driver(driver1)/(double)get_num_viagens_driver(driver1)+non_div_zero_a > (double)get_avaliacao_total_driver(driver2)/(double)get_num_viagens_driver(driver2)+non_div_zero_b)
	
		return -1;
	
	else if((double)get_avaliacao_total_driver(driver1)/(double)get_num_viagens_driver(driver1)+non_div_zero_a < (double)get_avaliacao_total_driver(driver2)/(double)get_num_viagens_driver(driver2)+non_div_zero_b)
	
		return 1;
	
	else{
		
		if(get_id_driver(driver1) > get_id_driver(driver2))
		
			return -1;
		
		else if(get_id_driver(driver1) < get_id_driver(driver2))
		
			return 1;
		
		else

			return 0;
	}

}

int compare_tmdates(struct tm date1, struct tm date2){

	if(date1.tm_year > date2.tm_year)
		return 1;
	else if(date1.tm_year < date2.tm_year)
		return -1;

	else{

		if(date1.tm_mon > date2.tm_mon)
			return 1;
		else if(date1.tm_mon < date2.tm_mon)
			return -1;
		
		else{

			if(date1.tm_mday > date2.tm_mday)
				return 1;
			else if(date1.tm_mday < date2.tm_mday)
				return -1;
			else
				return 0;
		
		}
	
	}

}

double get_cost_ride(DATA_RIDES ride, GHashTable* drivers){

	int distancia=0;

	if(ride == NULL){printf("funk");return -1;}

	distancia = get_distance_rides(ride);
	int car_type = 0;
	DATA_DRIVER driver = g_hash_table_lookup(drivers, GINT_TO_POINTER(get_id_driver_rides(ride)));
	if(driver == NULL){printf("funk");return -1;}
	car_type = get_car_class(driver);


	if(car_type == 0) return distancia*0.62+3.25;
	else if(car_type == 1) return distancia*0.79+4.0;
	else if(car_type == 2) return distancia*0.94+5.20;
	
	else {printf("funk");return -1;}
}



void start_queries(FILE *commands_file_pointer, GHashTable *DB_users, GHashTable *DB_drivers, GHashTable *DB_rides){

	char *line=NULL;
	char *command=malloc(1000*sizeof(char));
	int i = 1;
	size_t len = 0;
	ssize_t read;

	while ((read = getline(&line, &len, commands_file_pointer)) != -1) {
		// printf("Retrieved line nº %d of length %zu ===> ",i, read);
		// printf("%s", line);

		//process command
		sprintf(command,"%s%d%s","Resultados/command",i,"_output.txt");
		
		FILE *output_file_pointer = fopen(command,"w+");

		char *token = strtok(line, " ");
		int commando = atoi(token);

		token = strtok(NULL, " ");

		switch (commando){

			case 1:
				
				strtok(token, "\n ");

				if (g_hash_table_contains(DB_users,token)){

					//we have a user
					DATA_USER user = g_hash_table_lookup(DB_users,token);

					if(!get_account_status_user(user)){
						char aux;
						int non_div_zero=0;
						char *name_user = get_name_user(user);
										if(get_gender_user(user) == 0) aux = 'M'; else aux = 'F';
										if(get_num_viagens_user(user) == 0) non_div_zero=1;
										fprintf(output_file_pointer,"%s;%c;%d;%.3f;%d;%.3f\n",name_user,aux,get_idade_user(user),((double)get_total_avaliacao_user(user)/(double)get_num_viagens_user(user)+(double)non_div_zero),get_num_viagens_user(user),get_total_gasto_user(user));
						free(name_user);
					}

				}

				else if (g_hash_table_contains(DB_drivers,GINT_TO_POINTER(atoi(token)))){

					//we have a driver
					DATA_DRIVER driver = g_hash_table_lookup(DB_drivers,GINT_TO_POINTER(atoi(token)));
					
					if(!get_account_status_driver(driver)){
						char aux;
						int non_div_zero=0;
						char *name_driver = get_name_driver(driver);

										if(get_gender_driver(driver) == 0) aux = 'M'; else aux = 'F';
										if(get_num_viagens_driver(driver) == 0) non_div_zero=1;
										fprintf(output_file_pointer,"%s;%c;%d;%.3f;%d;%.3f\n",name_driver,aux,get_age_driver(driver),((double)get_avaliacao_total_driver(driver)/(double)get_num_viagens_driver(driver)+(double)non_div_zero),get_num_viagens_driver(driver),get_total_auferido_driver(driver));
						free(name_driver);
					}

				}

				break;

			case 2:

				GList *list = g_hash_table_get_values(DB_drivers);
				list = g_list_sort(list,compare_drivers_orderby_score_id);
				int n = atoi(token);
				int counter_q2 = 0;
				char *name_driver=NULL;
				while(counter_q2<n){
					name_driver = get_name_driver(g_list_nth_data(list,counter_q2));
					if(!get_account_status_driver(g_list_nth_data(list,counter_q2)))
					fprintf(output_file_pointer,"%012d;%s;%.3f\n",get_id_driver(g_list_nth_data(list,i)),name_driver,(double)get_avaliacao_total_driver(g_list_nth_data(list,i))/(double)get_num_viagens_driver(g_list_nth_data(list,i)));
					counter_q2++;
					free(name_driver);
				}

				g_list_free(list);

				break;

			case 3:
				break;

			case 4:
				
				char *city = strtok(token, "\n ");
				int city_int=0;

				if(!strcmp(city,"Lisboa")) city_int = 0;
				else if(!strcmp(city,"Porto")) city_int = 1;
				else if(!strcmp(city,"Faro")) city_int = 2;
				else if(!strcmp(city,"Braga")) city_int = 3;
				else if(!strcmp(city,"Setúbal")) city_int = 4;


				GHashTableIter iter;
				gpointer key, value;
				g_hash_table_iter_init (&iter, DB_rides);
				double total_cost=0;
				int total_rides=0;
				while(g_hash_table_iter_next (&iter, &key, &value)){

					if(get_city_rides(value) == city_int){
						total_cost += get_cost_ride(g_hash_table_lookup(DB_rides,GINT_TO_POINTER(get_id_rides(value))),DB_drivers);
						total_rides++;
					}

				}

				int nz = 0;
				if(total_rides == 0) nz=1;
				double average_cost = (double)total_cost/(double)total_rides+(double)nz;
				fprintf(output_file_pointer,"%.3f\n",average_cost);

				break;

			
			default:
				
				// printf("\n");
				
				// printf("Line Nº %d -> Invalid command!! ❌",i);
				
				// printf("\n");

				break;
	
		}
			i++;
			fclose(output_file_pointer);
	
	}

	free(line);
	free(command);

}