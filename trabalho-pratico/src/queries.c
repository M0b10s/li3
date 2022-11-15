/*	details:

		dates are in the format dd/mm/yyyy

		accounts assume active or inactive

		car_class:

			Basic : price : 3,25€ + 0,62€/km
			Green : price : 4,00€ + 0,79€/km
			Premium : price : 5,20€ + 0,94€/km
		
		tips are in the format € and don't consider the value of the ride
		
		float results are rounded to 3 decimal places

		ride values are in the format €

		queries with invalid inputs generate a empty file



		Users dataset by index:
		
		username -> string
		name -> string
		gender -> string (M/F) -> Enum
		birth_date -> string (dd/mm/yyyy)
		account_creation_date -> string (dd/mm/yyyy)
		pay_method -> string (cash/credit card/debit card) -> Enum
		account_status -> string (active/inactive) -> Enum


		Drivers dataset by index:

		id -> int
		name -> string
		birth_day -> string (dd/mm/yyyy)
		gender -> string (M/F) -> Enum
		car_class -> string (basic/green/premium) -> Enum
		license_plate -> string
		city -> string (Lisboa/Porto/Faro/Braga/Setúbal) -> Enum
		account_creation -> string (dd/mm/yyyy)
		account_status -> string (active/inactive) -> Enum


		Rides dataset by index:
		
		id -> int
		date -> string (dd/mm/yyyy)
		driver -> int
		user -> string
		city -> string (Lisboa/Porto/Faro/Braga/Setúbal) -> Enum
		distance -> int
		score_user -> int
		score_driver -> int
		tip -> double (€)
		comment -> string


		Queries:

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

gint compare_drivers(gconstpointer a, gconstpointer b){

	DATA_DRIVER driver1 = (DATA_DRIVER) a;
	DATA_DRIVER driver2 = (DATA_DRIVER) b;

	int non_div_zero_a = 0;
	int non_div_zero_b = 0;
	if(get_num_viagens_driver(driver1) == 0) non_div_zero_a = 1;
	if(get_num_viagens_driver(driver2) == 0) non_div_zero_b = 1;

	if(get_avaliacao_total_driver(driver1)/get_num_viagens_driver(driver1)+non_div_zero_a > get_avaliacao_total_driver(driver2)/get_num_viagens_driver(driver2)+non_div_zero_b)
		return -1;
	else if(get_avaliacao_total_driver(driver1)/get_num_viagens_driver(driver1)+non_div_zero_a < get_avaliacao_total_driver(driver2)/get_num_viagens_driver(driver2)+non_div_zero_b)
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
										if(get_num_viagens(user) == 0) non_div_zero=1;
										fprintf(output_file_pointer,"%s;%c;%d;%.3f;%d;%.3f\n",name_user,aux,get_idade(user),((double)get_total_avaliacao(user)/(double)get_num_viagens(user)+(double)non_div_zero),get_num_viagens(user),get_total_gasto(user));
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

				// GList *list = g_hash_table_get_values(DB_drivers);
				// list = g_list_sort(list,compare_drivers);
				// int n = atoi(token);
				// int i = 0;
				// while(i<n){
				// 	fprintf(output_file_pointer,"%d;%s;%.3f\n",get_id_driver(g_list_nth_data(list,i)),get_name_driver(g_list_nth_data(list,i)),(double)get_avaliacao_total_driver(g_list_nth_data(list,i))/get_num_viagens_driver(g_list_nth_data(list,i)));
				// 	i++;
				// }


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