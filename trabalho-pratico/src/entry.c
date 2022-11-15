#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h> 
#include <unistd.h> 
#include <sys/ioctl.h>
#include <users.h>
#include <drivers.h>
#include <rides.h>
#include <queries.h>


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
	system("clear");
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
	GHashTable *datab_users = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (void*)free_user);
	//drivers
	GHashTable *datab_drivers = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, (void*)free_driver);
	//rides
	GHashTable *datab_rides = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, (void*)free_rides);


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
	printf("Rides: %d\n",g_hash_table_size(datab_rides));


	//=============================================================================================================================
	//===================================================Queries===================================================================


	start_queries(commands_file_pointer,datab_users,datab_drivers,datab_rides);
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

 	//=============================================================================================================================

	return 0;

}
