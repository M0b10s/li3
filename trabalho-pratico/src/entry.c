#include <entry.h>


// funcion to auto center prints to window terminal

void printCenter(char *s, int size){

	int strsize = strlen(s);
	if (size % 2 != 0) size = size - 1;


	int aux = ((size - strsize)/2)-3;

        if (strsize % 2 == 0) printf("---%*s%s%s%*s---\n",aux,"",s,"",aux,"");
        else printf("---%*s%s%*s ---\n",aux,"",s,aux,"");

}

// Directory checks

int Directory_verification(int argc, char const *argv[],int size){
	//check if arguments are valid
	if(argc != 3){
		printCenter("Invalid number of arguments!! ❌",size);
		return 1;
	}
	//check if input files exist
	if(access(argv[1], F_OK) == -1){
		printCenter("Input DIR : NOT FOUND!! ❌",size);
		return 1;
	}
	//check if commands file exists
	if(access(argv[2], F_OK) == -1){
		printCenter("Commands FILE : NOT FOUND!! ❌",size);
		return 1;
	}
	return 0;
}

int Start_interactive_mode(int size, char *input_dir){

	printf("Input DIR path: ");
	scanf("%s",input_dir);

	//check if input files exist
	if(access(input_dir, F_OK) == -1){
		printCenter("Input DIR : NOT FOUND!! ❌",size);
		return 0;
	}

	return 1;


}

int main(int argc, char const *argv[])
{
	system("rm -f Resultados/*"); //clear results folder
	system("clear");
	//start timer
	clock_t begin = clock();
	int interctive_mode=0;
	char *interctive_path=malloc(1000*sizeof(char));

	//===================================================Visual Optimization=======================================================

	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	const int size = w.ws_col;
	const int size_lines = w.ws_row;

	//=============================================================================================================================
	//===================================================Project Entry=============================================================
	
	printCenter("Program Start ✅",size);

	//=============================================================================================================================
	//===================================================Directory Check===========================================================

	if (argc == 1 && !(interctive_mode = Start_interactive_mode(size,interctive_path))) return 0;

	if (!interctive_mode && Directory_verification(argc,argv,size)) return 1;

	else printCenter("Path's verified!! ✅",size);


	//=============================================================================================================================
	//===================================================File Handling=============================================================

	char *aux_user_path = malloc(strlen("/users.txt")+1000);
	char *aux_drivers_path = malloc(strlen("/drivers.txt")+1000);
	char *aux_rides_path = malloc(strlen("/rides.txt")+1000);

	//inicialize paths
	if (interctive_mode){
		strcpy(aux_user_path,interctive_path);
		strcpy(aux_drivers_path,interctive_path);
		strcpy(aux_rides_path,interctive_path);
	}
	else{
		strcpy(aux_user_path,argv[1]);
		strcpy(aux_drivers_path,argv[1]);
		strcpy(aux_rides_path,argv[1]);
	}


	char *users_file_pointer_string = malloc(strlen("users.csv")+1000);
	strcpy(users_file_pointer_string,aux_user_path);
	strcat(users_file_pointer_string,"/users.csv");

	char *drivers_file_pointer_string = malloc(strlen("drivers.csv")+1000);
	strcpy(drivers_file_pointer_string,aux_drivers_path);
	strcat(drivers_file_pointer_string,"/drivers.csv");

	char *rides_file_pointer_string = malloc(strlen("rides.csv")+1000);
	strcpy(rides_file_pointer_string,aux_rides_path);
	strcat(rides_file_pointer_string,"/rides.csv");


	FILE *users_file_pointer = fopen(users_file_pointer_string,"r");
	FILE *drivers_file_pointer = fopen(drivers_file_pointer_string,"r");
	FILE *rides_file_pointer = fopen(rides_file_pointer_string,"r");
	FILE *commands_file_pointer = fopen(argv[2],"r");


	if (!interctive_mode && (users_file_pointer == NULL || drivers_file_pointer == NULL || rides_file_pointer == NULL || commands_file_pointer == NULL)){
		printCenter("Error opening files!! ❌",size);
		return 1;
	}

	if (interctive_mode && (users_file_pointer == NULL || drivers_file_pointer == NULL || rides_file_pointer == NULL)){
		printf("USER FILE: %s\n",users_file_pointer_string);
		printf("DRIVERS FILE: %s\n",drivers_file_pointer_string);
		printf("RIDES FILE: %s\n",rides_file_pointer_string);

		printCenter("Error opening files!! Interactive ❌",size);
		return 1;
	}

	printCenter("Files opened!! ✅",size);

	
	//=============================================================================================================================
	//===================================================Data Structure=============================================================


	//users
	GHashTable *datab_users = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_user_void);
	//drivers
	GHashTable *datab_drivers = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, free_driver_void);
	//rides
	GHashTable *datab_rides = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, free_rides_void);


	//==============================================================================================================================
	//===================================================Data Loading===============================================================


	fscanf(users_file_pointer, "%*[^\n]\n");	//Descarta primeira linha do ficheiro Users
	fscanf(drivers_file_pointer, "%*[^\n]\n");	//Descarta primeira linha do ficheiro Drivers
	fscanf(rides_file_pointer, "%*[^\n]\n");	//Descarta primeira linha do ficheiro Rides

	load_users_to_DB(datab_users,users_file_pointer);
	load_drivers_to_DB(datab_drivers,drivers_file_pointer);
	load_rides_to_DB(datab_rides,rides_file_pointer, datab_users, datab_drivers);

	printf("Users: %d\n",g_hash_table_size(datab_users));
	printf("Drivers: %d\n",g_hash_table_size(datab_drivers));
	printf("Rides: %d\n\n",g_hash_table_size(datab_rides));


	//=============================================================================================================================
	//===================================================Queries===================================================================

	char menuop[100];

	if (!interctive_mode){
	start_queries(commands_file_pointer,datab_users,datab_drivers,datab_rides,0);
	}
	

	//-----------------------------------------------------------------------------------------------------------------------------

	else {

		int valid_run_index = 1;
		GList *list_inputs = NULL;
		menuop[0] = '\0';

		while(menuop[0] != 'Q' && menuop[0] != 'q'){

			int req_made = -1;

			menu_principal(size,size_lines);
			
			scanf(" %s", menuop);

			if (strlen(menuop) > 1) menuop[0] = 'F'; //control invalid input in main menu

			file result = input_level_1(menuop[0],&valid_run_index,list_inputs,size,&req_made);


			if (result && req_made < 0) {
			
					list_inputs = g_list_prepend(list_inputs,result);

					char *interactive_command = malloc(sizeof(char)*50);
					strcpy(interactive_command,"inputs/interactive_command.txt");

					fill_file(result,interactive_command);

					FILE *interactive_command_file_pointer = fopen(interactive_command,"r");

					start_queries(interactive_command_file_pointer,datab_users,datab_drivers,datab_rides,result->n_request);

					//------------------------------------------------------------------------------------------------------------------------------------

					system("clear");

					char *ficheiro_resultado = malloc(sizeof(char)*50);
					strcpy(ficheiro_resultado,"Resultados/command_interactive_1_output.txt");

					FILE *fp = fopen(ficheiro_resultado,"r");

					print_result_on_screen(size,valid_run_index-1);

					printf("\n");
					wait_for_enter_v2();
			
				free(interactive_command);
				fclose(interactive_command_file_pointer);
				free(ficheiro_resultado);
				fclose(fp);
			
			}

			//------------------------------------------------------------------------------------------------------------------------------------
			
			else if(toupper(menuop[0]) == 'C'){


				if(valid_run_index > req_made){
					print_result_on_screen(size,req_made);
					printf("\n");
					wait_for_enter_v2();
				}
				
				else {
					printCenter("Invalid request number!!",size);
					printf("\n");
					wait_for_enter();
				}

			}

		
		}

		gpointer list_original = list_inputs;

		while(list_inputs){
			free_file(list_inputs->data);
			list_inputs = g_list_next(list_inputs);
		}

		g_list_free(list_original);

	}

	//-----------------------------------------DEBUG-------------------------------------------------------------------------------

	// DATA_DRIVER driver = g_hash_table_lookup(datab_drivers,(gpointer)3308); //check values for x driver WORKING RIGHT
	// print_driver(driver);
	// DATA_USER user = NULL;
	// user = g_hash_table_lookup(datab_users,"LeTavares103"); //check values for x user
	// if(user)print_user(user);
	// else printf("NOT FOUND!!!\n");
	// DATA_RIDES ride = g_hash_table_lookup(datab_rides,(gpointer)1); //check values for x ride
	// print_rides(ride);


	//=============================================================================================================================
	//===================================================Data Freeing==============================================================

	g_hash_table_destroy(datab_users);
	g_hash_table_destroy(datab_drivers);
	g_hash_table_destroy(datab_rides);

	free(users_file_pointer_string);
	free(drivers_file_pointer_string);
	free(rides_file_pointer_string);
	free(aux_user_path);
	free(aux_drivers_path);
	free(aux_rides_path);
	free(interctive_path);


	//=============================================================================================================================
	//===================================================File Closure==============================================================

 	fclose(users_file_pointer);
 	fclose(drivers_file_pointer);
 	fclose(rides_file_pointer);
 	if(!interctive_mode) fclose(commands_file_pointer);

 	printCenter("Files closed!! ✅",size);

	//=============================================================================================================================
	//===================================================Project Exit==============================================================

 	printCenter("Program End ✅",size);
 	clock_t end = clock();
 	printf("Time elapsed: %f\n",(double)(end - begin) / CLOCKS_PER_SEC);

 	//=============================================================================================================================

	return 0;

}
