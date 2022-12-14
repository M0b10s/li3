#include <menu.h>

#define NUM_ELEM 10

void repeat (char c , int count )
{
     for (int i = 0; i<count;i++){
        printf("%c", c);
    }
}

void free_file(file input){
	if(input->arg1) free(input->arg1);
	if(input->arg2) free(input->arg2);
	if(input->arg3) free(input->arg3);
	free(input);
}

void fill_file (file input,char *name){

	//create a file
	FILE *fp = fopen(name, "wr+");
	if (fp == NULL){
		printf("Error opening file to inset command data!\n");
		exit(1);
	}

	//write to file

	if(input){

		fprintf(fp,"%d",input->n_querie);

		if(input->arg1){
			fprintf(fp, " %s", input->arg1);
		}
		if(input->arg2){
			fprintf(fp, " %s", input->arg2);
		}
		if(input->arg3){
			fprintf(fp, " %s", input->arg3);
		}

	}

	fclose(fp);

}

void get_arg_id(char *arg1){

	printf("\nID em busca : ");
	scanf(" %s",arg1);

}

void get_arg_n(char *arg1){

	printf("\nN : ");
	scanf(" %s",arg1);

}

void get_arg_city(char *arg1){

	printf("\nCity : ");
	scanf(" %s",arg1);

}

void get_arg_date_inic(char *arg1){

	printf("\nDate 1 (format dd/mm/yyyy): ");
	scanf(" %s",arg1);

}

void get_arg_date_end(char *arg2){

	printf("\nDate 2 (format dd/mm/yyyy): ");
	scanf(" %s",arg2);

}

void get_arg_gender(char *arg1){

	printf("\nGender :");
	scanf(" %s",arg1);

}

void cancel_query(){

	printf("Query Cancelada\n");
	printf("Press Return to continue\n");
	getchar();
	while(getchar() != '\n');

}

void wait_for_enter(){

	printf("Press Return to continue\n");
	getchar();
	while(getchar() != '\n');

}

void wait_for_enter_v2(){

	printf("Press Return to continue\n");
	while(getchar() != '\n');

}

int count_number_lines_in_file(FILE *file){

	FILE *aux = file;
	int count = 0;
	char *line=NULL;
	size_t len = 0;

	while(getline(&line,&len, aux) != -1) count++;

	return count;	
}

void file_stand_print(file input){
	
	switch(input->n_querie){
	
		case 1:
	
			printf("1 %s\n",input->arg1);
	
			break;
	
		case 2:

			printf("2 %s\n",input->arg1);

			break;

		case 3:

			printf("3 %s\n",input->arg1);

			break;

		case 4:

			printf("4 %s\n",input->arg1);

			break;

		case 5:

			printf("5 %s %s\n",input->arg1,input->arg2);

			break;

		case 6:

			printf("6 %s %s %s\n",input->arg1,input->arg2,input->arg3);

			break;

		case 7:

			printf("7 %s %s\n",input->arg1,input->arg2);

			break;

		case 8:

			printf("8 %s %s\n",input->arg1,input->arg2);

			break;

		case 9:

			printf("9 %s %s\n",input->arg1,input->arg2);

			break;

		default:

			printf("Invalid querie number\n");

			break;
	}

}

void print_file(file input){

	printf("numeber of request : %d\n",input->n_request);
	printf("numeber of querie : %d\n",input->n_querie);
	printf("Arg1 : %s\n",input->arg1);
	printf("Arg2 : %s\n",input->arg2);
	printf("Arg3 : %s\n",input->arg3);

}

int number_of_results_in_query(FILE *fp){

	int count = 0;
	char *line=NULL;
	size_t len = 0;
	FILE *aux = fp;

	getline(&line, &len, aux);

	while(line != NULL){

		strsep(&line, ";");
		count++;

	}

	free(line);

	return count;

}

void print_result_on_screen(int size_of_window, int columns,int file_index){

	char c='n';   
   static struct termios oldt, newt;
   tcgetattr(STDIN_FILENO, &oldt);
   char *last_file_run=malloc(1000*sizeof(char));
   char *inspect_file=malloc(1000*sizeof(char));
   char *page_manager=malloc(1000*sizeof(char));

   
   FILE *fp=NULL;
	sprintf(inspect_file,"%s%d%s","Resultados/command_interactive_",file_index-1,"_output.txt");
   fp = fopen(inspect_file, "r");

   int lines_in_file = count_number_lines_in_file(fp);
   
   /*now the settings will be copied*/
   newt = oldt;
   
   newt.c_lflag &= ~(ICANON);          
   
   tcsetattr( STDIN_FILENO, TCSANOW, &newt);

   int low_ind=1,high_ind=NUM_ELEM;

   while(toupper(c=getchar()) != 'Q'){

   	if(toupper(c) == 'N' && high_ind < lines_in_file){
   		low_ind+=NUM_ELEM;
   		high_ind+=NUM_ELEM;
   	}

   	if(toupper(c) == 'P' && low_ind > 1){
   		low_ind-=NUM_ELEM;
   		high_ind-=NUM_ELEM;
   	}


   	sprintf(last_file_run,"%s%d%s%d%s%s","awk 'NR >=",low_ind," && NR <= ",high_ind,"' ",inspect_file);
   	system("clear");
   	system(last_file_run);

   	repeat('=',size_of_window);

		sprintf(page_manager,"%s%d%s%d%s%d%s","Elementos ",low_ind," a ",high_ind," (Total: ",lines_in_file,")");
		printCenter(page_manager,size_of_window);
		printCenter("N -> Next Page | P -> Previous Page | Q -> Quit",size_of_window);

		repeat('=',size_of_window);

   }      
                
   /*restore the old settings*/
   tcsetattr(STDIN_FILENO, TCSANOW, &oldt);	
   fclose(fp);
   free(last_file_run);
   free(inspect_file);
   free(page_manager);

}

file confirm_request(int querie,char *arg1,char *arg2,char *arg3,int *valid_run){

			char aux[100];

			if(querie == 1 || querie == 2 || querie == 3 || querie == 4){
		
				printf("Confirma a Query? (Y/N): ");
				printf("%d %s\n",querie,arg1);
				scanf(" %s",aux);
		
			}

			else if(querie == 5 || querie == 7 || querie == 8 || querie == 9){
		
				printf("Confirma a Query? (Y/N): ");
				printf("%d %s %s\n",querie,arg1,arg2);
				scanf(" %s",aux);
		
			}

			else if(querie == 6){
		
				printf("Confirma a Query? (Y/N): ");
				printf("%d %s %s %s\n",querie,arg1,arg2,arg3);
				scanf(" %s",aux);
		
			}

			else{
		
				printf("Invalid querie number\n");
				*valid_run = 0;
		
			}

			
			if(toupper(aux[0])=='Y'){

				file f = malloc(sizeof(struct file));
				f->n_request = *valid_run;
				f->n_querie = querie;
				if(arg1) f->arg1 = strdup(arg1);
				else f->arg1 = NULL;
				if(arg2) f->arg2 = strdup(arg2);
				else f->arg2 = NULL;
				if(arg3) f->arg3 = strdup(arg3);
				else f->arg3 = NULL;

				*valid_run = *valid_run + 1;

				return f;
			}

			else

				return NULL;

}

void menu_principal(unsigned int size,unsigned int size_lines){
	
	//print menu with all queries options and consult result option and quit option

		system("clear");

	//Menu Design
	repeat('=',size);
	repeat('=',size);
	
		printCenter("MENU PRINCIPAL",size);

	repeat('=',size);
	repeat('=',size);

		printCenter("R - Realizar uma Query (1/9)",size);
		printCenter("C - Consultar Último Resultado Gerado",size);
		printCenter("L - Log de Pedidos",size);
		printCenter("Q - Quit Program",size);

	repeat('=',size);
	repeat('=',size);
	printf("\n\n");

}

file input_level_1(char x,int *valid_run,GList *list_inputs,int size){

	switch(toupper(x)){
		case 'R':
			
			file result = do_queries(valid_run); 
			return result;

			break;
		case 'C':
			printf("C\n");
			break;
		case 'L':

			//iterate list and print all files
			while(list_inputs != NULL){
				file_stand_print(list_inputs->data);
				list_inputs = list_inputs->next;
			}

			wait_for_enter();

			break;
		case 'Q':
			printf("Leaving the Program!\n");
			break;
		default:

			printf("Invalid Input\n");
			wait_for_enter();
			
			break;
	}

	return NULL;

}

file do_queries(int *valid_run){

	char querie_number[100]={};
	char arg1[100]={};
	char arg2[100]={};
	char arg3[100]={};


	for(int i=0;i<100;i++){
		querie_number[i]='\0';
		arg1[i]='\0';
		arg2[i]='\0';
		arg3[i]='\0';
	}

	while(querie_number[0]<49 || querie_number[0]>57 || querie_number[1]!=0){
		querie_number[0]=0;
		printf("Querie Number: ");
		scanf(" %s",querie_number);
	}

	switch(atoi(querie_number)){
		case 1:

			get_arg_id(arg1);
			file try1 = confirm_request(1,arg1,NULL,NULL,valid_run);

			if(!try1) cancel_query();
			
			else return try1;

			break;

		case 2:
			
			while(!atoi(arg1)) get_arg_n(arg1);

			file try2 = confirm_request(2,arg1,NULL,NULL,valid_run);

			if(!try2) cancel_query();
			
			else return try2;

			break;

		case 3:
			
			while(!atoi(arg1)) get_arg_n(arg1);

			file try3 = confirm_request(3,arg1,NULL,NULL,valid_run);

			if(!try3) cancel_query();
			
			else return try3;

			break;

		case 4:
			
			get_arg_city(arg1);

			file try4 = confirm_request(4,arg1,NULL,NULL,valid_run);

			if(!try4) cancel_query();
			
			else return try4;

			break;

		case 5:
			
			get_arg_date_inic(arg1);
			get_arg_date_end(arg2);

			file try5 = confirm_request(5,arg1,arg2,NULL,valid_run);

			if(!try5) cancel_query();
			
			else return try5;

			break;

		case 6:
			
			get_arg_city(arg1);
			get_arg_date_inic(arg2);
			get_arg_date_end(arg3);

			file try6 = confirm_request(6,arg1,arg2,arg3,valid_run);

			if(!try6) cancel_query();
			
			else return try6;

			break;

		case 7:
			
			get_arg_n(arg1);
			get_arg_city(arg2);

			file try7 =  confirm_request(7,arg1,arg2,NULL,valid_run);

			if(!try7) cancel_query();
			
			else return try7;

			break;

		case 8:
			
			get_arg_gender(arg1);
			get_arg_n(arg2);

			file try8 =  confirm_request(8,arg1,arg2,NULL,valid_run);

			if(!try8) cancel_query();
			
			else return try8;

			break;

		case 9:
			
			get_arg_date_inic(arg1);
			get_arg_date_end(arg2);

			file try9 =  confirm_request(9,arg1,arg2,NULL,valid_run);

			if(!try9) cancel_query();
			
			else return try9;

			break;

		default:

			//Control never reaches here
			printf("Invalid Input\n");
			wait_for_enter();
			break;
	}

return NULL;

}