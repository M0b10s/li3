// main gets two arguments:
// arguement 1: path to input files
// argument 2: path to commands file (query file)
// main parses the input files and stores dataset's in glib data structures
// the result of each querie in commands file is stored in a directory named "Resultados" on root file of project
// each result is stored in a file named "commandX_output.txt" where X is the number of the line of the command in the commands file


//input files:
/*
	users.csv -> contains all users
	drivers.csv -> contains all drivers
	rides.csv -> contains all rides

	details:

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

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h> 
#include <unistd.h> 
#include <sys/ioctl.h>
#include <users.h>
#include <drivers.h>
#include <rides.h>


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



int main(int argc, char const *argv[])
{

	//start timer
	clock_t begin = clock();

	//===================================================Visual Optimization=======================================================

	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	const int size = w.ws_col;

	//=============================================================================================================================
	//===================================================Project Entry=============================================================
	
	printCenter("Program Start ✅",size);

	//=============================================================================================================================
	//===================================================Directory Check===========================================================


	if (Directory_verification(argc,argv,size)) return 1;
	else printCenter("Path's verified!! ✅",size);

	//=============================================================================================================================
	//===================================================File Handling=============================================================

	char *users_file_pointer_string = malloc(strlen(argv[1])+strlen("users.csv")+1000);
	strcpy(users_file_pointer_string,argv[1]);
	strcat(users_file_pointer_string,"/users.csv");

	char *drivers_file_pointer_string = malloc(strlen(argv[1])+strlen("drivers.csv")+1000);
	strcpy(drivers_file_pointer_string,argv[1]);
	strcat(drivers_file_pointer_string,"/drivers.csv");

	char *rides_file_pointer_string = malloc(strlen(argv[1])+strlen("rides.csv")+1000);
	strcpy(rides_file_pointer_string,argv[1]);
	strcat(rides_file_pointer_string,"/rides.csv");


	FILE *users_file_pointer = fopen(users_file_pointer_string,"r");
	FILE *drivers_file_pointer = fopen(drivers_file_pointer_string,"r");
	FILE *rides_file_pointer = fopen(rides_file_pointer_string,"r");
	FILE *commands_file_pointer = fopen(argv[2],"r");

	if (users_file_pointer == NULL || drivers_file_pointer == NULL || rides_file_pointer == NULL || commands_file_pointer == NULL){
		printCenter("Error opening files!! ❌",size);
		return 1;
	}

	printCenter("Files opened!! ✅",size);

	
	//=============================================================================================================================
	//===================================================Data Structure=============================================================

	//users
	GHashTable *datab_users = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, (void*)free_user);
	//drivers
	GHashTable *datab_drivers = g_hash_table_new_full(g_int_hash, g_int_equal, NULL, (void*)free_driver);
	//rides
	GHashTable *datab_rides = g_hash_table_new_full(g_int_hash, g_int_equal, NULL, (void*)free_rides);




	//=============================================================================================================================
	//===================================================Data Loading===============================================================

	fscanf(users_file_pointer, "%*[^\n]\n");	//Descarta primeira linha do ficheiro Users
	fscanf(drivers_file_pointer, "%*[^\n]\n");	//Descarta primeira linha do ficheiro Drivers
	fscanf(rides_file_pointer, "%*[^\n]\n");	//Descarta primeira linha do ficheiro Rides
	

	load_users_to_DB(datab_users,users_file_pointer);
	load_drivers_to_DB(datab_drivers,drivers_file_pointer);
	load_rides_to_DB(datab_rides,rides_file_pointer);


	// printf("User Table size:  %d\n",g_hash_table_size(datab_users));
	// printf("Drivers Table size: %d\n",g_hash_table_size(datab_drivers));
	// printf("Rides Table size: %d\n",g_hash_table_size(datab_rides));


	//=============================================================================================================================
	//===================================================Queries===================================================================

	//read commands file line by line

	char *line=malloc(1000);
	char *command=malloc(1000);
	int i = 1;
	size_t len = 0;
	ssize_t read;

	while ((read = getline(&line, &len, commands_file_pointer)) != -1) {
		printf("Retrieved line nº %d of length %zu ===> ",i, read);
		printf("%s", line);

		//process command
		sprintf(command,"%s%d%s","Resultados/command",i,"_output.txt");
		
		FILE *output_file_pointer = fopen(command,"w+");

		char *token = strtok(line, " ");
		int commando = atoi(token);

		token = strtok(NULL, " ");
		switch (commando){
			case 1:

				fprintf(output_file_pointer,"%s\n",token);

				break;
			
			default:
				char *aux = malloc(1000);
				
				printf("\n");
				
				sprintf(aux,"Line Nº %d -> Invalid command!! ❌",i);
				printCenter(aux,size);
				
				printf("\n");
				
				
				free(aux);

				break;
	
		}
			i++;
			fclose(output_file_pointer);
	
	}

	free(line);
	free(command);


// 	//parse commands file
// 	parse_commands(argv[2]);
	
// 	//run commands
// 	run_commands();

	

	//=============================================================================================================================
	//===================================================Data Freeing==============================================================

	g_hash_table_destroy(datab_users);
	g_hash_table_destroy(datab_drivers);
	g_hash_table_destroy(datab_rides);

	// g_hash_table_for_each(datab_users,print_user,NULL);
	// g_hash_table_for_each(datab_drivers,print_driver,NULL);
	// g_hash_table_for_each(datab_rides,print_ride,NULL);


	free(users_file_pointer_string);
	free(drivers_file_pointer_string);
	free(rides_file_pointer_string);


	//=============================================================================================================================
	//===================================================File Closure==============================================================

 	fclose(users_file_pointer);
 	fclose(drivers_file_pointer);
 	fclose(rides_file_pointer);
 	fclose(commands_file_pointer);
 	printCenter("Files closed!! ✅",size);


	//=============================================================================================================================
	//===================================================Project Exit==============================================================

 	printCenter("Program End ✅",size);
 	clock_t end = clock();
 	printf("Time elapsed: %f\n",(double)(end - begin) / CLOCKS_PER_SEC);

// 	//=============================================================================================================================

	return 0;
}
