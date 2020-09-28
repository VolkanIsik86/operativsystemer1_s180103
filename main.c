#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define TRUE 1

void type_prompt();

void read_command(char *shellInput[], char *pipeInput[], int *isPipe);

int main() {
    /**
     * Koden gentager uendeligt
     */
    while (TRUE) {
        /**
         * pipefd[2] = Pipe kanalerne
         * isPipe = variabel som ændres i tilfælde af pipe
         * pid1,pid2 = process id'er initaliseres.
         */
        int pipefd[2];
        int isPipe = 0;
        int pid1;
        int pid2;
        /**
         * en prompt på skærmen
         */
        type_prompt();                          /* display promt on the screen */
        /**
         * *shellInput[20] = en char array pointer (2 dimentionelle array). Indeholder argumenter deres eskeveres.
         * *pipeInput[20] = en char array pointer (2 dimentionelle array). Indeholder pipe argumenter
         */
        char *shellInput[20];
        char *pipeInput[20];

        /**
         * funktionen som indlæser vores input og seperer shellinput og pipeinput i tilfæle af pipe
         */
        read_command(shellInput, pipeInput, &isPipe);      /* read input from terminal */


        /**
         * exit funktionen for vores shell
         */
        int leave = strcmp(shellInput[0], "exit");
        if (leave == 0) {
            exit(0);

        }

        /**
         * Hardcoded cd funktion benyttet c's egen funktion til at skifte nuværende mappe.
         */
        int cd = strcmp(shellInput[0], "cd");
        if (cd == 0) {
            chdir(shellInput[1]);
        } else {
            /**
             * hvis der er en pipe så pipe funktionen kaldes for pipekanallerne
             */
            if (isPipe) {
                pipe(pipefd); /* https://www.geeksforgeeks.org/pipe-system-call/ */
            }
            /**
             * En child process forkes dette gør at den nu værende process kopiers
             */
            pid1 = fork();   /* https://www.geeksforgeeks.org/fork-system-call/ */
            /**
             * fejl håndtering
             */
            if (pid1 < 0) {
                perror("fork");
                exit(1);
            } else if (pid1 == 0) {
                /**
                 * childprocess koden
                 */
                if (isPipe) {
                    /**
                     *  output kanallen duplikeres og begge kanaller lukkes derefter
                     *  dup2 kaldes med pipe write kanalen og strout gives som argument
                     *  begge kanaler lukkes efterfølgende især write kanalen så reader i et andet process kan finde EOF
                     */
                    dup2(pipefd[1], STDOUT_FILENO); /* https://www.geeksforgeeks.org/dup-dup2-linux-system-call/ */
                    close(pipefd[1]);
                    close(pipefd[0]);
                }
                /**
                 * execvp funktionen kører en række af argumenter og erstatter alt koden efter.
                 */
                execvp(shellInput[0], shellInput);      /* inspireret fra Bhupjit */

            } else if (isPipe) {

                /**
                 * hvis isPipe af sandt så forkes en ny child process
                 */
                pid2 = fork();
                if (pid2 == 0) {
                    /**
                     * childprocess 2 kode:
                     */
                    dup2(pipefd[0], STDIN_FILENO);
                    close(pipefd[1]);
                    close(pipefd[0]);
                    execvp(pipeInput[0], pipeInput);

                } else {
                    /**
                     * parentkode hvis isPipe af sandt
                     */
                    close(pipefd[0]);
                    close(pipefd[1]);
                    /**
                     * venter på begge child processer kører færdig
                     */
                    waitpid(-1, NULL, 0);
                    waitpid(-1, NULL, 0);
                    free(*shellInput);
                    free(*pipeInput);

                }


            } else {
                /**
                 * parentkode uden pipe
                 */
                wait(NULL);
                free(*shellInput);
            }
        }
    }
}

void type_prompt() {
    /**
     * allokerer plads for nuværende mappe sætning
     */
    char directory[1000];
    /**
     * returner nuværende mappe
     */
    getcwd(directory, 1000);
    /**
     * ændrer farver på prompt
     * www.web.theurbanpenguin.com/adding-color-to-your-output-from-c
     */
    printf("\033[1;32m");
    printf("%s:$ ", directory);
    printf("\33[0m");
}

void read_command(char *shellInput[], char *pipeInput[], int *isPipe) {
    /**
     * maxlength = maximum længde af en input
     * *input allokerer plads i heap for input
     * wordCount = variabel for at tælle antal argumenter
     * whereIsPipe = indeholder pladsen for "|"
     */
    int maxlength = 100;
    char *input = malloc(sizeof(char) * maxlength);
    int wordCount = 0;
    int whereIsPipe = 0;
    /**
     *  scanner input fra consolen.
     */
    scanf("%[^\n]%*c", input);  /* https://www.geeksforgeeks.org/taking-string-input-space-c-3-different-methods/   */

    /**
     * flusher input og output stream
     */
    fflush(stdin);
    fflush(stdout);

    /**
     * *args = indeholder argumenter som delt med " " . strtok læser et string af gangen indtil den møder en ny " ".
     * Linjen gennemgåes og kopiers til heap hvor shellinput indeholder pointer til hver string
     */
    char *args = strtok(input, " ");

    while (args != NULL) {
        shellInput[wordCount] = strdup(args);
        args = strtok(NULL, " ");
        wordCount++;
    }
    shellInput[wordCount] = NULL;
    /**
     * "|" søges efter og pladsen gennem is whereIsPipe
     */
    for (int i = 0; i < wordCount; ++i) {
        if (!strcmp(shellInput[i], "|")) {
            *isPipe = 1;
            whereIsPipe = i;
        }
    }
    /**
     * hvis der findes "|" så vil shellinput indeholde argumenter indtil pipe og pipeInput vil indeholde pipe argumenter
     */
    if (*isPipe) {
        shellInput[whereIsPipe] = NULL;
        for (int i = 0; i < wordCount - whereIsPipe; ++i) {
            pipeInput[i] = shellInput[whereIsPipe + 1 + i];
        }
        pipeInput[(wordCount - whereIsPipe) + 1] = NULL;
    }
    free(input);
}
