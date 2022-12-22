// C Program to design a shell in Linux
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>

#define MAXCOM 1000 // max number of letters to be supported
#define MAXLIST 100 // max number of commands to be supported

// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")

// Greeting shell during startup
void init_shell() {
    clear();
    printf("\n\n\n\n******************"
           "************************");
    printf("\n\n\n\t****MY SHELL****");
    printf("\n\n\t-USE AT YOUR OWN RISK-");
    printf("\n\n\n\n*******************"
           "***********************");
    char *username = getenv("USER");
    printf("\n\n\nUSER is: @%s", username);
    printf("\n");
    sleep(1);
    clear();
}

// Function to take input
int takeInput(char *str) {
    char *buf;

    buf = readline("\n>>> ");
    if (strlen(buf) != 0) {
        add_history(buf);
        strcpy(str, buf);
        return 0;
    } else {
        return 1;
    }
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
        printf("\nFailed forking child..");
        return;
    } else if (pid == 0) {
        if (execvp(parsed[0], parsed) < 0) {
            printf("\nCould not execute command..");
        }
        exit(0);
    } else {
        // waiting for child to terminate
        wait(NULL);
        return;
    }
}

// Function where the piped system commands is executed
void execArgsPiped(char **parsed, char **parsedpipe) {
    // 0 is read end, 1 is write end
    int pipefd[2];
    pid_t p1, p2;

    if (pipe(pipefd) < 0) {
        printf("\nPipe could not be initialized");
        return;
    }
    p1 = fork();
    if (p1 < 0) {
        printf("\nCould not fork");
        return;
    }

    if (p1 == 0) {
        // Child 1 executing..
        // It only needs to write at the write end
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        if (execvp(parsed[0], parsed) < 0) {
            printf("\nCould not execute command 1..");
            exit(0);
        }
    } else {
        // Parent executing
        p2 = fork();

        if (p2 < 0) {
            printf("\nCould not fork");
            return;
        }

        // Child 2 executing..
        // It only needs to read at the read end
        if (p2 == 0) {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if (execvp(parsedpipe[0], parsedpipe) < 0) {
                printf("\nCould not execute command 2..");
                exit(0);
            }
        } else {
            // parent executing, waiting for two children
            wait(NULL);
            wait(NULL);
        }
    }
}

// Help command builtin
void openHelp() {
    puts("\n***WELCOME TO MY SHELL HELP***"
         "\nCopyright @ Suprotik Dey"
         "\n-Use the shell at your own risk..."
         "\nList of Commands supported:"
         "\n>cd"
         "\n>ls"
         "\nfirststr"
         "\nmostreap"
         "\nnospace"
         "\nnotcomment"
         "\nnumline"
         "\ntenline"
         "\n>exit"
         "\n>all other general commands available in UNIX shell"
         "\n>pipe handling"
         "\n>improper space handling");

    return;
}

// Function to execute builtin commands
int ownCmdHandler(char **parsed) {
    int NoOfOwnCmds = 10, i, switchOwnArg = 0;
    char *ListOfOwnCmds[NoOfOwnCmds];
    char *username;

    ListOfOwnCmds[0] = "exit";
    ListOfOwnCmds[1] = "cd";
    ListOfOwnCmds[2] = "help";
    ListOfOwnCmds[3] = "hello";
    ListOfOwnCmds[4] = "firststr";
    ListOfOwnCmds[5] = "nospace";
    ListOfOwnCmds[6] = "mostrep";
    ListOfOwnCmds[7] = "numline";
    ListOfOwnCmds[8] = "tenline";
    ListOfOwnCmds[9] = "nocomment";


    for (i = 0; i < NoOfOwnCmds; i++) {
        if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) {
            switchOwnArg = i + 1;
            break;
        }
    }

    pid_t pid1 = fork();
    switch (switchOwnArg) {
            case 1:
                printf("\nGoodbye\n");
            exit(0);
            case 2:
                chdir(parsed[1]);
            return 1;
            case 3:
                openHelp();
            return 1;
            case 4:
                username = getenv("USER");
            printf("\nHello %s.\nMind that this is "
                   "not a place to play around."
                   "\nUse help to know more..\n",
                   username);
            return 1;
            case 5:
                if (pid1 == 0) {
                    printf(" child : %d \n", getpid());
                    printf("parent : %d \n", getppid());
                    firstStr(parsed[1]);
                    exit(0);
                } else {
                    printf("im parent : %d", getpid());
                    wait(NULL);
                }
            return 1;
            case 6:
                if (pid1 == 0) {
                    printf(" child : %d \n", getpid());
                    printf("parent : %d \n", getppid());
                    emptySpace(parsed[1]);
                    exit(0);
                } else {
                    printf("im parent : %d", getpid());
                    wait(NULL);
                    return;
                }
            return 1;
            case 7:
                if (pid1 == 0) {
                    printf(" child : %d \n", getpid());
                    printf("parent : %d \n", getppid());
                    mostRepeated(parsed[1]);
                    exit(0);
                } else {
                    printf("im parent : %d", getpid());
                    wait(NULL);
                }
            return 1;
            case 8:
                if (pid1 == 0) {
                    printf(" child : %d \n", getpid());
                    printf("parent : %d \n", getppid());
                    numLine(parsed[1]);
                    exit(0);
                } else {
                    printf("im parent : %d", getpid());
                    wait(NULL);
                }
            return 1;
            case 9:
                if (pid1 == 0) {
                    printf(" child : %d \n", getpid());
                    printf("parent : %d \n", getppid());
                    tenLine(parsed[1]);
                    exit(0);
                } else {
                    printf("im parent : %d", getpid());
                    wait(NULL);
                }
            return 1;
            case 10:
                if (pid1 == 0) {
                    printf(" child : %d \n", getpid());
                    printf("parent : %d \n", getppid());
                    noComment(parsed[1]);
                    exit(0);
                } else {
                    printf("im parent : %d\n", getpid());
                    wait(NULL);
                }
            return 1;
            default:
                break;
        }
    return 0;
}

void firstStr(char *ch) {
    FILE *fPointer, *fWords, *fWordCopy;
    char singleLine[150];
    if (access(ch, F_OK) == 0) {
        fPointer = fopen(ch, "r");

        char *pch;
        while (!feof(fPointer)) {
            fgets(singleLine, 100, fPointer);
            pch = strtok(singleLine, " ,'(");
            printf("%s\n", pch);
        }
        fclose(fPointer);
    } else {
        fprintf(stderr , "%s" , "No File Found !\n");
    }
}

void emptySpace(char *ch) {
    FILE *filePtr;
    char fileBuff[255];
    int fileChar;

    filePtr = fopen(ch, "r");
    if (filePtr == NULL) {
        fprintf(stderr , "%s" , "No File Found !\n");
        return;
    }

    fgets(fileBuff, 255, filePtr);
    fseek(filePtr, 0, SEEK_SET);
    do {
        fileChar = fgetc(filePtr);
        if (isgraph(fileChar)) {
            putchar(fileChar);
        }
    } while (fileChar != EOF);
    fclose(filePtr);
    return;
}

void mostRepeated(char *ch1) {
    FILE *file;
    char ch, *line;
    size_t len = 0, read;
    char words[1000][1000], word[20];
    int i = 0, j = 0, k = 0, maxCount = 0, count = 0;

    file = fopen(ch1, "r");
    if (file == NULL) {
        fprintf(stderr , "%s" , "No File Found !\n");
    }
    while ((read = getline(&line, &len, file)) != -1) {
        for (k = 0; line[k] != '\0'; k++) {
            if (line[k] != ' ' && line[k] != '\n' && line[k] != ',' && line[k] != '.') {
                words[i][j++] = tolower(line[k]);
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
    FILE *fPointer;
    int count = 0;
    char c;
    if (access(ch, F_OK) == 0) {
        fPointer = fopen(ch, "r");
        for (c = getc(fPointer); c != EOF; c = getc(fPointer)) {
            if (c == '\n')
                count++;
        }
        printf("Number of Line is : %d", count + 1);
        fclose(fPointer);
    } else {
        fprintf(stderr , "%s" , "No File Found !\n");
    }
}

void tenLine(char *ch) {
    FILE *fPointer, *fWords, *fWordCopy;
    char singleLine[150];
    int count = 0;
    if (access(ch, F_OK) == 0) {
        fPointer = fopen(ch, "r");

        char *pch;
        while (!feof(fPointer) && count < 10) {
            fgets(singleLine, 100, fPointer);
            printf("%s", singleLine);
            count++;
        }
        fclose(fPointer);
    } else {
        fprintf(stderr , "%s" , "No File Found !\n");
    }
}

void noComment(char *ch) {
    FILE *fPointer, *fWords, *fWordCopy;
    char singleLine[150];
    if (access(ch, F_OK) == 0) {
        fPointer = fopen(ch, "r");

        char *pch;
        while (!feof(fPointer)) {
            fgets(singleLine, 100, fPointer);
            pch = strtok(singleLine, " ,'(");
            if (pch[0] == '#') {
                continue;
            } else {
                printf("%s", singleLine);
            }
        }
        fclose(fPointer);
    } else {
        fprintf(stderr , "%s" , "No File Found !\n");
    }
}

// function for finding pipe
int parsePipe(char *str, char **strpiped) {
    int i;
    for (i = 0; i < 2; i++) {
        strpiped[i] = strsep(&str, "|");
        if (strpiped[i] == NULL)
            break;
    }

    if (strpiped[1] == NULL)
        return 0; // returns zero if no pipe is found.
    else {
        return 1;
    }
}

// function for parsing command words
void parseSpace(char *str, char **parsed) {
    int i;

    for (i = 0; i < MAXLIST; i++) {
        parsed[i] = strsep(&str, " ");

        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i--;
    }
}

int processString(char *str, char **parsed, char **parsedpipe) {

    char *strpiped[2];
    int piped = 0;

    piped = parsePipe(str, strpiped);

    if (piped) {
        parseSpace(strpiped[0], parsed);
        parseSpace(strpiped[1], parsedpipe);

    } else {

        parseSpace(str, parsed);
    }

    if (ownCmdHandler(parsed))
        return 0;
    else
        return 1 + piped;
}

void execArgsCmd(char **parsed) {
    pid_t pid = fork();
    if (pid < 0) {
        printf("Error !");
        return;
    } else if (pid == 0) {
        printf("Im child :\n");
        printf("child : %d \n", getpid());
        printf("parent : %d\n", getppid());
        exit(0);
    } else {
        printf("Im parent : \n");
        wait(NULL);
        printf("parent %d\n", getpid());
        return;
    }

}

int main() {
    char inputString[MAXCOM], *parsedArgs[MAXLIST];
    char *parsedArgsPiped[MAXLIST];
    int execFlag = 0;
    init_shell();

    while (1) {
        // print shell line
        printDir();
        // take input
        if (takeInput(inputString))
            continue;
        // process
        execFlag = processString(inputString,
                                 parsedArgs, parsedArgsPiped);
        // execflag returns zero if there is no command
        // or it is a builtin command,
        // 1 if it is a simple command
        // 2 if it is including a pipe.
//        printf("%d", execFlag);

//        if(execFlag == 0)
//            execArgsCmd(parsedArgs);
        // execute
        if (execFlag == 1)
            execArgs(parsedArgs);

        if (execFlag == 2)
            execArgsPiped(parsedArgs, parsedArgsPiped);
    }
    return 0;
}
