#include "stdio.h"
#include "stdlib.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "dirent.h"
#include "string.h"
#include "unistd.h"

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
void dtRecursive(char* dir, int indents) {
  //Initializing directory entry, stats
  struct dirent *dirEnt;
  struct stat dirStats;

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
      printf("%d", dirStats.st_size);
      printf("%*s%s\n", indents, "", dirEnt->d_name);
      dtRecursive(buffer, indents + 2);
    }
    else {
      printf("%d", dirStats.st_size);
      printf("%*s%s\n", indents, "", dirEnt->d_name);
    }
  }
  closedir(dirName);
}

int main(int argc, char* argv[]) {

  extern char *optarg;
  extern int optind;
  int opt, err = 0;
  int hFlag=0, bigIFlag=0, bigLFlag=0, dFlag=0, gFlag=0,
      pFlag=0, sFlag=0, tflag=0, uFlag=0, lFlag=0;
  int flagArray[] = { hFlag, bigIFlag, bigLFlag, dFlag, gFlag,
      pFlag, sFlag, tflag, uFlag, lFlag };
  int indents = 0;

  while((opt = getopt(argc, argv, ":hI:Ldgipstul")) != -1) {
    switch(opt) {
      case 'h':
        hflag=1;
        printf("dt: a nice program to depth first traverse a directory.\n
                all the flags will go here too.\n");
        return;
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
      case 'u':
        uFlag=1;
      case 'l':
        lFlag=1;
    }
  }

  for(; optind < argc; optind++) {
    printf("extra arguments: %s\n", argv[optind]);
  }

  printf("Flags:");
  int i;
  for(i=0; i<sizeof(flagArray)/sizeof(flagArray[0]); i++) {
    printf("%d ", flagArray[i]);
  }

  dtRecursive(".", indents);

  return 0;

}
