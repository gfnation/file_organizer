#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void createFolder(char* folderName);
char* findExtension(char* fileName);

int main(int argc, char* argv[])
{

  if (argc >2)
  {
    fprintf(stderr, "The argument count is not correct.\n");
    exit(1);
  }

  char string[] = "string.tct";
  char* exten = findExtension(string);
  printf("%s\n", exten);
}

void createFolder(char* folderName)
{


}

char* findExtension(char* fileName)
{
  char* extension;

  int loop = strlen(fileName);

  unsigned char count =0;
  while(!fileName[loop] != '.')
  {
    count++;
    loop--;
    printf("%d %d\n", loop, count);
  }

  extension = malloc(count * sizeof(char));

  int index =0;
  while(index != count)
  {
    extension[index] = fileName[loop];
    index++;
    loop++;
    printf("%d %d\n", loop, index);
  }
  return extension;
}
