#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char **argv){
  int c;
  while((c = getopt(argc, argv, "hi::o::")) != -1)
     switch(c)
     {
     case 'h':
        printf("Legal Command line options: -h, -i, -o\n");
	printf("-i is the input file and should have an input file name afterwards\n");
	printf("-o is the output file and should have an output file name afterwards\n");
	printf("If either input file name or output file name is not specified, the defaults input.dat and output.dat will be used respectively.\n");
	return 0;
     case 'i':
	if(optarg == NULL){
	   printf("no input file");
	}
	else{
	   printf("%s", optarg);
	   }
     	break;
     case 'o':
     	if(optarg == NULL){
	   printf("no output file");
	}
	else{
	    printf("%s", optarg);
	}
	break;
     case '?':
        printf("Invalid option");
	break;
     default:
        printf("getopt didn't work");
     }
return 0;
}
