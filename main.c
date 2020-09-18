#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1


char * inputarr[10];
int * arguments;
int * status;

void type_prompt();
void read_command(char *command , char *parameters);

int main() {

    while (TRUE) {                                /* repeat forever */
        type_prompt();                          /* display promt on the screen */

        char command[20];
        char parameters[80];
        read_command(&command, &parameters);      /* read input from terminal */

        printf("%s\n", *command);
        printf("%s\n", *parameters);

        if (fork() != 0) {                         /* fork off child process */
            /* parent code*/
            waitpid(-1, &status, 0);             /* wait for child to exit */
        } else {
            /*child code */
            execve(command, parameters, 0);      /* execute command */


        }

        return 0;
    }
}


void type_prompt(){
    printf("$ ");
    char* input= malloc(sizeof(char)*100);
    gets(input);

   arguments = malloc(sizeof(int)*2);
   arguments[0] = 0;

    char *inputtokens = strtok(input," ");

    while(inputtokens != NULL) {
        inputarr[arguments[0]] = inputtokens;
        arguments[0]++;
        inputtokens = strtok(NULL," ");
    }


}

void read_command(char *command , char *parameters){
    command = inputarr[0];
    printf("%s\n",command);

    printf("%d\n",arguments[0]);

    char * param = calloc(81, sizeof(char));
    for (int i = 1; i < arguments[0] ; ++i) {
        strcat(param,inputarr[i]);
        if (i!=(arguments[0]-1)) {
            strcat(param, " ");
        }
    }

    parameters = param;




}