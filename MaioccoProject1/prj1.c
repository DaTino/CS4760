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

int main(int argc, char* argv[]) {
  //set up stack
  struct StackNode* root = NULL;
  // set up directory entry
  struct dirent *de;

  char buffer[512];
  struct stat mystat;

  //open directory
  DIR *dr = opendir(".");

  //sweet ass error handling
  if (dr == NULL) {
    printf("Could not open current directory");
    return 0;
  }

  //push everything into the stack
  while ((de = readdir(dr)) != NULL) {
    sprintf(buffer, "%s/%s", argv[1], de->d_name);
    stat(buffer, &mystat);
    push(&root, de->d_name);
  }

  printf("Top element is %s\n", peek(root));

  closedir(dr);
  return 0;

}
