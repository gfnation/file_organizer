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
void  easyOrg(char *fileName, char* fileExt);
int ignore(char* name);

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
  mkdir("./music", 0644);
  mkdir("./movies", 0644);
  mkdir("./shows", 0644);
  mkdir("./documents", 0644);
  mkdir("./pictures", 0644);
  mkdir("./other", 0644);

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


      }
      else if(strcmp(findExtension(dir_pt->d_name), MUSIC) == 0)
      {
        //TODO: add music caller here
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

void  easyOrg(char *fileName, char* fileExt)
{
  int ofile, nfile, nread, nwrite;
  struct stat statbuf;

  if((ofile = open(fileName, O_RDONLY)) == -1)
  {
    fprintf(stderr, "%s\n", "The old file couldn't be opened");
    exit(1);
  }

  if(stat(fileName, &statbuf) == -1)
  {
    fprintf(stderr, "%s\n", "Could not get stat on file");
    exit(1);
  }

    if(strcmp(fileExt, PNG) ==0 || strcmp(fileExt, JPEG) ==0)
    {
      chdir("./pictures");
      printf("%s\n","Working in Pictures now");
    }
    else if (strcmp(fileExt, DOC) ==0 || strcmp(fileExt, TEXT) ==0)
    {
      chdir("./documents");

      printf("%s\n","Working in Documents now");
    }
    else
    {

      printf("%s\n","Working in other now");
      chdir("./other");
    }

    //char *oldFile = malloc(strlen(fileName)+4);
    //strcpy(oldFile, "../");
    //strcat(oldFile, fileName);

    int buf_size = statbuf.st_size;
    char buffer[buf_size];

    printf("%s\n", fileName);

    if((nfile = open(fileName, O_CREAT | O_WRONLY, 0644)) ==-1)
    {
      fprintf(stderr, "%s\n", "Couldn't open the new file");
      exit(1);
    }


    while ((nread = read(ofile, buffer, buf_size)) > 0)
    {
      nwrite = write(nfile, buffer, buf_size);
    }

    close(nfile); close(ofile);

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
