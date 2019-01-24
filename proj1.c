#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv){
  int c;
  printf("Hello World\n");
  while((c = getopt(argc, argv, "h")) != -1)
     switch(c)
     {
     case 'h':
        printf("case h");
	break;
     case '?':
        printf("Error");
	break;
     default:
        printf("getopt didn't work");
     }
return 0;
}
