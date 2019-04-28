/**
 * wc command for Custom shell
 * Support the command: ls cat wc pwd help cd exit
 *
 * CSE344 System Programming HomeWork 3
 * Sinan Elveren - Gebze Technical University - Computer Engineering
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>


#define NDEBUG


int wcPrint(const char *fileName);
//int catWrite(const char *fileName, const char *outFileName);


int main(int argc, char *argv[]) {
    /* * * * * * ** * * * * * * * * * * * * * * * CHECK_USAGE * * * * * * * * * * * * * * * * * * * * * * * * * * * * */



    /* * * * * * ** * * * * * * * * * * * * * * * DECLARATION * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    int file = 0;



    //restore current direcory
    if (chdir(argv[argc-1]) == -1){
        perror("Error getcwd");
        return 0;
    }

    if(argc == 2){      // wc without parameter
        return 0;
    }
    if(argc == 3){      // wc with one parameter
        return wcPrint(argv[1]);

    }
    else if(argc == 5 && strcmp(argv[2], ">") == 0){
        file = open(argv[3], O_CREAT | O_TRUNC | O_RDWR, 00644);
        close(STDOUT_FILENO);
        if(file < 0) {
            perror("file open error");
            return -1;
        }
        if(dup2(file, STDOUT_FILENO) < 0) {
            perror("dup2 error");
            return -1;
        }
        close(file);

        return wcPrint(argv[1]);
    }
    else{
        perror("Error usage : wc fileName1 > outFileName");
        return -1;
    }

}




/* * * * * * * * * * * * * * * * * * * * * * * * EN_OF_MAIN * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//"cat filename"  print file content
int wcPrint(const char *fileName) {
    long fsize = 0;
    char *content = NULL;
    int wCount = 0;
    FILE *fp =NULL;

    fp = fopen(fileName, "rb");

    if ( fp == NULL ) {
        perror( "Could not open file" ) ;
        return -1;
    }


    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    //allocate
    content = malloc(fsize + 1);

    fread(content, fsize, 1, fp);
    fclose(fp);


    for (int i = 0; content[i] != '\0'; ++i) {
        if ( content[i] == '\n' )
            wCount++;
    }

    //print filecontent
    fprintf(stdout, "%d  %s\n",wCount, fileName);

    free(content);
    return 0;
}




/* * * * * * ** * * * * * * * * * * * * * * * EN_OF_IMPLEMENTATION* * * * * * * * * * * * * * * * * * * * * * * * * * */
