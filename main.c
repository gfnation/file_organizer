#include "library.h"
#include "constants.h"
#include "easyOrg.h"
#include "musicOrg.h"
#include "vidOrg.h"
char* findExtension(char* fileName);
void organize(DIR* dir);
int ignore(char* name);

char* getTokened(char* original, char token);
char* getRemaining(char* original, char* split);

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
  mkdir("./music", 0777);
  mkdir("./movies", 0777);
  mkdir("./shows", 0777);
  mkdir("./documents", 0777);
  mkdir("./pictures", 0777);
  mkdir("./other", 0777);

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
  extension[count] = '\0';
  return extension;
}

void organize(DIR *dir)
{
  struct dirent *dir_pt;

  while((dir_pt = readdir(dir)) !=NULL)
  {


    if(ignore(dir_pt->d_name)==0)
    {
      if(strcmp(findExtension(dir_pt->d_name), VIDEO) == 0)
      {
        vidOrg(dir_pt->d_name);
      }
      else if(strcmp(findExtension(dir_pt->d_name), MUSIC) == 0)
      {
        //TODO: add music caller here
        musicOrg(dir_pt->d_name);
      }
      else if(strcmp(findExtension(dir_pt->d_name), DOC) == 0 || strcmp(findExtension(dir_pt->d_name), TEXT) == 0)
      {
        //TODO: handle documents
        easyOrg(dir_pt->d_name, findExtension(dir_pt->d_name));

      }
      else if(strcmp(findExtension(dir_pt->d_name),JPEG) == 0 || strcmp(findExtension(dir_pt->d_name), PNG) == 0)
      {
        //TODO: handle pictures
        easyOrg(dir_pt->d_name, findExtension(dir_pt->d_name));
      }
      else
      {
        //TODO: handle all other cases
        easyOrg(dir_pt->d_name, findExtension(dir_pt->d_name));
      }
    }
  }
}






int ignore(char* name)
{
  //ignore current and past directories
  if(strcmp(name, ".") == 0 || strcmp(name, "..") ==0 || strcmp(name, "core") == 0)
  {
    return 1;
  }
  //ignore folders I made
  else if(strcmp(name, "music") == 0 || strcmp(name, "movies") == 0 || strcmp(name, "shows") == 0 || strcmp(name, "documents")==0 || strcmp(name, "other")==0 || strcmp(name, "pictures") ==0)
  {
    return 1;
  }
  //ignore headers and c files and executables
  else if(strcmp(findExtension(name), ".c") ==0 || strcmp(findExtension(name), ".h") ==0 || strcmp(name, "organizer") ==0)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

char* getTokened(char* original, char token)
{
  char* returnString = malloc(strlen(original) * sizeof(char));
  int i=0;
  while(original[i] != token)
  {
    returnString[i] = original[i];
    i++;
  }
  returnString[i] = '\0';
  return returnString;
}

char* getRemaining(char* original, char* split)
{
  char* string = malloc(strlen(original) * sizeof(char));
  int length = strlen(split) +1;
  int i=0;
  for(;length < strlen(original); length++)
  {
    string[i] = original[length];
    i++;
  }
  return string;
}
