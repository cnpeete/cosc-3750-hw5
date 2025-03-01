/*
wyls.c
Author: Cameron Peete
Date: February 29, 2025

COSC 3750, Homework 5 Program

This file contains the source code for Homework 5
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

struct dirent *dp;
struct stat statbuf;
struct passwd *pwd;
struct group *grp;
struct tm *tm;
char date[256];

void printOutput(char *input, bool nFlag, bool hFlag);

char *perms(mode_t perm);


int main(int argc, char **argv)
{
  int count;
  bool nFlag = false;
  bool hFlag = false;
  count = 1;

  if (argc == 1)
  {
    printOutput(argv[0], nFlag, hFlag);
  }
  else
  {
    //only check argv[1] if it exists
    if (argc > 1)
    {
      if (strcmp(argv[1], "-n") == 0 || strcmp(argv[1], "-n -h") == 0 ||
          strcmp(argv[1], "-nh") == 0 || strcmp(argv[1], "-hn") == 0)
      {
        nFlag = true;
        printf("nFlag: %d\n", nFlag);
      }
      if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-h -n") == 0 ||
          strcmp(argv[1], "-hn") == 0 || strcmp(argv[1], "-nh") == 0)
      {
        hFlag = true;
        printf("hFlag: %d\n", hFlag);
      }
      //if the first argument triggers neither of thse flags, then maybe we should do the while loop
      //and call printOutput()
      if (!nFlag && !hFlag)
      {
        count = 1;
        while (count < argc)
        {
          printOutput(argv[count], nFlag, hFlag);
          count = count+1;
        }
      }
    }
    //we only want to check the 2nd item for a flag if the first entry did not set one of the flags
    //otherwise we want to call printOutput function
    if (nFlag || hFlag)
    {
      //only check argv[2] if it exists
      if (argc > 2)
      {
        if (strcmp(argv[2], "-n") == 0 || strcmp(argv[2], "-n -h") == 0 ||
            strcmp(argv[2], "-nh") == 0 || strcmp(argv[2], "-hn") == 0)
        {
          nFlag = true;
          printf("nFlag: %d\n", nFlag);
        }
        if (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "-h -n") == 0 ||
            strcmp(argv[2], "-hn") == 0 || strcmp(argv[2], "-nh") == 0)
        {
          hFlag = true;
          printf("hFlag: %d\n", hFlag);
        }
        //put the while loop here since we have evaluated for both flags
        //we know nFlag or hFlag are true
        //call the printOutput() function and pass it the values
        if (nFlag && hFlag)
        {
          count = 3; //have to account for both flags being set
          //while (count < argc)
        }
      }
    }
  }

  return 0;
}

void printOutput(char *input, bool nFlag, bool hFlag)
{
    //if the stat function errors
    if (stat(input, &statbuf) != 0)
    {
      perror("error");
      return;
    }

    if (S_ISDIR(statbuf.st_mode))
    {
      DIR *dir = opendir(input);
      if (dir == NULL)
      {
        perror("\nerror");
      }
      else
      {
        while ((dp=readdir(dir)) != NULL)
        {
          //if the directory name is '.' or '..' skip it
          if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) continue;

          char path[1024];
          snprintf(path, 1024, "%s%s", input, dp->d_name);

          //if (stat(path, &statbuf) == -1) perror("\nerror");
          printf("%10.10s", perms(statbuf.st_mode));

          if ((pwd = getpwuid(statbuf.st_uid)) != NULL)
          {
            printf(" %-8.8s", pwd->pw_name);
          }
          else
          {
            printf(" %-8d", statbuf.st_uid);
          }
          if ((grp = getgrgid(statbuf.st_gid)) != NULL)
          {
            printf(" %8.8s", grp->gr_name);
          }
          else
          {
            printf(" %-8d", statbuf.st_gid);
          }
          printf(" %9jd", (intmax_t)statbuf.st_size);

          tm = localtime(&statbuf.st_mtime);
          strftime(date, 256, nl_langinfo(D_T_FMT), tm);
          printf(" %s %s\n", date, dp->d_name);
        }
      }
    }
}

char *perms(mode_t perm)
{
  static char permissions[11];
  permissions[0] = S_ISDIR(perm) ? 'd' :'-';
  permissions[1] = (perm & S_IRUSR) ? 'r' : '-';
  permissions[2] = (perm & S_IWUSR) ? 'w' : '-';
  permissions[3] = (perm & S_IXUSR) ? 'x' : '-';
  permissions[4] = (perm & S_IRGRP) ? 'r' : '-';
  permissions[5] = (perm & S_IWGRP) ? 'w' : '-';
  permissions[6] = (perm & S_IXGRP) ? 'x' : '-';
  permissions[7] = (perm & S_IROTH) ? 'r' : '-';
  permissions[8] = (perm & S_IWOTH) ? 'w' : '-';
  permissions[9] = (perm & S_IXOTH) ? 'x' : '-';
  permissions[10] = '\0';

  return permissions;
}
