//Esempio shell con possibilita' di eseguire in background usando la &

#include "procedure.h"

int main() {
	char buffer[128];
	pid_t pid;
	int n;
	char* args[10];
	char* token;

	while(1){

		printf("Shell> ");
		//scanf("%127s\n", buffer); // scanf non legge l'intera stringa se contiene spazi
		fgets(buffer, 127, stdin);
		buffer[strlen(buffer) - 1] = '\0';

		printf("buffer='%s'\n", buffer);


		token = strtok(buffer, " ");

		args[0] = token;
		printf("ARG 0: '%s'\n", args[0]);

		n = 1;
		while(token != NULL && n<10) {

			token = strtok(NULL, " ");

			args[n] = token;
			printf("ARG %d: '%s'\n", n, args[n]);

			n++;
		}
		printf("ciao");
		args[9] = (char *)0;

		if(!strcmp(args[0], "exit")){
			return 0;//exit(0);
		}
		
		if(strcmp(args[1],"&")==0)
			{
				printf("Eseguo BackGroud \n");
				for(int i=1;i<9;i++)
					args[i]=args[i+1];
				args[9]='\0';
				                                printf("Eseguo BackGroud \n");

				execback(args[0],args);
			}
		else
			{
				printf("Eseguo normale \n");
				exnoback(args[0],args);
			}
	}	
}
