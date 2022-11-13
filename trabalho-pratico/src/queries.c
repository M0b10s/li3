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

#include "users.h"
#include "drivers.h"
#include "rides.h"
#include "queries.h"


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

				fprintf(output_file_pointer,"%s\n",token);

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