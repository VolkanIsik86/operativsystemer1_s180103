# shell
Volkan Isik - s180103


Systemkrav:
Unix baseret Operativsystem, c sproget installeret.

Installation:
Åbn en terminal og skriv: gcc shell.c


Brugermanual: 
Programmet kan køres med ./a.out kommando.
Brugeren vil blive præsenteret med nuværende mappe afsluttet med $:. På nuværende tidspunkt er den klar til at tag imod kommandoer.
Skriv exit for at afslutte programmet og skriv cd for at navigere imellem mapper.


cd - command:
chdir() funktionen bliver kaldt fra c. Denne funktion skifter den nuværende mappe som brugeren befinder sig. Den indtastede string vil blive kontrolleret om den indeholder "cd" skriften.

exit : 
Når man skriver exit vil der blive kaldt exit() funktion fra c. Den indtastede string vil blive kontrolleret om den indeholder "exit" skriften.

Child process 1:

pipe() :
Hvis brugerens indtastede linje indeholde "|" karakteren så vil pipe() funktionen blive kaldt med parameteren pipefd arrayer som indeholder 2 int variabler som er file descriptors.
Ved hjælp af pipe() funktionen kan man nu bruge file descriptors hvor man kan skrive og læse igennem pipe kanalerne. Disse kanaler er tilgængelig imellem processer. 

fork():
Denne funktionen kopier alt koden og generer en child process som har nøjagtig samme kode som parenten.
fork() funktionen returnerer process id som gemmes i pid1 variabel. På denne måde kan vi adskille child og parent koden fra hinanden da child har pid 0 og parent har et positivt tal som pid.
Hvis der skulle være fejl så håndteres det med en exit() funktion kald.

dup2():
Når brugeren af programmet benytter sig af pipe - "|" så vil programmet kalde en dub2() funktion for at benytte pipe kanalerne. dub2() kaldes med pipefd[1](write siden af pipe kanal) og STDOUT_FILENO som er en super global (1 for stdout). På denne måde vil outputtet blive skrevet i pipe.

close():
Begge pipe kanaler lukkes især write siden ellers EOF kan ikke findes ved læsning fra pipe kanal.

execvp():
Funktionen eksekverer en kommando med argumenter. Det anderledes ved execvp er at den kigger i PATH miljøet for ekskevererbar filer. Den første del indtil pipe "|" eller hele linjen  bruges som argument. Funktionen læser alle argumenter indtil den møder null.
Koden efter execvp() vil ikke blive kørt da denne funktion kald erstatter hele koden.


Child process 2 - i tilfælde hvor der bliver kaldt pipe "|" :

fork():  
Denne funktion kaldes igen når når brugeren indtaster pipe "|". For at kommuniker med det første child process vil der blive forket igen så man har to child processer som kommunikerer sammen.

dub2():
Funktionen kaldes med pipefd[0] (read siden af pipe kanallet) og STDIN_FILENO () som er en super global (0 for stdin). Denne kald sørge for at processen læser fra pipe kanal og nu kan udføre den ønskede kommando på det.

Begge pipe kanaler lukkes da de ikke bliver brugt efterfølgende.

execvp(): 
Funktionen kaldes med pipeInput som indeholder pipe argumentet. Alt koden efter execvp i child process vil ikke blive kørt.


Parent kode:

close():
Begge pipe kanaler lukkes hvis de er oprettet pga. pipe "|" ønske fra brugeren.

waitpid():
Denne funktion kaldes med -1 i parameteren som gør at parent processen venter hvad som helst child process. Funktionen kaldes igen med det andet child process.

free():
*shellInput og *pipeInput pointers peger på de variabler som indeholder de forskellige argumenter som
brugeren har indtastet. Disse variabler gemmes i heap med strdup() funktion kald som kopierer en string op i heap. free() vil frigive de allokeret pladser i hukommelsen.

 
type_prompt():
Funktionen skriver en prompt med den nuværende mappe som brugeren befinder sig i med getcwd() funktion kald fra c. Den nuværende mappe returneres med grøn skrift som afslutter med "$"  

read_command(char *shellInput[], char *pipeInput[], int *isPipe):
Brugerens input bliver skrevet i input variable. WordCount initaliseres for antallet af ord og whereIsPipe bruges til at lokalisere "|".

Både stdin og stdout flushes for at undgå åben buffer for input og output.

Input variablen splittes for hver " " space og hver enkelte ord allokeres i heap hvor pointeren gives videre til shellInput arrayet.

Hvis inputtet indeholder pipe "|" så vil pipe "|" ordet i shellInput skiftes med null og der vil blive gemt pipe argumenter i pipeInput.

Funktionen afsluttes med frigivelse af input variablen.
