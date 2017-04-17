#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#define VIDEO ".mkv"
#define MUSIC ".mp3"
#define DOC ".docx"
#define TEXT ".txt"
#define JPEG ".jpg"
#define PNG ".png"

char* findExtension(char* fileName);
void organize(DIR* dir);
void  otherOrg(DIR *curr, char *fileName);

int main(int argc, char* argv[])
{

  if (argc >2)
  {
    fprintf(stderr, "The argument count is not correct.\n");
    exit(1);
  }

  int current_dir = chdir(".");
  //change directory to specified directory if needed
  if(argc == 2)
  {

    if (current_dir = chdir(argv[1]) == -1)
    {
      fprintf(stderr, "%s\n", "The path file could not be found.\n");
      exit(1);
    }
  }

  //Make all directory
  mkdir("./music", 0744);
  mkdir("./movies", 0744);
  mkdir("./shows", 0744);
  mkdir("./documents", 0744);
  mkdir("./pictures", 0744);
  mkdir("./other", 0744);

  DIR *dir = opendir(".");
  organize(dir);

}



char* findExtension(char* fileName)
{
  char* extension;

  int loop = strlen(fileName);

  unsigned char count =0;
  while(fileName[loop] != '.')
  {
    count++;
    loop--;
  }

  extension = malloc(count * sizeof(char));

  int index =0;
  while(index != count)
  {
    extension[index] = fileName[loop];
    index++;
    loop++;
  }
  return extension;
}

void organize(DIR *dir)
{
  struct dirent *dir_pt;

  while((dir_pt = readdir(dir)) !=NULL)
  {

    if(strcmp(findExtension(dir_pt->d_name), VIDEO))
    {


    }
    else if(strcmp(findExtension(dir_pt->d_name), MUSIC))
    {
      //TODO: add music caller here
    }
    else if(strcmp(findExtension(dir_pt->d_name), DOC) || strcmp(findExtension(dir_pt->d_name), TEXT))
    {
      //TODO: handle documents

    }
    else if(strcmp(findExtension(dir_pt->d_name),JPEG) || strcmp(findExtension(dir_pt->d_name), PNG))
    {
      //TODO: handle pictures
    }
    else
    {
      //TODO: handle all other cases
      otherOrg(dir, dir_pt->d_name);
    }
  }
}

void  otherOrg(DIR *curr, char *fileName)
{
  char *oldFile = malloc(strlen(fileName)+4);
  strcpy(oldFile, "../");
  strcat(oldFile, fileName);

  printf("%s\n", oldFile);
}
