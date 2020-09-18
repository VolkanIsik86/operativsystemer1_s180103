#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1


char * inputarr[10];
int * status;

void type_prompt();
void read_command(char *command , char *parameters);

int main() {

    while (TRUE) {                                /* repeat forever */
        type_prompt();                          /* display promt on the screen */

        char *command = NULL;
        char * parameters = NULL;
        read_command(command, parameters);      /* read input from terminal */

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

    int i = 0;

    char *inputtokens = strtok(input," ");

    while(inputtokens != NULL) {
        inputarr[i] = inputtokens;
        i++;
        inputtokens = strtok(NULL," ");
    }


}

void read_command(char *command , char *parameters){
    command = inputarr[0];
    parameters = inputarr[1];
}