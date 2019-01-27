#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char **argv){
  int c;
  char inputFileName[20] = "input.dat";
  char outputFileName[20] = "output.dat";
  while((c = getopt(argc, argv, "hi::o::")) != -1)
     switch(c)
     {
     case 'h':
        printf("Legal Command line options: -h, -i, -o\n");
	printf("-i is the input file and should have an input file name afterwards\n");
	printf("-o is the output file and should have an output file name afterwards\n");
	printf("If either input file name or output file name is not specified, the defaults input.dat and output.dat will be used respectively.\n");
	printf("If -h is used, all other options will be ignored.\n");
	return 0;
     case 'i':
	if(optarg != NULL){
	   strncpy(inputFileName, optarg, sizeof(inputFileName)); 
	   printf("%s", inputFileName);
	   }
     	break;
     case 'o':
     	if(optarg != NULL){
	    strncpy(outputFileName, optarg, sizeof(outputFileName));
	    printf("%s", outputFileName);
	}
	break;
     case '?':
	printf("invalid option used");
	break;
     default:
        printf("getopt didn't work");
     }
     
     //BEGIN FILE PROCESSING//
     FILE *fp;
     int count = 0, i;;
     fp = fopen(inputFileName, "r");
     fscanf(fp, "%d", &count);
     
     //FORKS//
     pid_t child_pid, wpid;
     int status =0;
     for (i = 0; i < count; i++){
       if ((child_pid = fork()) == 0) {
          printf("Hello from child\n");  
          exit(0);
       }
     }
     while((wpid = wait(&status)) > 0);
     printf("Hello from parent");
return 0;
}
