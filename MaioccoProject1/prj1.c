#include "stdio.h"
#include "stdlib.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "dirent.h"
#include "string.h"
#include "unistd.h"
#include "pwd.h"
#include "grp.h"
#include "locale.h"
#include "langinfo.h"
//build a stack with linked lists
//don't need this for recursive
struct StackNode {
  char* data;
  struct StackNode* next;
};
struct StackNode* newNode(char* data) {
  struct StackNode* stackNode = (struct StackNode*)malloc(sizeof(struct StackNode));
  stackNode->data = data;
  stackNode->next = NULL;
  return stackNode;
}
int isEmpty(struct StackNode* root) {
  return !root;
}
void push(struct StackNode** root, char* data) {
  struct StackNode* stackNode = newNode(data);
  stackNode->next = *root;
  *root = stackNode;
  printf("%s pushed to stack\n", data);
}
char* pop(struct StackNode** root) {
  if (isEmpty(*root)) {
    return "Sweet empty list bro.";
  }
  struct StackNode* temp = *root;
  *root = (*root)->next;
  char* popped = temp->data;
  free(temp);

  return popped;
}
char* peek(struct StackNode* root) {
  if (isEmpty(root)) {
    return "Sweet empty list bro.";
  }
  return root->data;
}

//recursive directory traversal
void dtRecursive(char* dir, int indents, int flags[], int faSize) {

  //Initializing directory entry, stats
  struct dirent *dirEnt;
  struct stat dirStats;
  struct passwd *pwd;
  struct group *grp;
  struct tm *tm;
  char datestring[256];

  //open directory, error and exit if its not found
  DIR *dirName = opendir(dir);
  if (dirName == NULL) {
    perror("dt: Error: Specified directory not found");
    return;
  }

  //reading directories
  while ((dirEnt = readdir(dirName)) != NULL) {
    if (dirEnt->d_type == DT_DIR) {
      char buffer[512];
      if (strcmp(dirEnt->d_name, ".") == 0 || strcmp(dirEnt->d_name, "..") == 0) {
        continue;
      }
      snprintf(buffer, sizeof(buffer), "%s/%s", dir, dirEnt->d_name);
      stat(buffer, &dirStats);
      printf("%*s%s\t", indents, "", dirEnt->d_name);
          // L follow symbolic links
          // t print info on file type
          if (flags[6] || flags[10]) {
            switch (dirStats.st_mode &S_IFMT) {
              case S_IFDIR: printf("%16s", "d"); break;
              case S_IFREG: printf("%16s", "-"); break;
              default: printf("%16s", "");
            }
            // if (dirStats.st_mode == DT_DIR) printf("%16s", "d");
            // printf( (S_ISDIR(dirStats.st_mode)) ? "d" : " ");
          }

          // p pint permission bits
          if(flags[8] || flags[10]) {
            printf( (dirStats.st_mode & S_IRUSR) ? "r" : "-");
            printf( (dirStats.st_mode & S_IWUSR) ? "w" : "-");
            printf( (dirStats.st_mode & S_IXUSR) ? "x" : "-");
            printf( (dirStats.st_mode & S_IRGRP) ? "r" : "-");
            printf( (dirStats.st_mode & S_IWGRP) ? "w" : "-");
            printf( (dirStats.st_mode & S_IXGRP) ? "x" : "-");
            printf( (dirStats.st_mode & S_IROTH) ? "r" : "-");
            printf( (dirStats.st_mode & S_IWOTH) ? "w" : "-");
            printf( (dirStats.st_mode & S_IXOTH) ? "x" : "-");
          }

          // i print number of links to file in inode table
          if(flags[5]) {
            printf("%4d", dirStats.st_nlink);
          }
          ////:hI:Ldgipstul
          // u print uid
          if(flags[9] || flags[10]) {
            if ((pwd = getpwuid(dirStats.st_uid)) != NULL)
              printf(" %-8.8s", pwd->pw_name);
            else
              printf(" %-8d", dirStats.st_uid);
          }
          // g print gid
          if(flags[4] || flags[10]) {
            if ((grp = getgrgid(dirStats.st_gid)) != NULL)
              printf(" %-8.8s", grp->gr_name);
            else
              printf(" %-8d", dirStats.st_gid);
          }
          // s print file size in k, if a meg m, if a gig g
          if(flags[7] || flags[10]) {
            printf("%9jd", (intmax_t)dirStats.st_size);
          }
          // d time of last modification
          if(flags[3]) {
            printf(" %d", ctime(&dirStats.st_mtime));
          }
          // l does tpiugs
          printf("\n");
      dtRecursive(buffer, indents + 2, flags, faSize);
    }
    else {
      printf("%*s%s\t", indents, "", dirEnt->d_name);
          // L follow symbolic links
          // t print info on file type
          if (flags[6] || flags[10]) {
            switch (dirStats.st_mode &S_IFMT) {
              case S_IFDIR: printf("%16s", "d"); break;
              case S_IFREG: printf("%16s", "-"); break;
              default: printf("%16s", "");
            }
            // if (dirStats.st_mode == DT_DIR) printf("%16s", "d");
            // printf( (S_ISDIR(dirStats.st_mode)) ? "d" : " ");
          }

          // p pint permission bits
          if(flags[8] || flags[10]) {
            printf( (dirStats.st_mode & S_IRUSR) ? "r" : "-");
            printf( (dirStats.st_mode & S_IWUSR) ? "w" : "-");
            printf( (dirStats.st_mode & S_IXUSR) ? "x" : "-");
            printf( (dirStats.st_mode & S_IRGRP) ? "r" : "-");
            printf( (dirStats.st_mode & S_IWGRP) ? "w" : "-");
            printf( (dirStats.st_mode & S_IXGRP) ? "x" : "-");
            printf( (dirStats.st_mode & S_IROTH) ? "r" : "-");
            printf( (dirStats.st_mode & S_IWOTH) ? "w" : "-");
            printf( (dirStats.st_mode & S_IXOTH) ? "x" : "-");
          }

          // i print number of links to file in inode table
          if(flags[5]) {
            printf("%4d", dirStats.st_nlink);
          }
          ////:hI:Ldgipstul
          // u print uid
          if(flags[9] || flags[10]) {
            if ((pwd = getpwuid(dirStats.st_uid)) != NULL)
              printf(" %-8.8s", pwd->pw_name);
            else
              printf(" %-8d", dirStats.st_uid);
          }
          // g print gid
          if(flags[4] || flags[10]) {
            if ((grp = getgrgid(dirStats.st_gid)) != NULL)
              printf(" %-8.8s", grp->gr_name);
            else
              printf(" %-8d", dirStats.st_gid);
          }
          // s print file size in k, if a meg m, if a gig g
          if(flags[7] || flags[10]) {
            printf("%9jd", (intmax_t)dirStats.st_size);
          }
          // d time of last modification
          if(flags[3]) {
            printf(" %d", ctime(&dirStats.st_mtime));
          }
          // l does tpiugs
      printf("\n");
    }
  }
  closedir(dirName);
}

int main(int argc, char* argv[]) {

  extern char *optarg;
  extern int optind;
  int opt, err = 0;
  int hFlag=0, bigIFlag=0, bigLFlag=0, dFlag=0, gFlag=0,
      iFlag=0, pFlag=0, sFlag=0, tFlag=0, uFlag=0, lFlag=0;
  int indents = 0;

  while((opt = getopt(argc, argv, ":hI:Ldgipstul")) != -1) {
    switch(opt) {
      case 'h':
        hFlag=1;
        printf("dt: a nice program to depth first traverse a directory.\n"
                "all the flags will go here too.\n");
        return 0;
      case 'I':
        bigIFlag=1;
        break;
      case 'L':
        bigLFlag=1;
        break;
      case 'd':
        dFlag=1;
        break;
      case 'g':
        gFlag=1;
        break;
      case 'i':
        iFlag=1;
        break;
      case 'p':
        pFlag=1;
        break;
      //:hI:Ldgipstul
      case 's':
        sFlag=1;
        break;
      case 't':
        tFlag=1;
        break;
      case 'u':
        uFlag=1;
        break;
      case 'l':
        lFlag=1;
        break;
    }
  }

  int flagArray[] = { hFlag, bigIFlag, bigLFlag, dFlag, gFlag,
                      iFlag, pFlag, sFlag, tFlag, uFlag, lFlag };
  int faSize = sizeof(flagArray)/sizeof(flagArray[0]);

  for(; optind < argc; optind++) {
    printf("extra arguments: %s\n", argv[optind]);
  }

  dtRecursive(".", indents, flagArray, faSize);

  return 0;
}
