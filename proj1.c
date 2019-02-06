#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

//Function that takes in a string and tells you how many numbers is in it//
int stringCounter(char *str){
   int i, stringCount = 0;
   for (i = 0; i < strlen(str); i++){
      char p = str[i];
      if(p != ' '){
         if(str[i+1] == ' ' || str[i+1] == '\n'){
	    stringCount++;
	 }
	 else if(p == '\n'){
	    break;
	 }
	 else{
	    continue;
	 }
      }
   }
   return stringCount;
}

//Stack Structure taken from geeksforgeeks.org//
struct Stack {
  int top;
  unsigned capacity;
  int* array;
};

//function to create a stack of given capacity. It initializes size of stack as 0
struct Stack* createStack(unsigned capacity)
{
  struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
  stack->capacity = capacity;
  stack->top = -1;
  stack->array = (int*)malloc(stack->capacity * sizeof(int));
  return stack;
}
//Stack is full when top is equal to last index
int isFull(struct Stack* stack) 
{ 
    return stack->top == stack->capacity - 1; 
}
//Stack is empty when top is equal to -1
int isEmpty(struct Stack* stack) 
{ 
    return stack->top == -1; 
}
//Function to add an item to stack, It increases top by 1
void push(struct Stack* stack, int item) 
{ 
    if (isFull(stack)) 
            return; 
    stack->array[++stack->top] = item; 
}
//function to remove an item from stack. It decreases top by 1.
int pop(struct Stack* stack) 
{ 
    if (isEmpty(stack)) 
        return INT_MIN; 
    return stack->array[stack->top--]; 
}
int main(int argc, char **argv){
  int c;
  char *defaultInputFileName = "input.dat", *inputFileName;
  char *defaultOutputFileName = "output.dat", *outputFileName;
  int iflag=0, oflag=0;

  /*char  *errorMessage = argv[0];
  strncat(errorMessage, ":Error", 7);
  printf("%s\n", errorMessage);
  */

  //Grabs command line arguments and based on case, does something//
  while((c = getopt(argc, argv, "hi:o:")) != -1)
     switch(c)
     {
     case 'h':       //Hint option//
        printf("Legal Command line options: -h, -i, -o\n");
	printf("-i is the input file and should have an input file name afterwards\n");
	printf("-o is the output file and should have an output file name afterwards\n");
	printf("If either input file name or output file name is not specified, the defaults input.dat and output.dat will be used respectively.\n");
	printf("**If -h is used, all other options will be ignored.\n");
	return 0;

     case 'i':       //Input file, next argument is the new inputFileName, sets iflag to true;
     	inputFileName = optarg;
	iflag = 1;
	break;
     case 'o':       //output file, next arugment is new OutputFileName, sets oflag to true;
	outputFileName = optarg;
	oflag = 1;
	break;
     case '?':
       	//perror(errorMessage);
	return 0;
	break;
     default:
	abort();
     }

     //BEGIN FILE PROCESSING//
     FILE *fp;
     
     //if input was not changed, use the default name which is input.dat//
     if(iflag == 0){         
       fp = fopen(defaultInputFileName, "r");
     }
     //if input was changed, used the new input file name.
     else{
       fp = fopen(inputFileName, "r");
     }

     //checks if the file exists
     if(fp == NULL){
        perror("Error:");
	return(-1);
     }

     //Grabs the first line in the file.
     char str[100];
     int stringCount = 0;
     fgets(str, 100, fp);
     printf("%s", str);
     stringCount = stringCounter(str);
     if(stringCount < 1){
        printf("Error:no number found");
	return(-1);
     }
     else if(stringCount > 1){
        printf("too many numbers");
	return(-1);
     }
     int forkCount=0;
     forkCount = atoi(str); 
     
     //Child Processes in a loop//
       pid_t child_pid[forkCount], wpid;
       int status =0;
       int origStack[8];
       int numOfNumsFake;
       int i;
     for (i = 0; i < forkCount; i++){
       if(i > 0){
          sleep(1*i);
       }
       if ((child_pid[i] = fork()) == 0) {
          FILE *fo;
	  if(oflag == 0){
	    fo = fopen(defaultOutputFileName, "a");
	  }
	  else{
	    fo = fopen(outputFileName, "a");
	  }
	  fgets(str, 100, fp);
          int stringCount = stringCounter(str);
	  if(stringCount < 1){
	     printf("Error2:No number found\n");
	     return(-1);
	  }
	  else if(stringCount > 1){
	     printf("Error2: Too many numbers!\n");
	     return(-1);
	  }
	  int numOfNums = atoi(str);
	  int k;
	  int helpStack[100];
          char newStr[100];
	  fgets(newStr, 100, fp);
	  int newStrCount = stringCounter(newStr);
	  printf("Banana: %d %d\n",numOfNums, newStrCount);
	  if(newStrCount > numOfNums){
	     printf("Too many numbers in the file\n");
	     //return(-1);
	  }
	  else if(newStrCount > numOfNums){
	     printf("Not enough numbers in the file\n");
	     //return (-1);
	  }
	  struct Stack* stack = createStack(100);
          /*
	  for(k = 0; k< numOfNums; k++){
	     sscanf(newStr, "%d", &helpStack[k]);
	     push(stack, helpStack[k]);
	  }
	 */ 
	  FILE *stream;
	  stream = fmemopen(newStr, strlen(newStr), "r");
	  for(k = 0; k<numOfNums; k++){
	     fscanf(stream, "%d", &helpStack[k]);
	     push(stack, helpStack[k]);
	     }
	  fprintf(fo, "%d: ", getpid());
	  for(k = 0; k < numOfNums; k++){
	    fprintf(fo, "%d ", pop(stack));
	  }
	  fprintf(fo, "\n");
	  fclose(stream);
	  exit(0);
       }
       fgets(str, 100, fp);
       fgets(str, 100, fp);
     }
     //PARENT PROCESS//
     while((wpid = wait(&status)) > 0);
     printf("Hello from parent\n");
     FILE *fo;
     if(oflag == 0){
       fo = fopen(defaultOutputFileName, "a");
     }
     else{
       fo = fopen(outputFileName, "a");
     }
     fprintf(fo, "My pid(parent) is: %d\n", getpid());
     fprintf(fo, "All Children were: ");
     for(i = 0; i<forkCount; i++){
        fprintf(fo, "%d ", child_pid[i]);
     }
     fprintf(fo, "\n");
     fclose(fp);
return 0;
}
