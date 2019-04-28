/**
 * cat command for Custom shell
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


int catPrint(const char *fileName);
//int catWrite(const char *fileName, const char *outFileName);


int main(int argc, char *argv[]) {
    /* * * * * * ** * * * * * * * * * * * * * * * CHECK_USAGE * * * * * * * * * * * * * * * * * * * * * * * * * * * * */



    /* * * * * * ** * * * * * * * * * * * * * * * DECLARATION * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    int file = 0;
    char filePath[64];
    char outFilePath[64];


    //restore current direcory
    if (chdir(argv[argc-1]) == -1){
        perror("Error getcwd");
        return 0;
    }


    if(argc == 2){      // if cat without parameter
        return 0;
    }
    if(argc == 3){      //if cat with one parameter as filename
        return catPrint(argv[1]);

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

        return catPrint(argv[1]);
        //return catWrite(argv[1], argv[3]);
    }
    else{
        perror("Error usage : cat fileName1 > fileName2");
        return -1;
    }

}




/* * * * * * * * * * * * * * * * * * * * * * * * EN_OF_MAIN * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//"cat filename"  print file content
int catPrint(const char *fileName) {
    long fsize = 0;
    char *content = NULL;
    FILE *fp =NULL;

    fp = fopen(fileName, "rb");

    if ( fp == NULL ) {
        perror( "Could not open file" ) ;
        return -1;
    }


    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    content = malloc(fsize + 1);

    fread(content, fsize, 1, fp);
    fclose(fp);

    //print filecontent
    fprintf(stdout, "%s", content);

    free(content);
    return 0;
}

/*

//"cat filename > filename" write file content to file
int catWrite(const char *fileName, const char *outFileName) {
    FILE *fp = fopen(fileName, "rb");
    FILE *fpOut = fopen(outFileName, "w");

    long fsize = 0;

    if ( fp == NULL || fpOut == NULL ) {
        perror( "Could not open file" ) ;
        return -1;
    }


    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *content = malloc(fsize + 1);

    fread(content, fsize, 1, fp);
    fclose(fp);

    //print filecontent
    fseek(fpOut, 0, SEEK_SET);
    fwrite(content, fsize, 1, fpOut);

    fclose(fpOut);
    return 0;
}

*/



/* * * * * * ** * * * * * * * * * * * * * * * EN_OF_IMPLEMENTATION* * * * * * * * * * * * * * * * * * * * * * * * * * */
