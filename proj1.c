#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <limits.h>

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
  char inputFileName[20] = "input.dat";
  char outputFileName[20] = "output.dat";
  
  //Grabs command line arguments and based on case, does something//
  while((c = getopt(argc, argv, "hi::o::")) != -1)
     switch(c)
     {
     case 'h':
        printf("Legal Command line options: -h, -i, -o\n");
	printf("-i is the input file and should have an input file name afterwards\n");
	printf("-o is the output file and should have an output file name afterwards\n");
	printf("If either input file name or output file name is not specified, the defaults input.dat and output.dat will be used respectively.\n");
	printf("**If -h is used, all other options will be ignored.\n");
	return 0;
     case 'i':
	if(optarg != NULL){
	   strncpy(inputFileName, optarg, sizeof(inputFileName)); 
	   }
     	break;
     case 'o':
     	if(optarg != NULL){
	    strncpy(outputFileName, optarg, sizeof(outputFileName));
	}
	break;
     case '?':
	printf("invalid option used");
	return 0;
	break;
     default:
        printf("getopt didn't work");
	return 0;
	break;
     }
     
     //BEGIN FILE PROCESSING//
     FILE *fp;
     int count = 0, i;
     fp = fopen(inputFileName, "r");
     fscanf(fp, "%d", &count);
     
     //Child Processes in a loop//
       pid_t child_pid[count], wpid;
       int status =0;
       int origStack[8];
       int numOfNumsFake;
     for (i = 0; i < count; i++){
       if(i > 0){
          sleep(.5*i);
       }
       if ((child_pid[i] = fork()) == 0) {
          FILE *fo;
	  fo = fopen(outputFileName, "a");
	  int numOfNums=0;
	  int helpStack[numOfNums], k = 0;
	  fscanf(fp, "%d", &numOfNums); 

	  struct Stack* stack = createStack(numOfNums);
          for(k = 0; k< numOfNums; k++){
	     fscanf(fp, "%d", &helpStack[k]);
	     push(stack, helpStack[k]);
	  }
	  fprintf(fo, "%d: ", getpid());
	  for(k = 0; k < numOfNums; k++){
	    fprintf(fo, "%d ", pop(stack));
	  }
	  fprintf(fo, "\n");
	  exit(0);
       }
       fscanf(fp, "%d", &numOfNumsFake);
       int j;
       for(j = 0; j < numOfNumsFake; j++){
          fscanf(fp, "%d", &origStack[j]);
       }
     }
     //PARENT PROCESS//
     while((wpid = wait(&status)) > 0);
     printf("Hello from parent\n");
     FILE *fo;
     fo = fopen(outputFileName, "a");
     fprintf(fo, "My pid(parent) is: %d\n", getpid());
     fprintf(fo, "All Children were: ");
     for(i = 0; i<count; i++){
        fprintf(fo, "%d ", child_pid[i]);
     }
     fprintf(fo, "\n");
     fclose(fp);
return 0;
}
