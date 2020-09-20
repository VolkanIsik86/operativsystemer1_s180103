#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define TRUE 1

void type_prompt();
void read_command(char * shellInput[]);

int main() {

    while (TRUE) {                                /* repeat forever */
        int pid;
        type_prompt();                          /* display promt on the screen */

        char *shellInput[20];

        read_command(shellInput);      /* read input from terminal */

        int exit = strcmp(shellInput[0],"exit");
        if(exit == 0){
            break;
        }

        pid = fork();
        if (pid > 0) {                         /* fork off child process */
            /* parent code*/
           pid = wait(NULL);             /* wait for child to exit */
           printf("Type new command or type exit to leave\n");
        } else {
            /*child code */
            execvp(shellInput[0], shellInput);      /* execute command */
        }
    }
    return 0;
}

void type_prompt() {
    printf("$ ");
}
    void read_command(char *shellInput[]) {

        int maxlength = 100;
        char *input = malloc(sizeof(char) * maxlength);
        int wordCount = 0;

        scanf("%[^\n]%*c",input);

       // fgets(input, 100, stdin);
        fflush(stdin);
        fflush(stdout);

        char *args = strtok(input," ");

        while (args != NULL ){
            shellInput[wordCount] = strdup(args);
            args = strtok(NULL," ");
            wordCount++;
        }
        shellInput[wordCount] = NULL;

    free(input);
    }
