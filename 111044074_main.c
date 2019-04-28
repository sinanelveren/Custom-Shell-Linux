/**
 * Custom shell
 * Support the command: ls cat wc pwd help cd exit
 *
 * CSE344 System Programming HomeWork 3
 * Sinan Elveren - Gebze Technical University - Computer Engineering
 */
#define  _GNU_SOURCE //for getusername
#define _POSIX_SOURCE
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <fcntl.h>
#include <signal.h>
#include <error.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

//#define NDEBUG
#define COMMAND_BUFFER_SIZE 512
#define COMMAND_COUNT 100



int getCommandInf();
int pwd(int argc, char *argv[COMMAND_BUFFER_SIZE]);
int help();
int cd(char *argv[COMMAND_BUFFER_SIZE]);
int commandHistory(int argc, char *argv[COMMAND_BUFFER_SIZE], char history[COMMAND_COUNT][COMMAND_BUFFER_SIZE], int totalCommandCount);


int runCommand(char command[], char history[COMMAND_COUNT][COMMAND_BUFFER_SIZE], int totalCommandCount);
char cwd[1024];
char cwdFirst[1024];


pid_t   parentPID = 0;      //for check child pid

pid_t myWait(int *status) {
    pid_t rtrn;

    while (((rtrn = wait(status)) == -1) && (errno == EINTR));

    return rtrn;
}



void myAtexit(void){
    //childreen is coming
    if (getpid() != parentPID) {
        exit(EXIT_SUCCESS);
    } else {
        fprintf(stdout,"\n All child proccess is exited\n");
        fflush(stdout);
    }

    while (myWait(NULL ) > 0);
    fprintf(stdout,"\n    > [%d] Parent came here\n", getpid());
    fflush(stdout);

    fprintf(stdout,"\n    Process[%d] has been exit succesfuly\n", getpid());
    fflush(stdout);
}


// or exit funcs
void finish(int exitNum) {
    //  myAtexit();         //for wait children
    exit(exitNum);
}



void signalCatcher(int signum) {
    switch (signum) {
        case SIGUSR1: puts("\ncaught SIGUSR1");
            break;
        case SIGUSR2: puts("\ncaught SIGUSR2");
            break;
        case SIGINT:
            fprintf(stderr,"\n\nSIGINT:Ctrl+C signal detected, exit\n",stderr);
            finish(1);
        default:
            fprintf(stderr,"catcher caught unexpected signal %d\n", signum);
            finish(1);
    }
}



/* Save current stdout for use later */
//saved_stdout = dup(1);

int main(int argc, char *argv[]) {
    /* * * * * * ** * * * * * * * * * * * * * * * CHECK_USAGE * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    parentPID = getpid();
    if(atexit(myAtexit) !=0 ) {
        perror("atexit");
        return 1;
    }
    /* There are wrong argument count */
    if (argc != 1) {
        printf("\n'Too many/less paramters'\nUsage Error  : ./GTU_Shell\n");
        return 1;
    }


    /* * * * * * ** * * * * * * * * * * * * * * * DECLARATION * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    sigset_t sigset, oldMask, newMask;
    struct  sigaction sact;

    //save first working directory
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("Error getcwd");
        return -1;
    }
    if (getcwd(cwdFirst, sizeof(cwd)) == NULL) {
        perror("Error getcwd");
        return -1;
    }

    /**Signal**/
    sigemptyset(&oldMask);
    sigemptyset(&newMask);

    struct sigaction newact;
    newact.sa_handler = signalCatcher;
    /* set the new handler */

    newact.sa_flags = 0;

    /* no special options */
    if ((sigemptyset(&newact.sa_mask) == -1) || (sigaction(SIGINT, &newact, NULL) == -1))
        perror("Failed to install SIGINT signal handler");




    //get command - pars and run it
    getCommandInf();


}




/* * * * * * * * * * * * * * * * * * * * * * * * EN_OF_MAIN * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//get command and pars it
int getCommandInf() {
    char command[COMMAND_BUFFER_SIZE] = "";                  //= malloc(sizeof(char) * COMMAND_BUFFER_SIZE);
    char oneCommand[COMMAND_BUFFER_SIZE] = "";               // = malloc(sizeof(char) * COMMAND_BUFFER_SIZE);;
    char previousCommands[COMMAND_COUNT][COMMAND_BUFFER_SIZE]= {};
    char cwd[1024];
    char username[64];
    char shell[1024];
    int     isExit = 0;
    int     i = 0, j = 0,  k = 0, n = 0;
    char    ch = '\0';


    if(getlogin_r(username, sizeof(username)) != 0){
        perror("Error get username");
    }



    while(isExit == 0) {
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("Error getcwd");
            return -1;
        }
        strcpy(shell, username);
        strcat(shell, "@ :~");
        strcat(shell, cwd);
        strcat(shell, "$ ");

        fprintf(stdout, "%s", shell);           //info for show to user

        //read command char by char
        ch = getchar();
        for (i = 0; ch != '\n'; ++i) {
            command[i] = ch;

            //read command char by char
            ch = getchar();
        }
        command[i] = '\0';
        if (strlen(command))
            strcat(previousCommands[n++], command);




        //delim by pipe and run one command
        j = 0;
        for (i = 0; command[i] != '\0' ; ++i) {
            if(command[i] == '|') {
                oneCommand[j] = '\0';
                //exit if command is exit
                if (strcmp(oneCommand, "exit") == 0) {
                    return -1;  //exit
                }
                else if (strcmp(oneCommand, "help") == 0) {
                    help();
                }
                else
                    runCommand(oneCommand, previousCommands, n);
                j = 0;
            }else {
                oneCommand[j++] = command[i];
            }

        }
        oneCommand[j] = '\0';
        //exit if command is exit
        if (strcmp(oneCommand, "exit") == 0) {
            return -1;  //exit
        }
        else if (strcmp(oneCommand, "help") == 0) {
            help();
        }
        else
            isExit = runCommand(oneCommand, previousCommands, n);


#ifdef NDEBUG
        for (j = 0; j < n; ++j) {
            fprintf(stdout, "[%d] : %s\n", j+1, previousCommands[j]);
        }
#endif

    }
 //   free(command);
//    free(oneCommand);
    return 0;
}


//takes command and pars it as parameter and run it
int runCommand(char command[], char history[COMMAND_COUNT][COMMAND_BUFFER_SIZE], int totalCommandCount){
    pid_t   PID = 0;
    int     status = 0, n = 0;
    char    *args[COMMAND_BUFFER_SIZE];
    char    myCommand[COMMAND_BUFFER_SIZE];
    char    tempcwd[1024];




    args[n++] = strtok(command, " ");

    while (args[n-1] != NULL) {
        args[n++] = strtok (NULL, " ");
    }

    //to send to execvp -- custom command
    strcpy(myCommand, cwdFirst);     //first locaiion of current working directory
    strcat(myCommand, "/");



    if (n > 1 && strcmp(args[0], "cd") == 0) {
        if(n == 3){     //mean 2 arguements.. cd and path
            strcpy(myCommand, cwd);     //current working directory
            strcat(myCommand, "/");

            return cd(args);
        }else{
            perror("cd usage error: try 'cd PATH' or");
            return 0;       //continue get command
        }
    }


    // do it 2 proccoeses

    PID = fork();

    if (PID == -1) {
        perror("Error fork");
    }
    else if (PID == 0) {
        // Child process - run command
        strcat(myCommand, args[0]);

        if(strcmp(args[0], "pwd") == 0){
            status = pwd(n-1, args);
        }
        else if(strcmp(args[0], "history") == 0){
            status = commandHistory(n-1, args, history, totalCommandCount);
        }
        else{
            strcpy(tempcwd, cwd);
            strcat(tempcwd, "*");
            args[n-1] = strtok(tempcwd, "*");
            args[n] ='\0';

            status = execv(myCommand, args) == -1;
        }

        if(status == -1)
            perror("exec error - wrong command");

        //while(!myWait(&status));
        exit(EXIT_FAILURE);
    }
    else {
        // Parent process
        do {
            waitpid(PID, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 0; //return succesfuly
}



int pwd(int argc, char *argv[COMMAND_BUFFER_SIZE]){
    char cwd[1024];
    int  file = 1;


    fflush(stdout);


    if(argc == 3) {
        if (strcmp(argv[1], ">") == 0) {
            fflush(stdout);
            file = open(argv[2], O_CREAT | O_TRUNC | O_RDWR, 00644);

            if (file < 0) {
                perror("file open error");
                return -1;
            }
            close(STDOUT_FILENO);

            if (dup2(file, STDOUT_FILENO) < 0) {
                perror("dup2 error");
                return -1;
            }
        }
    } else if(argc != 1){
        perror("Command error: try 'pwd > fileName'");
        return -1;
    }

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("pwd: Error getcwd");
        return -1;
    }


    fprintf(stdout, "%s\n", cwd);

    close(file);

    return 1;
}



int help(){
    fprintf(stdout, "\tUSAGE PROGRAM: \t./GTU_Shel\n");
    fprintf(stdout, "\tUSAGE COMMAND: \tls\n");
    fprintf(stdout, "\t\t\tls > fileName\n");
    fprintf(stdout, "\t\t\tls > fileName > fileName2\n");
    fprintf(stdout, "\t\t\tls > fileName > fileName2 > ...\n\n");
    fprintf(stdout, "\tUSAGE COMMAND: \tpwd\n");
    fprintf(stdout, "\t\t\tpwd > fileName\n\n");
    fprintf(stdout, "\tUSAGE COMMAND: \texit\n\n");
    fprintf(stdout, "\tUSAGE COMMAND: \tcat fileName\n");
    fprintf(stdout, "\t\t\tcat fileName > fileName\n\n");
    fprintf(stdout, "\tUSAGE COMMAND: \tcd PATH\n");
    fprintf(stdout, "\t\t\tcd PATH > fileName\n\n");
    fprintf(stdout, "\tUSAGE COMMAND: \twc fileName\n");
    fprintf(stdout, "\t\t\twc fileName > outFileName\n\n");

    fprintf(stdout, "\tUSAGE COMMAND: \thistory\n");
    fprintf(stdout, "\t\t\thistory number\n\n");
    fprintf(stdout, "\t\t\tPreviously command from last to begin...\n\n");

    fprintf(stdout, "\tUSAGE PIPE:\n");
    fprintf(stdout, "\t\t\tCommand1 | Command2 > fileName | Command3 | ..\n\n");


    return 0;
}


int cd(char *argv[COMMAND_BUFFER_SIZE]){
    char cwdNew[1024];

    if (chdir(argv[1]) == -1){
        perror("Error getcwd");
        return 0;
    }

    if (getcwd(cwdNew, sizeof(cwd)) == NULL) {
        perror("Error getcwd");
        return -1;
    }

    // save current direcroty
    strcpy(cwd, cwdNew);

    return 0;
}



int commandHistory(int argc, char *argv[COMMAND_BUFFER_SIZE], char history[COMMAND_COUNT][COMMAND_BUFFER_SIZE],
                                                                                        int totalCommandCount){
    fprintf(stdout, "\nCOMMAND HISTORY\n");

    for (int i = 0; i < totalCommandCount; ++i)
        fprintf(stdout, "\t[%d] : %s\n", totalCommandCount - i , history[i]);


    if (argc == 1){
        fprintf(stdout,"\nUsage example : 'history 2'\n");
        return 0;
    }
    if(argc == 2){
        fprintf(stdout, "\nExcecuting %d. previous command (FROM LAST) \n", atoi(argv[1]));
        fprintf(stdout, "COMMAND '%s' \n\n", history[ totalCommandCount-atoi(argv[1]) -1 ]);

        runCommand(history[totalCommandCount-atoi(argv[1])-1], history, totalCommandCount+1);
        return 0;
    }
    if (argc > 2){
        perror("Error history usage : try 'history number");
        return 0;
    }

    return 0;
}



/* * * * * * ** * * * * * * * * * * * * * * * EN_OF_IMPLEMENTATION* * * * * * * * * * * * * * * * * * * * * * * * * * */
