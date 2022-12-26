// C Program to design a shell in Linux
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include <signal.h>

#define maxLetters 200 // max number of letters to be supported
#define maxCom 200 // max number of commands to be supported


void firstStr(char *ch);

void numLine(char *ch);

void emptySpace(char *ch);

void mostRepeated(char *ch);

void tenLine(char *ch);

void noComment(char *ch);

void historyFunc();

void startShell();

void printDir();

int getInput(char *ch);

int processString(char *str, char **parsed, char **parsedPipe);

void execArgs(char **parsed);

void parseCommands(char *str, char **parsed);

void sigintHandler();

int commandHandler(char **parsed);

void openHelp();

int main() {
    char inputString[maxLetters], *parsedArgs[maxCom];
    char *parsedArgsPiped[maxCom];
    int execFlag ;
    startShell();

    signal(SIGINT, sigintHandler);

    while (1) {
        // print shell line
        printDir();
        // take input
        if (getInput(inputString))
            continue;

        // process
        execFlag = processString(inputString,
                                 parsedArgs, parsedArgsPiped);
        // execFlag returns zero if there is no command
        // or it is a builtin command,
        // 1 if it is a simple command
        // 2 if it is including a pipe.

        // execute
        if (execFlag == 1)
            execArgs(parsedArgs);

//        if (execFlag == 2)
//            execArgsPiped(parsedArgs, parsedArgsPiped);
    }
}

// Greeting shell during startup
void startShell() {
    printf("\033[H\033[J");
    for (int i = 1; i <= 10; i++) {
        for (int j = 1; j <= 60; j++) {
            if (i == 1 || i == 10 || j == 1 || j == 60) {
                if (i != 5)
                    printf("*");
            } else {
                if (i == 5 && j == 2) {
                    printf("\t\t  -WELCOME TO MY SHELL-");
                }
                printf(" ");
            }
        }
        printf("\n");
    }
    sleep(2);
    printf("\033[H\033[J");
}

// Function to take input

int getInput(char *ch) {
    char *buf;
    buf = readline("\n>>> ");
    if (strlen(buf) != 0) {
        add_history(buf);
        strcpy(ch, buf);
        return 0;
    } else {
        return 1;
    }
}

void sigintHandler() {
    signal(SIGINT, sigintHandler);
    fflush(stdout);
}


// Function to print Current Directory.
void printDir() {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDir: %s", cwd);
}

// Function where the system command is executed
void execArgs(char **parsed) {
    // Forking a child
    pid_t pid = fork();

    if (pid == -1) {
        fprintf(stderr, "%s", "No Process Created !\n");
        return;
    } else if (pid == 0) {
        if (execvp(parsed[0], parsed) < 0) {
            fprintf(stderr, "%s", " No Child Processed ! \n");
        }
        exit(0);
    } else {
        // waiting for child to terminate
        wait(NULL);
        return;
    }
}

// Function where the piped system commands is executed
//void execArgsPiped(char **parsed, char **parsedPipe) {
//    // 0 is read end, 1 is write end
//    int pipeFd[2];
//    pid_t p1, p2;
//
//    if (pipe(pipeFd) < 0) {
//        printf("\nPipe could not be initialized");
//        return;
//    }
//    p1 = fork();
//    if (p1 < 0) {
//        printf("\nCould not fork");
//        return;
//    }
//
//    if (p1 == 0) {
//        // Child 1 executing..
//        // It only needs to write at the write end
//        close(pipeFd[0]);
//        dup2(pipeFd[1], STDOUT_FILENO);
//        close(pipeFd[1]);
//
//        if (execvp(parsed[0], parsed) < 0) {
//            printf("\nCould not execute command 1..");
//            exit(0);
//        }
//    } else {
//        // Parent executing
//        p2 = fork();
//
//        if (p2 < 0) {
//            printf("\nCould not fork");
//            return;
//        }
//
//        // Child 2 executing..
//        // It only needs to read at the read end
//        if (p2 == 0) {
//            close(pipeFd[1]);
//            dup2(pipeFd[0], STDIN_FILENO);
//            close(pipeFd[0]);
//            if (execvp(parsedPipe[0], parsedPipe) < 0) {
//                printf("\nCould not execute command 2..");
//                exit(0);
//            }
//        } else {
//            // parent executing, waiting for two children
//            wait(NULL);
//            wait(NULL);
//        }
//    }
//}

// Help command builtin
void openHelp() {
    puts("\n***WELCOME TO MY SHELL HELP***"
         "\nSupported Commands are :"
         "\n>>cd"
         "\n>>ls"
         "\n>>firstStr"
         "\n>>mostReap"
         "\n>>noSpace"
         "\n>>notComment"
         "\n>>numLine"
         "\n>>tenLine"
         "\n>>exit");
}

// Function to execute builtin commands
int commandHandler(char **parsed) {
    pid_t pid1 = fork();
    if (pid1 < 0) {
        fprintf(stderr, "%s", "No Process Created !\n");
        return;
    }
    if (pid1 == 0) {
        printf(" child : %d \n", getpid());
        printf("parent : %d \n", getppid());

        if (strcmp(parsed[0], "exit") == 0) {
            printf("Exit from shell ! \n");
            exit(0);
            return 1;
        } else if (strcmp(parsed[0], "cd") == 0) {
            chdir(parsed[1]);
            return 1;
        } else if (strcmp(parsed[0], "help") == 0) {
            openHelp();
            return 1;
        } else if (strcmp(parsed[0], "firstStr") == 0) {
            firstStr(parsed[1]);
            return 1;
        } else if (strcmp(parsed[0], "noSpace") == 0) {
            emptySpace(parsed[1]);
            return 1;
        } else if (strcmp(parsed[0], "mostRep") == 0) {
            mostRepeated(parsed[1]);
            return 1;
        } else if (strcmp(parsed[0], "numLine") == 0) {
            numLine(parsed[1]);
            return 1;
        } else if (strcmp(parsed[0], "tenLine") == 0) {
            tenLine(parsed[1]);
            return 1;
        } else if (strcmp(parsed[0], "noComment") == 0) {
            noComment(parsed[1]);
            return 1;
        } else if (strcmp(parsed[0], "history") == 0) {
            historyFunc();
            return 1;
        }
    } else {
        printf("Im parent %d", getpid());
        wait(NULL);
    }
    return 0;
}

void firstStr(char *ch) {
    FILE *file;
    char singleLine[100];
    if (access(ch, F_OK) == 0) {
        file = fopen(ch, "r");

        char *pointerCh;
        while (!feof(file)) {
            fgets(singleLine, 100, file);
            pointerCh = strtok(singleLine, " ");
            printf("%s\n", pointerCh);
        }
        fclose(file);
    } else {
        fprintf(stderr, "%s", "No File Found !\n");
    }
}

void emptySpace(char *ch) {
    FILE *file;
    char fileBuff[100];
    int fileChar;

    file = fopen(ch, "r");
    if (file == NULL) {
        fprintf(stderr, "%s", "No File Found !\n");
        return;
    }

    fgets(fileBuff, 100, file);
    fseek(file, 0, SEEK_SET);
    while (fileChar != EOF) {
        fileChar = fgetc(file);
        if (isgraph(fileChar)) {
            putchar(fileChar);
        }
    }
    fclose(file);
    return;
}

void mostRepeated(char *ch1) {
    FILE *file;
    char ch, *line;
    int len = 0, read;
    char words[100][100], word[20];
    int i = 0, j = 0, k = 0, maxCount = 0, count = 0, index = 0;

    file = fopen(ch1, "r");
    if (file == NULL) {
        fprintf(stderr, "%s", "No File Found !\n");
    }

    while (getline(&line, &len, file) != EOF) {
        for (index = 0; line[index] != '\0'; index++) {
            if (line[index] != ' ' && line[index] != '\n' && line[index] != ',' && line[index] != '.') {
                words[i][j] = tolower(line[index]);
                j++;
            } else {
                words[i][j] = '\0';
                i++;
                j = 0;
            }
        }
    }

    int length = i;
    for (int i = 0; i < length; i++) {
        count = 1;
        for (j = i + 1; j < length; j++) {
            if (strcmp(words[i], words[j]) == 0 && (strcmp(words[i], "") != 0)) {
                count++;
            }
        }
        if (count > maxCount) {
            maxCount = count;
            strcpy(word, words[i]);
        }
    }
    printf("Most Repeated word is : %s ", word);
    fclose(file);
    return;
}

void numLine(char *ch) {
    FILE *file;
    char singleLine[100];
    int count = 0;
    if (access(ch, F_OK) == 0) {
        file = fopen(ch, "r");

        while (!feof(file)) {
            fgets(singleLine, 100, file);
            count++;
        }
        printf("Number of line is : %d", count);
        fclose(file);
    } else {
        fprintf(stderr, "%s", "No File Found !\n");
    }
}

void tenLine(char *ch) {
    FILE *file;
    char singleLine[150];
    int count = 0;
    if (access(ch, F_OK) == 0) {
        file = fopen(ch, "r");

        while (!feof(file) && count < 10) {
            fgets(singleLine, 100, file);
            printf("%s", singleLine);
            count++;
        }
        fclose(file);
    } else {
        fprintf(stderr, "%s", "No File Found !\n");
    }
}

void noComment(char *ch) {
    FILE *file;
    char singleLine[100];
    if (access(ch, F_OK) == 0) {
        file = fopen(ch, "r");

        char *pointerCh;
        while (!feof(file)) {
            fgets(singleLine, 100, file);
            pointerCh = strtok(singleLine, " ");
            if (pointerCh[0] == '#') {
                continue;
            } else {
                printf("%s", singleLine);
            }
        }
        fclose(file);
    } else {
        fprintf(stderr, "%s", "No File Found !\n");
    }
}

void historyFunc() {
    FILE *file;
    file = fopen("history.txt", "w");
    HISTORY_STATE *myHist = history_get_history_state();
    /* retrieve the history list */
    HIST_ENTRY **myList = history_list();
    for (int i = 0; i < myHist->length; i++) { /* output history list */
        printf("%s \n", mylist[i]->line);
        fprintf(file, "%s\n ", mylist[i]->line);
    }
    fclose(file);
}

// function for finding pipe
int parsePipe(char *str, char **strPiped) {
    int i;
    for (i = 0; i < 2; i++) {
        strPiped[i] = strsep(&str, "|");
        if (strPiped[i] == NULL)
            break;
    }

    if (strPiped[1] == NULL)
        return 0; // returns zero if no pipe is found.
    else {
        return 1;
    }
}

// function for parsing command words
void parseCommands(char *str, char **parsed) {
    int i;

    for (i = 0; i < maxCom; i++) {
        parsed[i] = strsep(&str, " ");

        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i--;
    }
}

int processString(char *str, char **parsed, char **parsedPipe) {

    char *strPiped[2];
    int piped = 0;

    piped = parsePipe(str, strPiped);

    if (piped) {
        parseCommands(strPiped[0], parsed);
        parseCommands(strPiped[1], parsedPipe);

    } else {
        parseCommands(str, parsed);
    }

    if (commandHandler(parsed))
        return 0;
    else
        return 1 + piped;
}



