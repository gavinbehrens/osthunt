/*
 *
 *
*/

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
#include <stdlib.h>


int delta;

void deletefile (char myfile[255])
{
    remove(myfile);
    printf ("***removed***\n");
}

void checkfile(char myfile[255])  //checks to see if the file modified time is more than 30 days

{
    time_t currenttime;
    struct stat attr;

    stat(myfile, &attr);
    currenttime = time(NULL);
    printf("file : %s - ",myfile);

    if (currenttime - attr.st_mtime > delta )
    {
        deletefile(myfile);
    }else
    {
        printf  ("keeping\n");
    }
    //printf ("%s\n%ld  %s \n",myfile,currenttime, );
}

void userdir(char path[255])
{
    char filetype[5] = ".ost";
    struct dirent *dp;
    DIR *dir = opendir(path);
    char fpath[255];
    strcpy (fpath, path);  //save original path

    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0 )
        {
            strcat(path, dp->d_name);
            if (strstr(path,filetype) != NULL )
            {
                checkfile(path);
            }
        strcpy (path,fpath);  //return to original
        }
    }
    // Close directory stream
    closedir(dir);
}

void listusers(char path[255])
{
    struct dirent *dp;
    DIR *dir = opendir(path);
    char fpath[255];
    strcpy (fpath, path);  //save original path

    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0 && strcmp(dp->d_name, "desktop.ini") != 0 \
            && strcmp(dp->d_name, "All Users") != 0 && strcmp(dp->d_name, "Public") != 0 && strcmp(dp->d_name, "Default") != 0 \
            && strcmp(dp->d_name, "Default User") != 0)
        {
            strcat(path, dp->d_name);
            strcat(path,"\\appdata\\local\\microsoft\\outlook\\");
            userdir(path);
            strcpy (path,fpath);  //return to original
         }
    }
    closedir(dir);
}

int main(int argc, char *argv[])
{
    char path[255];

    printf ("\nosthunt - Finds ost files on specified drive and deletes files older than X days\n\n\n");

    if (argc < 3)
    {
        printf ("Expecting 2 arguments: <drive> <number of days>\n");
        printf ("Example:-  \"osthunt.exe c: 90\"\n\n");
        return 0;
    }

    strcpy (path,argv[1]);
    delta = (atoi(argv[2]) * 24 *60 * 60);
    if (delta == 0)
    {
        delta = 86400;
        printf ("Minimum is 1 day, changing value.\n");
    }

    strcat(path,"\\users\\");
    listusers(path);
    return 0;
}






