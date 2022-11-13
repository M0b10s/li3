#include<users.h>


enum gender{M,F};
enum pay_method{cash,credit_card,debit_card};
enum account_status{active,inactive};

typedef struct data_user{

	char *username;
	char *name;
	enum gender gender;
	struct tm birth_date;
	struct tm account_creation;
	enum pay_method pay_method;
	enum account_status account_status;
	int idade;
	int num_viagens;
	double distancia_viajada;
	double total_avaliacao;
	double total_gasto;
	
}*DATA_USER;

//===============================================Set's=====================================================

void set_increment_num_viagens(DATA_USER user){
	
	if(user) user->num_viagens = user->num_viagens + 1;

}

void set_distancia_viajada(DATA_USER user,double distancia){
	user->distancia_viajada += distancia;
}

void set_total_avaliacao(DATA_USER user,double avaliacao){
	user->total_avaliacao += avaliacao;
}

void set_total_gasto(DATA_USER user,double gasto){
	user->total_gasto += gasto;
}

//===============================================Get's=====================================================

char *get_username(DATA_USER user){

	return strdup(user->username);

}

char *get_name_user(DATA_USER user){

	return strdup(user->name);

}

int get_gender_user(DATA_USER user){

	return user->gender;

}

void get_birth_date_user(DATA_USER user,struct tm *dest){

	dest->tm_mday = user->birth_date.tm_mday;
	dest->tm_mon = user->birth_date.tm_mon;
	dest->tm_year = user->birth_date.tm_year;

}

void get_account_creation_user(DATA_USER user, struct tm *dest){

	dest->tm_mday = user->account_creation.tm_mday;
	dest->tm_mon = user->account_creation.tm_mon;
	dest->tm_year = user->account_creation.tm_year;

}

int get_pay_method_user(DATA_USER user){

	return user->pay_method;

}

int get_account_status_user(DATA_USER user){

	return user->account_status;

}

int get_idade(DATA_USER user){

	return user->idade;

}

int get_num_viagens(DATA_USER user){

	return user->num_viagens;

}

double get_distancia_viajada(DATA_USER user){

	return user->distancia_viajada;

}

double get_total_avaliacao(DATA_USER user){

	return user->total_avaliacao;

}

double get_total_gasto(DATA_USER user){

	return user->total_gasto;

}

//===============================================END Get's=====================================================

void free_user(DATA_USER user){

	free(user->username);
	free(user->name);
	free(user);

}

void print_user(DATA_USER user){

	if(user){
			printf("\n");
			printf("username: %s\n",user->username);
			printf("name: %s\n",user->name);
        	printf("gender: %d\n",get_gender_user(user));
        	printf("birth_date: %d/%d/%d\n",user->birth_date.tm_mday,user->birth_date.tm_mon,user->birth_date.tm_year+1900);
        	printf("account_creation: %d/%d/%d\n",user->account_creation.tm_mday,user->account_creation.tm_mon,user->account_creation.tm_year+1900);
        	printf("pay_method: %d\n",get_pay_method_user(user));
        	printf("account_status: %d\n",get_account_status_user(user));
        	printf("idade: %d\n",user->idade);
        	printf("num_viagens: %d\n",user->num_viagens);
        	printf("distancia_viajada: %lf\n",user->distancia_viajada);
        	printf("total_avaliacao: %lf\n",user->total_avaliacao);
        	printf("total_gasto: %lf\n",user->total_gasto);
       		printf("\n");
    }

}

DATA_USER clone_user(DATA_USER user){
	
	if(user == NULL) return NULL;

	DATA_USER clone = malloc(sizeof(struct data_user));
	clone->username = strdup(user->username);
	clone->name = strdup(user->name);
	clone->gender = user->gender;
	clone->birth_date.tm_mday = user->birth_date.tm_mday;
	clone->birth_date.tm_mon = user->birth_date.tm_mon;
	clone->birth_date.tm_year = user->birth_date.tm_year;
	clone->account_creation.tm_mday = user->account_creation.tm_mday;
	clone->account_creation.tm_mon = user->account_creation.tm_mon;
	clone->account_creation.tm_year = user->account_creation.tm_year;
	clone->pay_method = user->pay_method;
	clone->account_status = user->account_status;
	clone->idade = user->idade;
	clone->num_viagens = user->num_viagens;
	clone->distancia_viajada = user->distancia_viajada;
	clone->total_avaliacao = user->total_avaliacao;
	clone->total_gasto = user->total_gasto;

	return clone;

}


DATA_USER create_user(char *users_line){

	DATA_USER user = malloc(sizeof(struct data_user));

	user->username = NULL;
	user->name = NULL;

	char *token;
	char *rest = users_line;
	
	int i = 0,flag=0;

	
		while ((token = strtok_r(rest, ";\n", &rest))){
        

        switch(i){

        	case 0:

				user->username = strdup(token);
				
				break;
			
			case 1:
				
				user->name = strdup(token);

				break;
			
			case 2:

				if (!strcmp(token,"M"))
					user->gender = M;
				else if (!strcmp(token,"F"))
					user->gender = F;
				else
					flag = 1;
				break;

			case 3:

				if (strptime(token,"%d/%m/%Y",&user->birth_date) == NULL)
					flag = 1;
				break;
			
			case 4:
			
				if (strptime(token,"%d/%m/%Y",&user->account_creation) == NULL)
					flag = 1;
				break;
			
			case 5:
			
				if (!strcmp(token,"cash"))
					user->pay_method = cash;
				else if (!strcmp(token,"credit_card"))
					user->pay_method = credit_card;
				else if (!strcmp(token,"debit_card"))
					user->pay_method = debit_card;
				else
					flag = 1;
				
				break;
			
			case 6:
			
				if (!strcmp(token,"active"))
					user->account_status = active;
				else if (!strcmp(token,"inactive"))
					user->account_status = inactive;
				else
					flag = 1;
				break;
			
			
			default:
			
				flag = 1;
			
			break;
		
		}

		i++;

		}

			
			if(flag == 1 || i<6){
				printf("ERROR IN USER DATA!!!\n");
				user = NULL;
			}
			
			// AGE CALCULATION

			time_t t = time(NULL);
			struct tm tm = *localtime(&t);
			int idade = tm.tm_year - user->birth_date.tm_year;
			if (tm.tm_mon < user->birth_date.tm_mon || (tm.tm_mon == user->birth_date.tm_mon && tm.tm_mday < user->birth_date.tm_mday))
				idade--;
			user->idade = idade;


		free(token);

		return user;

}

void load_users_to_DB(GHashTable *DB_users,FILE *users_file_pointer){

	
	//load users to user structure and add them to ghash table
	
	char *users_line = NULL;
	size_t users_line_size = 0;
	ssize_t users_line_size_read;


 	DATA_USER user = NULL;

 	printf("\n==================================> Loading users to DB...\n\n");

	while((users_line_size_read = getline(&users_line,&users_line_size,users_file_pointer)) != -1){


		user = create_user(users_line);

		if(user){
			char *aux=NULL;
			aux = get_username(user);
			g_hash_table_insert(DB_users,g_strdup(aux),user);
			free(aux);
		}
 
	}
	
	printf("\n==================================> Users loaded to DB ✅\n\n\n");


	free(users_line);



}