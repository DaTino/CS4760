#include "stdio.h"
#include "stdlib.h"

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

int main() {

  struct StackNode* root = NULL;

  push(&root, "butt");
  push(&root, "shit");
  push(&root, "piss");

  printf("%s popped from stack\n", pop(&root));
  printf("Top element is %s\n", peek(root));

}
