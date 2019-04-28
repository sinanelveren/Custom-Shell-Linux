/**
 * ls command for Custom shell
 * Support the command: ls cat wc pwd help cd exit
 *
 * CSE344 System Programming HomeWork 3
 * Sinan Elveren - Gebze Technical University - Computer Engineering
 */
#define _GNU_SOURCE     //macro --std=gnu99 for lstat mode's macro
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>


#define NDEBUG


int printDir(const char *path);
//int writeDir(const char *path, const char *fileName);


int main(int argc, char *argv[]) {
    /* * * * * * ** * * * * * * * * * * * * * * * CHECK_USAGE * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    int i = 0;
    int file = 0;

    /* * * * * * ** * * * * * * * * * * * * * * * DECLARATION * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    char cwd[1024];
/*
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("Error getcwd");
        return -1;
    }*/

    //current working directory
    strcpy(cwd, argv[argc-1]);

    //ls without any argument
    if(argc == 2){
        return printDir(cwd);
    }
    //list with arguments
    else if(argc > 2){
        while(argv[++i] !=  NULL){
            if(strcmp(argv[i], ">") != 0) {
                perror("wrong command : usage is 'ls > fileName'");
                return -1;
            }
            else{
                file = open(argv[++i], O_CREAT | O_TRUNC | O_RDWR, 00644);
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
                return printDir(cwd);
                // return writeDir(cwd, argv[++i]);
            }
        }

    }


    return 0;
}




/* * * * * * * * * * * * * * * * * * * * * * * * EN_OF_MAIN * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int printDir(const char *path) {
    struct  dirent  *entry;
    struct  stat    fileStat;
    DIR     *dp;


    //open directory given path
    dp = opendir(path);
    if (dp == NULL) {
        perror("Error open dir");
        return -1;
    }



    //read all files in this location
    while((entry = readdir(dp))) {
        if(lstat(entry->d_name,&fileStat) < 0){
            perror("Error file stat");
            return 1;
        }

        //print file type
        switch (fileStat.st_mode & S_IFMT) {
            case S_IFREG:   fprintf(stdout, "Regular file.....\t"); break;
            case S_IFDIR:   fprintf(stdout, "Directory........\t"); break;
            case S_IFCHR:   fprintf(stdout, "Character Device.\t"); break;
            case S_IFBLK:   fprintf(stdout, "Block Device.....\t"); break;
            case S_IFLNK:   fprintf(stdout, "Symbolic Link....\t"); break;
            case S_IFIFO:   fprintf(stdout, "Pipe File........\t"); break;
            case S_IFSOCK:  fprintf(stdout, "Socket File......\t"); break;
            default:        fprintf(stdout, "Unknown File.....\t"); break;
        }

        //print access rights
        fprintf(stdout, (fileStat.st_mode & S_IRUSR) ? "r" : "-");
        fprintf(stdout, (fileStat.st_mode & S_IWUSR) ? "w" : "-");
        fprintf(stdout, (fileStat.st_mode & S_IXUSR) ? "x" : "-");
        fprintf(stdout, (fileStat.st_mode & S_IRGRP) ? "r" : "-");
        fprintf(stdout, (fileStat.st_mode & S_IWGRP) ? "w" : "-");
        fprintf(stdout, (fileStat.st_mode & S_IXGRP) ? "x" : "-");
        fprintf(stdout, (fileStat.st_mode & S_IROTH) ? "r" : "-");
        fprintf(stdout, (fileStat.st_mode & S_IWOTH) ? "w" : "-");
        fprintf(stdout, (fileStat.st_mode & S_IXOTH) ? "x\t" : "-\t");

        //print file size
        fprintf(stdout,"%d bytes\t", fileStat.st_size);

        //print file name
        fprintf(stdout, "%s\n",entry->d_name);
    }

    closedir(dp);
    return 0;
}
/*


//write files's info to file
int writeDir(const char *path, const char *fileName){
    struct  dirent  *entry;
    struct  stat    fileStat;
    DIR     *dp;
    FILE    *fp;

    fp = fopen(fileName, "w");

    if ( fp == NULL ) {
        perror( "Could not open file" ) ;
        return -1;
    }

    //open directory given path
    dp = opendir(path);
    if (dp == NULL) {
        perror("Error open dir");
        return -1;
    }



    //read all files in this location
    while((entry = readdir(dp))) {
        if(lstat(entry->d_name,&fileStat) < 0){
            perror("Error file stat");
            return 1;
        }

        //write file type
        switch (fileStat.st_mode & S_IFMT) {
            case S_IFREG:   fprintf(fp, "Regular file.....\t"); break;
            case S_IFDIR:   fprintf(fp, "Directory........\t"); break;
            case S_IFCHR:   fprintf(fp, "Character Device.\t"); break;
            case S_IFBLK:   fprintf(fp, "Block Device.....\t"); break;
            case S_IFLNK:   fprintf(fp, "Symbolic Link....\t"); break;
            case S_IFIFO:   fprintf(fp, "Pipe File........\t"); break;
            case S_IFSOCK:  fprintf(fp, "Socket File......\t"); break;
            default:        fprintf(fp, "Unknown File.....\t"); break;
        }

        //write access rights
        fprintf(fp, (fileStat.st_mode & S_IRUSR) ? "r" : "-");
        fprintf(fp, (fileStat.st_mode & S_IWUSR) ? "w" : "-");
        fprintf(fp, (fileStat.st_mode & S_IRGRP) ? "r" : "-");
        fprintf(fp, (fileStat.st_mode & S_IWGRP) ? "w" : "-");
        fprintf(fp, (fileStat.st_mode & S_IXGRP) ? "x" : "-");
        fprintf(fp, (fileStat.st_mode & S_IXUSR) ? "x" : "-");
        fprintf(fp, (fileStat.st_mode & S_IROTH) ? "r" : "-");
        fprintf(fp, (fileStat.st_mode & S_IWOTH) ? "w" : "-");
        fprintf(fp, (fileStat.st_mode & S_IXOTH) ? "x\t" : "-\t");
        //print file size
        fprintf(fp,"%d bytes\t", fileStat.st_size);

        //write file name
        fprintf(fp, "%s\n",entry->d_name);
    }

    closedir(dp);
    fclose(fp);
    return 0;
}

*/


/* * * * * * ** * * * * * * * * * * * * * * * EN_OF_IMPLEMENTATION* * * * * * * * * * * * * * * * * * * * * * * * * * */
