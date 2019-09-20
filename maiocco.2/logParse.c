#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

char eq[128];

int getSeconds() {
  time_t curTime;
  time(&curTime);
  struct tm *local = localtime(&curTime);
  int seconds = local->tm_sec;
  return seconds;
}

//from geeksforgeeks
bool isSubsetSum(int set[], int n, int sum, char* eq, int curTime) {
    if( getSeconds() - curTime == 1) {
      printf("%d: ", getpid());
      perror("logParse: Timeout on subset sum.\n");
    }
    if(sum==0) {
      return true;
    }
    if(n==0 && sum !=0){
      return false;
    }
    if(set[n-1] > sum) {
      return isSubsetSum(set, n-1, sum, eq, curTime);
    }
    if(isSubsetSum(set, n-1, sum, eq, curTime) == true) {
      return true;
    }
    else if (isSubsetSum(set, n-1, sum-set[n-1], eq, curTime)) {
      char* temp;
      sprintf(temp, "%d", set[n-1]);
      strcat(eq, temp);
      strcat(eq, " + ");
      //printf("%s\n", eq);
      return true;
    }
    else return false;
}

int main(int argc, char* argv[]) {

  //initialize getopt vars
  extern char *optarg;
  extern int optind;
  int opt, err = 0;
  int hFlag = 0, iFlag = 0,
      oFlag = 0, tFlag = 0;

  //getopt stuff
  while((opt = getopt(argc, argv, "hi:o:t")) != -1) {

    switch(opt) {
      case 'h':
        hFlag=1;
        printf("usage goes here\n");
        return 0;
      case 'i':
        iFlag=1;
        break;
      case 'o':
        oFlag=1;
        break;
      case 't':
        tFlag=1;
        break;
    }
  }

  for (; optind < argc; optind++) {
    printf("extra arguments: %s\n", argv[optind]);
  }

  //file pointer initialization/opening
  FILE *fp;
  char *filename = "input.dat";
  fp = fopen(filename, "r");
  if (!fp) {
    perror("logParse: Error: File not found.\n");
  }

  //read strings from file into array
  char lineList[32][127];
  int i = 0;
  while(fgets(lineList[i], sizeof(lineList[i]), fp)) {
    lineList[i][strlen(lineList[i]) - 1] = '\0';
    i++;
  }

  fclose(fp);

  //number of subset sum problems in file
  int nLines = atoi(lineList[0]);

  //get number of ints in each array string.
  for(i=1; i<nLines+1; i++) {

    //get length of string from file
    int len = 0;
    int j;
    for (j=0; lineList[i][j]!='\0'; j++) {
      //printf("%c", lineList[i][j]);
      if (isspace(lineList[i][j])) {
        len++;
      }
    }
    //printf("len = %d\n", len+1);
    int numSet[len];
    //set numSets vals to 0
    for (j=0; j<len+1; j++) {
      numSet[j] = 0;
    }

    //fill numSet with int values from list
    int k=0;
    for(j=0; lineList[i][j]!='\0'; j++) {
      if (isspace(lineList[i][j])) {
        k++;
      }
      else {
        numSet[k] = numSet[k]*10+(lineList[i][j]-48);
      }
    }

    // printf("---NUMSET---\n");
    // for (j=0; j<len+1; j++) {
    //   printf("%d ", numSet[j]);
    // }
    // printf("\n");

    //now we have an array. we can do the subset stuff now.
    int wstatus;
    pid_t pid = fork(), waitStat;

    if(pid == -1) {
      perror("logParse: Error: Child process broke. Sorry.\n");
    }
    else if(pid > 0) {
      //printf("I'm parent\n");
      do {
        waitStat = waitpid(pid, &wstatus, WUNTRACED | WCONTINUED);
        if (waitStat == -1) break;
      } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
    }
    else{ //Now we do the child stuff... the subset sum
      int sumSet[len-1];
      int q;
      //printf("---SETTING---\n");
      for (q = 1 ; q<sizeof(numSet)/sizeof(numSet[0])+1; q++) {
        sumSet[q-1] = numSet[q];
        //printf("%d ", numSet[q]);
      }
      // printf("\n");
      // printf("---SUMSET---\n");
      // for (q = 0; q<sizeof(sumSet)/sizeof(sumSet[0])+1;q++){
      //   printf("%d ", sumSet[q]);
      // }
      // printf("\n");
      //char eq[128];
      printf("NUMSET 0 = %d\n", numSet[0]);
      int rightNow;
      int total = numSet[0];
      printf("TOTAL =  %d\n", total);
      if (isSubsetSum(sumSet, sizeof(sumSet)/sizeof(sumSet[0]), numSet[0], eq, rightNow = getSeconds())) {
        printf("%s\n", eq);
        eq[strlen(eq)-2] = '=';
        printf("%d: ", getpid());
        printf("%s", eq);
        printf("%d\n", total);
      }
      else printf("%d: No list of numbers summed to %d.\n", getpid(), numSet[0]);
      exit(0);
    }

  }
  // debugging
  // int tot = nLines+1;
	// printf("\nThe content of the file %s are: \n", filename);
  // for(i = 0; i < tot; ++i)
  // {
  //     printf("%s \n", lineList[i]);
  // }
  // printf("\n");


}
