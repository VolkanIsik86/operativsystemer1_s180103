#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define TRUE 1

void type_prompt();
void read_command(char * shellInput[] ,char *pipeInput[], int * isPipe);

int main() {

    while (TRUE) {                                /* repeat forever */
        int pipefd[2];
        char recv[256];
        int isPipe = 0;
        int pid;

        type_prompt();                          /* display promt on the screen */

        char *shellInput[20];
        char *pipeInput[20];

        read_command(shellInput, pipeInput, &isPipe);      /* read input from terminal */


        //exit
        int leave = strcmp(shellInput[0], "exit");
        if (leave == 0) {
            exit(0);
            break;
        }

        int cd = strcmp(shellInput[0], "cd");
        if (cd == 0) {
            chdir(shellInput[1]);
        } else {

            //fork
            pid = fork();   /* fork off child process */

            if (pid < 0) {
                perror("fork");
                exit(1);
            } else if (pid == 0) {
                /*child code */
                //pipe
                if (isPipe) {
                    pipe(pipefd);
                    fork();

                }

                close(pipefd[0]);
                execvp(shellInput[0], shellInput);      /* execute command */

            } else {
                /* parent code*/
                pid = wait(NULL);             /* wait for child to exit */
                printf("Type new command or type exit to leave\n");
            }
        }
    }
    return 0;
}

void type_prompt() {
    char directory[1000];
    getcwd(directory,1000);
    printf("\033[1;32m");
    printf("%s:$ ",directory);
    printf("\33[0m");
}
    void read_command(char *shellInput[] ,char *pipeInput[], int * isPipe) {

        int maxlength = 100;
        char *input = malloc(sizeof(char) * maxlength);
        int wordCount = 0;
        int whereIsPipe = 0;

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

        for (int i = 0; i < wordCount ; ++i) {
            if(!strcmp(shellInput[i],"|")){
                *isPipe=1;
                whereIsPipe = i;
            }
        }

        if(*isPipe){
            shellInput[whereIsPipe] = NULL;
            for (int i = 0; i < wordCount - whereIsPipe ; ++i) {
                pipeInput[i] = shellInput[whereIsPipe+1+i];
            }
            pipeInput[(wordCount-whereIsPipe)+1] = NULL;
        }

//        for (int i = 0; i < whereIsPipe ; ++i) {
//            printf("%s\n",shellInput[i]);
//
//        }
//
//        for (int i = 0; i < wordCount-whereIsPipe ; ++i) {
//            printf("%s\n",pipeInput[i]);
//        }

    free(input);
    }
