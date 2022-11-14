#include <drivers.h>

enum gender{M,F};
enum car_calss{basic,green,premium};
enum city{Lisboa,Porto,Faro,Braga,Setubal};
enum account_status{active,inactive};


typedef struct data_driver{

	int id;
	char *name;
	struct tm birth_day;
	enum gender gender;
	enum car_calss car_class;
	char *license_plate;
	enum city city;
	struct tm account_creation;
	enum account_status account_status;
	int age;
	int num_viagens;
	int avaliacao_total;
	double total_auferido_driver; 



}*DATA_DRIVER;


//===============================================Set's=====================================================

void set_increment_num_viagens_driver(DATA_DRIVER driver){
	driver->num_viagens = driver->num_viagens + 1;
}

void set_increment_total_avaliacao_driver(DATA_DRIVER driver,int avaliacao){
	driver->avaliacao_total += avaliacao;
}

void set_increment_total_auferido_driver(DATA_DRIVER driver,double gasto){
	driver->total_auferido_driver += gasto;
}


//===============================================Get's=====================================================

int get_id_driver(DATA_DRIVER driver){

	return driver->id;

}

char *get_name_driver(DATA_DRIVER driver){

	return strdup(driver->name);

}

void get_birth_day(DATA_DRIVER driver, struct tm *dest){

	dest->tm_mday = driver->birth_day.tm_mday;
	dest->tm_mon = driver->birth_day.tm_mon;
	dest->tm_year = driver->birth_day.tm_year;

}

int get_car_class(DATA_DRIVER driver){

	return driver->car_class;

}

char *get_license_plate(DATA_DRIVER driver){

	return strdup(driver->license_plate);

}

int get_city(DATA_DRIVER driver){

	return driver->city;

}

void get_account_creation_driver(DATA_DRIVER driver, struct tm *dest){

	dest->tm_mday = driver->account_creation.tm_mday;
	dest->tm_mon = driver->account_creation.tm_mon;
	dest->tm_year = driver->account_creation.tm_year;

}

int get_account_status_driver(DATA_DRIVER driver){

	return driver->account_status;

}

int get_age_driver(DATA_DRIVER driver){

	return driver->age;

}

//===============================================END Get's=====================================================


void free_driver(DATA_DRIVER driver){

	free(driver->name);
	free(driver->license_plate);
	free(driver);

}

void print_driver(DATA_DRIVER driver){

	printf("\n");
	printf("\n");
	printf("id : %d\n",driver->id);
	printf("name : %s\n",driver->name);
	printf("birth_day : %d/%d/%d\n",driver->birth_day.tm_mday,driver->birth_day.tm_mon,driver->birth_day.tm_year+1900);
	printf("gender: %d\n",driver->gender);
	printf("car_class : %d\n",driver->car_class);
	printf("license_plate : %s\n",driver->license_plate);
	printf("city : %d\n",driver->city);
	printf("account_creation : %d/%d/%d\n",driver->account_creation.tm_mday,driver->account_creation.tm_mon,driver->account_creation.tm_year+1900);
	printf("account_status : %d\n",driver->account_status);
	printf("age : %d\n",driver->age);
	printf("num_viagens : %d\n",driver->num_viagens);
	printf("avaliacao_total : %d\n",driver->avaliacao_total);
	printf("total_auferido_driver : %f\n",driver->total_auferido_driver);
	printf("\n");

}


DATA_DRIVER clone_driver(DATA_DRIVER driver){
		
	DATA_DRIVER clone = malloc(sizeof(struct data_driver));

	clone->id = driver->id;
	clone->name = strdup(driver->name);
	clone->birth_day = driver->birth_day;
	clone->gender = driver->gender;
	clone->car_class = driver->car_class;
	clone->license_plate = strdup(driver->license_plate);
	clone->city = driver->city;
	clone->account_creation = driver->account_creation;
	clone->account_status = driver->account_status;
	clone->age = driver->age;
	clone->num_viagens = driver->num_viagens;
	clone->avaliacao_total = driver->avaliacao_total;

	return clone;
}

DATA_DRIVER create_driver(char *drivers_line){

	DATA_DRIVER driver = malloc(sizeof(struct data_driver));

	driver->name = NULL;
	driver->license_plate = NULL;

	char *token;
	char *rest = drivers_line;
	
	int i = 0,flag=0;

	
		while ((token = strtok_r(rest, ";\n", &rest))){
        

        switch(i){

        	case 0:

        		if (!atoi(token)) flag=1;
					driver->id = atoi(token);
        		
        		break;

        	case 1:

        		driver->name = strdup(token);
        		
        		break;

        	case 2:

        		if (strptime(token,"%d/%m/%Y",&driver->birth_day) == NULL)
        		flag = 1;

				break;

			case 3:

				if (!strcmp(token,"M"))
					driver->gender = M;
				else if (!strcmp(token,"F"))
					driver->gender = F;
				else
					flag = 1;
				break;

				break;

			case 4:

				if (!strcmp(token,"basic"))
					driver->car_class = basic;
				else if (!strcmp(token,"green"))
					driver->car_class = green;
				else if (!strcmp(token,"premium"))
					driver->car_class = premium;
				else 
					flag = 1;

				break;

			case 5:

				driver->license_plate = strdup(token);

				break;

			case 6:

				if (!strcmp(token,"Lisboa"))
					driver->city = Lisboa;
				else if (!strcmp(token,"Porto"))
					driver->city = Porto;
				else if (!strcmp(token,"Faro"))
					driver->city = Faro;
				else if (!strcmp(token,"Braga"))
					driver->city = Braga;
				else if (!strcmp(token,"Setúbal"))
					driver->city = Setubal;
				else
					flag = 1;
				break;

			case 7:

				if (strptime(token,"%d/%m/%Y",&driver->account_creation) == NULL)
				flag = 1;

				break;

			case 8:

				if (!strcmp(token,"active"))
					driver->account_status = active;
				else if (!strcmp(token,"inactive"))
					driver->account_status = inactive;
				else
					flag = 1;

			
				break;
			

			default:
			
				flag = 1;
			
			break;

        }

		i++;

		}


			if(flag == 1 || i<9){
				printf("ERROR IN DRIVER DATA!!!\n");
				driver = NULL;
			}
        	
			// AGE CALCULATION
			time_t t = time(NULL);
			struct tm tm = *localtime(&t);
			int age = tm.tm_year - driver->birth_day.tm_year;
			if (tm.tm_mon < driver->birth_day.tm_mon || (tm.tm_mon == driver->birth_day.tm_mon && tm.tm_mday < driver->birth_day.tm_mday))
				age--;
			driver->age = age;

			driver->num_viagens = 0;
			driver->avaliacao_total = 0;


		free(token);


		return driver;

}

void load_drivers_to_DB(GHashTable *DB_drivers,FILE *drivers_file_pointer){
	
	//load datab_drivers to ghash table
	char *driver_line = NULL;
	size_t driver_line_size = 0;
	ssize_t driver_line_size_read;

	DATA_DRIVER driver = NULL;
	gint id;

	printf("\n==================================> Loading drivers to DB...\n\n");

	while((driver_line_size_read = getline(&driver_line,&driver_line_size,drivers_file_pointer)) != -1){

		driver = create_driver(driver_line);

		if(driver){
			id = get_id_driver(driver);
			g_hash_table_insert(DB_drivers,GINT_TO_POINTER(id),driver);
		}
 
	}
	
	printf("\n==================================> Drivers loaded to DB ✅\n\n\n");

	free(driver_line);

}