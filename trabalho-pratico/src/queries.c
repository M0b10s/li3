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


typedef struct aux_struct_q8{

	int id_condutor;
	char *nome_condutor;
	char *username_utilizador;
	char *nome_utilizador;
	struct tm data_driver;
	struct tm data_user;
	int id_viagem;

}*AUX_STRUCT_Q8;


void free_aux_struct_q8(AUX_STRUCT_Q8 elem){

	free(elem->nome_condutor);
	free(elem->username_utilizador);
	free(elem->nome_utilizador);
	free(elem);

}


#define DAY_CALC 9
#define MON_CALC 10
#define YEA_CALC 2022

// compare q8 by date of driver and return,or if equal, by date of user an return, or if equal, by id of ride
gint compare_q8(gconstpointer a, gconstpointer b){
	AUX_STRUCT_Q8 elem1 = (AUX_STRUCT_Q8) a;
	AUX_STRUCT_Q8 elem2 = (AUX_STRUCT_Q8) b;
	if(compare_tmdates(elem1->data_driver,elem2->data_driver) == 0){
		if(compare_tmdates(elem1->data_user,elem2->data_user) == 0){
			if(elem1->id_viagem > elem2->id_viagem){
				return 1;
			}
			else{
				return 0;
			}
		}
		else{
			if(compare_tmdates(elem1->data_user,elem2->data_user) == 1){
				return 1;
			}
			else{
				return 0;
			}
		}
	}
	else{
		if(compare_tmdates(elem1->data_driver,elem2->data_driver) == 1){
			return 1;
		}
		else{
			return 0;
		}
	}
}


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

	if(ride == NULL) return -1;

	distancia = get_distance_rides(ride);
	int car_type = 0;
	DATA_DRIVER driver = g_hash_table_lookup(drivers, GINT_TO_POINTER(get_id_driver_rides(ride)));
	if(driver == NULL) return -1;
	car_type = get_car_class(driver);


	if(car_type == 0) return distancia*0.62+3.25;
	else if(car_type == 1) return distancia*0.79+4.0;
	else if(car_type == 2) return distancia*0.94+5.20;
	
	else return -1;
}

int num_anos_perfil_user(DATA_USER user){
	
	struct tm date;

	get_account_creation_user(user,&date);

	int anos = YEA_CALC - date.tm_year-1900;
	if(date.tm_mon > MON_CALC) anos--;
	else if(date.tm_mon == MON_CALC && date.tm_mday > DAY_CALC) anos--;
	
	return anos;

}


int num_anos_perfil_driver(DATA_DRIVER driver){
	
	struct tm date;

	get_account_creation_driver(driver,&date);

	int anos = YEA_CALC - date.tm_year-1900;
	if(date.tm_mon > MON_CALC) anos--;
	else if(date.tm_mon == MON_CALC && date.tm_mday > DAY_CALC) anos--;
	
	return anos;

}


void start_queries(FILE *commands_file_pointer, GHashTable *DB_users, GHashTable *DB_drivers, GHashTable *DB_rides){

	char *line=NULL;
	char *output_file_name=malloc(1000*sizeof(char));
	int i = 1;
	size_t len = 0;
	ssize_t read;

	while ((read = getline(&line, &len, commands_file_pointer)) != -1) {
		// printf("Retrieved line nº %d of length %zu ===> ",i, read);
		// printf("%s", line);

		//process command
		sprintf(output_file_name,"%s%d%s","Resultados/command",i,"_output.txt");
		
		FILE *output_file_pointer = fopen(output_file_name,"w+");

		char *multi_arg = strdup(line);
		char *tofree = multi_arg;
		char *token = strtok(line, " ");
		int commando = atoi(token);

		switch (commando){

			case 1:
				
				strsep(&multi_arg, " \n");
				strtok(multi_arg, "\n");
				
				if (g_hash_table_contains(DB_users,multi_arg)){

					//we have a user
					DATA_USER user = g_hash_table_lookup(DB_users,multi_arg);

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

				else if (g_hash_table_contains(DB_drivers,GINT_TO_POINTER(atoi(multi_arg)))){

					//we have a driver
					DATA_DRIVER driver = g_hash_table_lookup(DB_drivers,GINT_TO_POINTER(atoi(multi_arg)));
					
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

				strsep(&multi_arg, "\n ");
				strtok(multi_arg, "\n");

				GList *list = g_hash_table_get_values(DB_drivers);
				list = g_list_sort(list,compare_drivers_orderby_score_id);
				int n = atoi(multi_arg);
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
				strsep(&multi_arg, "\n ");
				strtok(multi_arg, "\n");
				break;

			case 4:
				
				strsep(&multi_arg, "\n ");
				char *city = strtok(multi_arg, "\n ");
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

			case 5:

				struct tm data_inicio;
				struct tm data_fim;
				char *dates;

				dates = strsep(&multi_arg, " ");
				dates = strsep(&multi_arg, " ");

				strptime(dates,"%d/%m/%Y",&data_inicio);
				data_inicio.tm_mon++;

				dates = strsep(&multi_arg, " ");

				strptime(dates,"%d/%m/%Y",&data_fim);
				data_fim.tm_mon++;

				//get all rides in the interval data_inicio - data_fim
				GHashTableIter iterq5;
				gpointer key2, valueq5;
				g_hash_table_iter_init (&iterq5, DB_rides);
				
				struct tm data_1_struct;

				//get all rides in the interval data_inicio - data_fim in a glist
				GList *list_rides = NULL;
				while(g_hash_table_iter_next(&iterq5, &key2, &valueq5)){
				
				get_date_rides(valueq5,&data_1_struct);

					if(compare_tmdates(data_1_struct,data_inicio) >= 0  && compare_tmdates(data_1_struct,data_fim) <= 0){
				
						list_rides = g_list_prepend(list_rides,valueq5);
					
					}

				}

				double total_cost_q5=0;
				int total_rides_q5=0;
				GList *aux_q5 = list_rides;

				while(aux_q5!=NULL){
					total_cost_q5 += get_cost_ride(g_hash_table_lookup(DB_rides,GINT_TO_POINTER(get_id_rides(aux_q5->data))),DB_drivers);
					total_rides_q5++;
					aux_q5 = aux_q5->next;
				}

				if(total_rides_q5 == 0) break;

				else{

					double average_cost_q5 = (double)total_cost_q5/(double)total_rides_q5; 

					fprintf(output_file_pointer,"%.3f\n",average_cost_q5);
				}

				g_list_free(list_rides);

				break;

			case 6:

				struct tm data_inicio_q6;
				struct tm data_fim_q6;
				char *dates_q6;
				char *city_q6;

				city_q6 = strsep(&multi_arg, " ");
				city_q6 = strsep(&multi_arg, " ");
				dates_q6 = strsep(&multi_arg, " ");

				strptime(dates_q6,"%d/%m/%Y",&data_inicio_q6);
				data_inicio_q6.tm_mon++;

				dates_q6 = strsep(&multi_arg, " ");

				strptime(dates_q6,"%d/%m/%Y",&data_fim_q6);
				data_fim_q6.tm_mon++;


				GHashTableIter iterq6;
				gpointer keyq6, valueq6;
				g_hash_table_iter_init (&iterq6, DB_rides);
				
				struct tm data_1_struct_q6;
				int city_int_q6=0;

				if(!strcmp(city_q6,"Lisboa")) city_int_q6 = 0;
				else if(!strcmp(city_q6,"Porto")) city_int_q6 = 1;
				else if(!strcmp(city_q6,"Faro")) city_int_q6 = 2;
				else if(!strcmp(city_q6,"Braga")) city_int_q6 = 3;
				else if(!strcmp(city_q6,"Setúbal")) city_int_q6 = 4;

			
				GList *list_rides_q6 = NULL;
				while(g_hash_table_iter_next(&iterq6, &keyq6, &valueq6)){
				
				get_date_rides(valueq6,&data_1_struct_q6);

					if(compare_tmdates(data_1_struct_q6,data_inicio_q6) >= 0  && compare_tmdates(data_1_struct_q6,data_fim_q6) <= 0 && get_city_rides(valueq6) == city_int_q6){
				
						list_rides_q6 = g_list_prepend(list_rides_q6,valueq6);
					
					}

				}

				double total_distance_q6=0;
				int total_rides_q6=0;
				GList *aux_q6 = list_rides_q6;

				while(aux_q6!=NULL){
					total_distance_q6 += get_distance_rides(aux_q6->data);
					total_rides_q6++;
					aux_q6 = aux_q6->next;
				}

				if(total_rides_q6 == 0) break;

				else{

					double average_distance_q6 = (double)total_distance_q6/(double)total_rides_q6; 

					fprintf(output_file_pointer,"%.3f\n",average_distance_q6);
				}

				g_list_free(list_rides_q6);

				break;
			
			case 7:

				char *city_q7;
				int city_int_q7=0;
				int n_q7;

				n_q7 = atoi(strsep(&multi_arg, " "));
				n_q7 = atoi(strsep(&multi_arg, " "));

				city_q7 = strsep(&multi_arg, " ");
				city_q7 = strtok(city_q7,"\n");


				if(!strcmp(city_q7,"Lisboa")) city_int_q7 = 0;
				else if(!strcmp(city_q7,"Porto")) city_int_q7 = 1;
				else if(!strcmp(city_q7,"Faro")) city_int_q7 = 2;
				else if(!strcmp(city_q7,"Braga")) city_int_q7 = 3;
				else if(!strcmp(city_q7,"Setúbal")) city_int_q7 = 4;


				GHashTableIter iterq7;
				gpointer keyq7, valueq7;
				g_hash_table_iter_init (&iterq7, DB_rides);
				
				//iter over hashtable rides and get all rides that are in the city
				GList *list_rides_q7 = NULL;
				while(g_hash_table_iter_next(&iterq7, &keyq7, &valueq7)){
				
					if(get_city_rides(valueq7) == city_int_q7 && get_account_status_driver(g_hash_table_lookup(DB_drivers,GINT_TO_POINTER(get_id_driver_rides(valueq7))))==0){
				
						list_rides_q7 = g_list_prepend(list_rides_q7,valueq7);
					
					}

				}

				GHashTable *hash_table_q7 = g_hash_table_new_full(g_direct_hash,g_direct_equal,NULL,(void*)free_driver);
				GList *aux_q7 = list_rides_q7;


				while(aux_q7!=NULL){
					
					if(g_hash_table_contains(hash_table_q7,GINT_TO_POINTER(get_id_driver_rides(aux_q7->data)))){
					
						DATA_DRIVER new = g_hash_table_lookup(hash_table_q7,GINT_TO_POINTER(get_id_driver_rides(aux_q7->data)));
						set_increment_num_viagens_driver(new);
						set_increment_total_avaliacao_driver(new,get_score_driver_rides(aux_q7->data));

					}
					
					else{
						
						DATA_DRIVER original = g_hash_table_lookup(DB_drivers,GINT_TO_POINTER(get_id_driver_rides(aux_q7->data)));
						char *aux_q7_str = NULL;
						aux_q7_str = get_name_driver(original);
						DATA_DRIVER new = ghost_driver(get_id_driver(original),aux_q7_str);
						set_increment_num_viagens_driver(new);
						set_increment_total_avaliacao_driver(new,get_score_driver_rides(aux_q7->data));
						g_hash_table_insert(hash_table_q7,GINT_TO_POINTER(get_id_driver(new)),new);
						free(aux_q7_str);
					}

					aux_q7 = aux_q7->next;
				}

				GHashTableIter iterq7_2;
				gpointer keyq7_2, valueq7_2;
				g_hash_table_iter_init (&iterq7_2, hash_table_q7);

				GList *list_q7 = NULL;
				while(g_hash_table_iter_next(&iterq7_2, &keyq7_2, &valueq7_2)){
				
					list_q7 = g_list_prepend(list_q7,valueq7_2);
					
				}

				list_q7 = g_list_sort(list_q7,compare_drivers_orderby_score_id);

				//print to output file n drivers

				GList *aux_q7_2 = list_q7;
				int i_q7 = 0;

				while(aux_q7_2!=NULL && i_q7<n_q7){

					char *aux_q7_2_str = NULL;
					aux_q7_2_str = get_name_driver(aux_q7_2->data);

					fprintf(output_file_pointer,"%012d;%s;%.3f\n",get_id_driver(aux_q7_2->data),aux_q7_2_str,(double)get_avaliacao_total_driver(aux_q7_2->data)/(double)get_num_viagens_driver(aux_q7_2->data));
					i_q7++;
					aux_q7_2 = aux_q7_2->next;

					free(aux_q7_2_str);
				
				}

				g_list_free(list_q7);
				g_list_free(list_rides_q7);
				g_hash_table_destroy(hash_table_q7);

				break;

			case 8:

				char *gender_q8 = NULL;
				int num_min_year_q8 = 0;
				int gender_int = 0;

				gender_q8 = strsep(&multi_arg," ");
				gender_q8 = strsep(&multi_arg," ");

				if(!strcmp(gender_q8,"M")) gender_int = 0;
				else gender_int = 1;

				num_min_year_q8 = atoi(strsep(&multi_arg," "));

				GHashTableIter iterq8;
				gpointer keyq8, valueq8;
				g_hash_table_iter_init (&iterq8, DB_rides);

				GList *list_q8 = NULL;

				while(g_hash_table_iter_next(&iterq8, &keyq8, &valueq8)){

					char * username = NULL;
					username = get_username_rides(valueq8);
					
					DATA_USER user_to_check = g_hash_table_lookup(DB_users,GINT_TO_POINTER(username));
					DATA_DRIVER driver_to_check = g_hash_table_lookup(DB_drivers,GINT_TO_POINTER(get_id_driver_rides(valueq8)));

					free(username);

					if(get_gender_user(user_to_check) == gender_int && get_gender_driver(driver_to_check) == gender_int && get_account_status_user(user_to_check) == 0 && get_account_status_driver(driver_to_check) == 0 && num_anos_perfil_user(user_to_check) >= num_min_year_q8 && num_anos_perfil_driver(driver_to_check) >= num_min_year_q8){

						list_q8 = g_list_prepend(list_q8,valueq8);

					}

				}

				GList *aux_list_q8 = list_q8;
				GList *processed_list_q8 = NULL;

				while(aux_list_q8){

					AUX_STRUCT_Q8 cicle = malloc(sizeof(struct aux_struct_q8));

					cicle->id_condutor = get_id_driver_rides(aux_list_q8->data);
					cicle->nome_condutor = get_name_driver(g_hash_table_lookup(DB_drivers,GINT_TO_POINTER(cicle->id_condutor)));
					cicle->username_utilizador = get_username_rides(aux_list_q8->data);
					cicle->nome_utilizador = get_name_user(g_hash_table_lookup(DB_users,cicle->username_utilizador));
					get_account_creation_driver(g_hash_table_lookup(DB_drivers,GINT_TO_POINTER(cicle->id_condutor)),&cicle->data_driver);
					get_account_creation_user(g_hash_table_lookup(DB_users,cicle->username_utilizador),&cicle->data_user);
					cicle->id_viagem = get_id_rides(aux_list_q8->data);

					processed_list_q8 = g_list_prepend(processed_list_q8,cicle);

					aux_list_q8 = aux_list_q8->next;

				}

				processed_list_q8 = g_list_sort(processed_list_q8,compare_q8);

				GList *aux_processed_list_q8 = processed_list_q8;

				while(aux_processed_list_q8){

					AUX_STRUCT_Q8 aux_processed_list_q8_data = aux_processed_list_q8->data;

					fprintf(output_file_pointer,"%012d;%s;%s;%s\n",aux_processed_list_q8_data->id_condutor,aux_processed_list_q8_data->nome_condutor,aux_processed_list_q8_data->username_utilizador,aux_processed_list_q8_data->nome_utilizador);

					aux_processed_list_q8 = aux_processed_list_q8->next;

				}

				GList *aux_list2_q8 = processed_list_q8;
				while(aux_list2_q8){

					free_aux_struct_q8(aux_list2_q8->data);
					aux_list2_q8 = aux_list2_q8->next;

				}


				g_list_free(list_q8);
				g_list_free(processed_list_q8);


				break;

			default:
				
				// printf("\n");
				
				// printf("Line Nº %d -> Invalid command!! ❌",i);
				
				// printf("\n");

				break;
			
			}

		i++;
		fclose(output_file_pointer);	
		free(tofree);
	}

	free(line);
	free(output_file_name);

}