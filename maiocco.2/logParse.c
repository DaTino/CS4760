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

//from geeksforgeeks
bool isSubsetSum(int set[], int n, int sum) {
    // The value of subset[i][j] will be true if there is a
    // subset of set[0..j-1] with sum equal to i
    bool subset[n+1][sum+1];
    // If sum is 0, then answer is true
    for (int i = 0; i <= n; i++)
      subset[i][0] = true;
    // If sum is not 0 and set is empty, then answer is false
    for (int i = 1; i <= sum; i++)
      subset[0][i] = false;
     // Fill the subset table in botton up manner
     for (int i = 1; i <= n; i++) {
       for (int j = 1; j <= sum; j++) {
         if(j<set[i-1]) {
           subset[i][j] = subset[i-1][j];
         }
         if (j >= set[i-1]) {
           subset[i][j] = subset[i-1][j] ||  subset[i - 1][j-set[i-1]];
         }
       }
     }
     /*   // uncomment this code to print table
     for (int i = 0; i <= n; i++)
     {
       for (int j = 0; j <= sum; j++)
          printf ("%4d", subset[i][j]);
       printf("\n");
     }*/
     return subset[n][sum];
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
    int pid = fork();

    if(pid == -1) {
      perror("logParse: Error: Child process broke. Sorry.\n");
    }
    else if(pid > 0) {
      //printf("I'm parent\n");
    }
    else{ //Now we do the child stuff... the subset sum
      int sumSet[len-1];
      int q;
      printf("---SETTING---\n");
      for (q = 1 ; q<sizeof(numSet)/sizeof(numSet[0])+1; q++) {
        sumSet[q-1] = numSet[q];
        printf("%d ", numSet[q]);
      }
      printf("\n");
      printf("---SUMSET---\n");
      for (q = 0; q<sizeof(sumSet)/sizeof(sumSet[0])+1;q++){
        printf("%d ", sumSet[q]);
      }
      printf("\n");
      if (isSubsetSum(sumSet, sizeof(sumSet)/sizeof(sumSet[0]), numSet[0]))
        printf("%d: FUCK YEAH SUMSET\n", getpid());
      else printf("%d: GODDAMMIT DIDNT WORK\n", getpid());
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
