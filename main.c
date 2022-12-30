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

int processString(char *str, char **parsed);

void execArgs(char **parsed);

void parseCommands(char *str, char **parsed);

void sigintHandler();

int commandHandler(char **parsed);

void openHelp();

int main() {
    char inputString[maxLetters], *parsedArgs[maxCom];
    int executeFlag;
    startShell();

    signal(SIGINT, sigintHandler);

    while (1) {
        printDir();
        if (getInput(inputString))
            continue;

        executeFlag = processString(inputString, parsedArgs);
        // execFlag returns zero if there is no command or it is a builtin command,

        // or == 1 and execute simple command
        if (executeFlag == 1)
            execArgs(parsedArgs);
    }
}

// function that show at the start of the shell
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

//function ro handle ctrl + c
void sigintHandler() {
    signal(SIGINT, sigintHandler);
    fflush(stdout);
}


// Function to print Current Directory
void printDir() {
    char dir[1024];
    getcwd(dir, sizeof(dir));
    printf("\nDir: %s", dir);
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

// function that use for Help command
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
        return 0;
    }
    if (pid1 == 0) {
        if (strcmp(parsed[0], "exit") == 0) {
            printf("Exit from shell ! \n");
            kill(0,SIGKILL);
            return 1;
        }
        if (strcmp(parsed[0], "cd") == 0) {
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
        wait(NULL);
    }
    return 0;
}

// function to print first string of each line
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

// function to remove any space in text
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

// function to print most repeated word in text
void mostRepeated(char *ch1) {
    FILE *file;
    char *line;
    int len = 0;
    char words[100][100], word[20];
    int i = 0, j = 0, maxCount = 0, count = 0, index = 0;

    file = fopen(ch1, "r");
    if (file == NULL) {
        fprintf(stderr, "%s", "No File Found !\n");
    }

    while (getline(&line, &len, file) != EOF) {
        for (index = 0; line[index] != '\0'; index++) {
            if (line[index] != ' ' && line[index] != '\n' && line[index] != ',') {
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

// function to print number of lines
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

// function to print first 10 lines
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

// function to remove comments that specified with # sign
void noComment(char *ch) {
    FILE *file;
    char singleLine[100];
    char slCpy[100];
    if (access(ch, F_OK) == 0) {
        file = fopen(ch, "r");

        char *pointerCh;
        while (!feof(file)) {
            fgets(singleLine, 100, file);
            strcpy(slCpy, singleLine);
            pointerCh = strtok(slCpy, " ");
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

// function to print the history of commands that entered in shell and save in history.txt file
void historyFunc() {
    FILE *file;
    file = fopen("history.txt", "w");
    HISTORY_STATE *myHist = history_get_history_state();
    /* retrieve the history list */
    HIST_ENTRY **myList = history_list();
    for (int i = 0; i < myHist->length; i++) { /* output history list */
        printf("%s \n", myList[i]->line);
        fprintf(file, "%s\n ", myList[i]->line);
    }
    fclose(file);
}

// function for parsing command words
void parseCommands(char *str, char **parsedStr) {
    int index;

    for (index = 0; index < maxCom; index++) {
        parsedStr[index] = strsep(&str, " ");

        if (parsedStr[index] == NULL)
            break;
        if (strlen(parsedStr[index]) == 0)
            index--;
    }
}

int processString(char *str, char **parsedStr) {

    parseCommands(str, parsedStr);

    if (commandHandler(parsedStr))
        return 0;
    else
        return 1;
}