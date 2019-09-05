#include "stdio.h"
#include "stdlib.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "dirent.h"
#include "string.h"

//build a stack with linked lists
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

  dtRecursive(argv[1], 2);

  return 0;

}
