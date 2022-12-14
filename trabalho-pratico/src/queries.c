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

//compare_q9 order by distance descending if equal most recent first if still equal order by id of ride descending
gint compare_q9(gconstpointer a, gconstpointer b){

	DATA_RIDES elem1 = (DATA_RIDES) a;
	DATA_RIDES elem2 = (DATA_RIDES) b;

	if(get_distance_rides(elem1) > get_distance_rides(elem2)){
		return 1;
	}
	else if(get_distance_rides(elem1) < get_distance_rides(elem2)){
		return -1;
	}
	else{


		struct tm data1_q9;
		get_date_rides(elem1,&data1_q9);
		struct tm data2_q9;
		get_date_rides(elem2,&data2_q9);

		if(compare_tmdates(data1_q9,data2_q9) == 1){
			return 1;
		}

		else if(compare_tmdates(data1_q9,data2_q9) == -1){

			return -1;
		}
		else if(get_id_rides(elem1) > get_id_rides(elem2)){
			
				return 1;
			
			}
			
			else if(get_id_rides(elem1) < get_id_rides(elem2)){
			
				return -1;
			
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

gint compare_drivers_orderby_score(gconstpointer a, gconstpointer b){

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
	
	else return 0;

}

gint compare_users_orderby_distance(gconstpointer a, gconstpointer b){

	DATA_USER user1 = (DATA_USER) a;
	DATA_USER user2 = (DATA_USER) b;

	if(get_distancia_viajada_user(user1) > get_distancia_viajada_user(user2))
	
		return -1;
	
	else if(get_distancia_viajada_user(user1) < get_distancia_viajada_user(user2))
	
		return 1;
	
	else return 0;

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


char *int_to_city(int x){

	switch(x){

		case 0: return "Lisboa";
		case 1: return "Porto";
		case 2: return "Faro";
		case 3: return "Braga";
		case 4: return "Setúbal";
		default: return "NULL";

	}


}

void swap_elements_glist(GList* list, int i, int j){

	GList* elem1 = g_list_nth(list,i);
	GList* elem2 = g_list_nth(list,j);

	gpointer aux = elem1->data;
	elem1->data = elem2->data;
	elem2->data = aux;

}


void start_queries(FILE *commands_file_pointer, GHashTable *DB_users, GHashTable *DB_drivers, GHashTable *DB_rides,int interctive_mode_index){

	char *line=NULL;
	char *output_file_name=malloc(1000*sizeof(char));
	int i = 1;
	size_t len = 0;
	ssize_t read;

	while ((read = getline(&line, &len, commands_file_pointer)) != -1){

		// printf("Retrieved line nº %d of length %zu ===> ",i, read);
		// printf("%s", line);

		//process command
		if(interctive_mode_index) sprintf(output_file_name,"%s%d%s","Resultados/command_interactive_",interctive_mode_index,"_output.txt");
		else sprintf(output_file_name,"%s%d%s","Resultados/command",i,"_output.txt");
		
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

				guint n_q2 = atoi(multi_arg);

				GList *list_q2 = g_hash_table_get_values(DB_drivers);
				list_q2 = g_list_sort(list_q2,compare_drivers_orderby_score);

				//iter over list and remove drivers with account status = 1
				GList *iter_q2 = list_q2;
				while(iter_q2 != NULL){

					if(get_account_status_driver(iter_q2->data)){
						GList *aux = iter_q2;
						iter_q2 = iter_q2->next;
						list_q2 = g_list_delete_link(list_q2,aux);
					}

					else iter_q2 = iter_q2->next;

				}


				gpointer to_free_q2=NULL;

				if(n_q2<g_list_length(list_q2)) to_free_q2 = g_list_nth(list_q2,n_q2);

				if(n_q2<g_list_length(list_q2)) g_list_nth(list_q2,n_q2-1)->next = NULL;

				
				GList *swaper = list_q2;
				int counter_q2=0;
				while(swaper->next != NULL){

					DATA_DRIVER driver1 = swaper->data;
					DATA_DRIVER driver2 = swaper->next->data;

					struct tm date_user_1;
					struct tm date_user_2;

					get_data_ultima_ride_driver(driver1,&date_user_1);
					get_data_ultima_ride_driver(driver2,&date_user_2);

					int non_div_zero_q2_driver1=0;
					int non_div_zero_q2_driver2=0;

					if(get_num_viagens_driver(driver1) == 0) non_div_zero_q2_driver1=1;
					if(get_num_viagens_driver(driver2) == 0) non_div_zero_q2_driver2=1;

					if((double)get_avaliacao_total_driver(driver1)/(double)get_num_viagens_driver(driver1)+non_div_zero_q2_driver1 == (double)get_avaliacao_total_driver(driver2)/(double)get_num_viagens_driver(driver2)+non_div_zero_q2_driver2 && compare_tmdates(date_user_1,date_user_2) == -1){

						swap_elements_glist(list_q2,counter_q2,counter_q2+1);

					}

					counter_q2++;
					swaper = swaper->next;

				}


				GList *aux_q2 = list_q2;
				while(aux_q2 != NULL){

					DATA_DRIVER driver = aux_q2->data;
					char *name_driver = get_name_driver(driver);
					int non_div_zero=0;
					if(get_num_viagens_driver(driver) == 0) non_div_zero=1;
					fprintf(output_file_pointer,"%012d;%s;%.3f\n",get_id_driver(driver),name_driver,((double)get_avaliacao_total_driver(driver)/(double)get_num_viagens_driver(driver)+(double)non_div_zero));
					free(name_driver);
					aux_q2 = aux_q2->next;

				}

				g_list_free(list_q2);
				if(to_free_q2)g_list_free(to_free_q2);

				break;

			case 3:

				strsep(&multi_arg, "\n ");
				strtok(multi_arg, "\n");

				guint n_q3 = atoi(multi_arg);

				GList *list_q3 = g_hash_table_get_values(DB_users);
				list_q3 = g_list_sort(list_q3,compare_users_orderby_distance);

				//iter over list and remove users with account status = 1
				GList *iter_q3 = list_q3;
				while(iter_q3 != NULL){

					if(get_account_status_user(iter_q3->data)){
						GList *aux = iter_q3;
						iter_q3 = iter_q3->next;
						list_q3 = g_list_delete_link(list_q3,aux);
					}

					else iter_q3 = iter_q3->next;

				}

				gpointer to_free_q3=NULL;

				if(n_q3<g_list_length(list_q3)) to_free_q3 = g_list_nth(list_q3,n_q3);

				if(n_q3<g_list_length(list_q3)) g_list_nth(list_q3,n_q3-1)->next = NULL;

				GList *swaper_q3 = list_q3;
				int counter_q3=0;
				while(swaper_q3->next != NULL){

					DATA_USER user1 = swaper_q3->data;
					DATA_USER user2 = swaper_q3->next->data;

					struct tm date_user_1;
					struct tm date_user_2;

					get_data_ultima_ride_user(user1,&date_user_1);
					get_data_ultima_ride_user(user2,&date_user_2);

					// if the distance is the same, iter over the rides and check the date of the last ride

					if(get_distancia_viajada_user(user1) == get_distancia_viajada_user(user2) && compare_tmdates(date_user_1,date_user_2) == -1){

						swap_elements_glist(list_q3,counter_q3,counter_q3+1);

					}

					counter_q3++;
					swaper_q3 = swaper_q3->next;

				}



				GList *aux_q3 = list_q3;
				while(aux_q3 != NULL){

					DATA_USER user_q2 = aux_q3->data;
					char *name_user_string_q2 = get_name_user(user_q2);
					char *username_string_q2 = get_username(user_q2);

					fprintf(output_file_pointer,"%s;%s;%d\n",username_string_q2,name_user_string_q2,get_distancia_viajada_user(user_q2));
					
					free(name_user_string_q2);
					free(username_string_q2);
					aux_q3 = aux_q3->next;

				}

				g_list_free(list_q3);
				if(to_free_q3)g_list_free(to_free_q3);

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

			case 9:

				struct tm data_inicio_q9;
				struct tm data_fim_q9;
				char *dates_q9;

				dates_q9 = strsep(&multi_arg, " ");
				dates_q9 = strsep(&multi_arg, " ");

				strptime(dates_q9,"%d/%m/%Y",&data_inicio_q9);
				data_inicio_q9.tm_mon++;

				// printf("DATA INICIO : %d/%d/%d",data_inicio_q9.tm_mday,data_inicio_q9.tm_mon,data_inicio_q9.tm_year);


				dates_q9 = strsep(&multi_arg, " ");

				strptime(dates_q9,"%d/%m/%Y",&data_fim_q9);
				data_fim_q9.tm_mon++;

				// printf("	DATA FIM : %d/%d/%d\n",data_fim_q9.tm_mday,data_fim_q9.tm_mon,data_fim_q9.tm_year);


				GHashTableIter iterq9;
				gpointer keyq9, valueq9;
				g_hash_table_iter_init(&iterq9, DB_rides);

				GList *list_q9=NULL;

				struct tm data_viagem_q9;

				while (g_hash_table_iter_next (&iterq9, &keyq9, &valueq9)) {

					get_date_rides(valueq9,&data_viagem_q9);

					if(compare_tmdates(data_viagem_q9,data_inicio_q9) >= 0 && compare_tmdates(data_viagem_q9,data_fim_q9) <= 0 && get_tip_rides(valueq9) > 0){

						list_q9 = g_list_prepend(list_q9,valueq9);

					}

				}

				list_q9 = g_list_sort(list_q9,compare_q9);
				list_q9 = g_list_reverse(list_q9);
				
				GList *aux_list_q9 = list_q9;
				struct tm aux_data_q9;

				while(aux_list_q9){

					get_date_rides(aux_list_q9->data,&aux_data_q9);

					fprintf(output_file_pointer,"%012d;%02d/%02d/%d;%d;%s;%.3f\n",get_id_rides(aux_list_q9->data),aux_data_q9.tm_mday,aux_data_q9.tm_mon,aux_data_q9.tm_year+1900,get_distance_rides(aux_list_q9->data),int_to_city(get_city_rides(aux_list_q9->data)),get_tip_rides(aux_list_q9->data));
					aux_list_q9 = aux_list_q9->next;

				}

				g_list_free(list_q9);

				break;

			default:
				
				printf("\n");
				
				printf("Line Nº %d -> Invalid command!! ❌",i);
				
				printf("\n");

				break;
			
			}

		i++;
		fclose(output_file_pointer);	
		free(tofree);
	}

	free(line);
	free(output_file_name);

}