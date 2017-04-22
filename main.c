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
void musicOrg(char* fileName);
void vidOrg(char* fileName);

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
void vidOrg(char* fileName)
{

}

void musicOrg(char* fileName)
{

  printf("%s\n", fileName);

  char* throwAway = malloc(sizeof(char) * strlen(fileName) +1);
  strcpy(throwAway, fileName);

//Make own tokenizer

  char* artist;
  artist = getTokened(fileName, '-');
  char* artRemaining = getRemaining(fileName, artist);
  char* album = getTokened(artRemaining, '-');
  char* albRemaining = getRemaining(artRemaining, album);
  printf("%\n", albRemaining);

  int oldFile;
  if((oldFile = open(fileName, O_RDONLY)) == -1)
  {
    fprintf(stderr, "%s\n", "The old music file couldn't be opened" );
    exit(1);
  }
  struct stat statbuf;
  if(stat(fileName, &statbuf) == -1)
  {
    fprintf(stderr, "%s\n","Couldn't get stat on file" );
    exit(1);
  }



  int buffer_size = statbuf.st_size;
  char* buffer[buffer_size];
  //We know the extension will put us in music folders
  chdir("./music"); DIR *dir = opendir(".");

  //Time to read the directory for the Artist name
  struct dirent *dirent_pt;
  int found = 1;
  while(((dirent_pt = readdir(dir)) != NULL) || found ==1)
  {
    if(strcmp(nextTok, dirent_pt->d_name) == 0)
    {
      chdir(nextTok);
      found =0;
    }
  }
  //if found = 1 the artist was not found
  if(found ==1)
  {
    mkdir(nextTok, 0777);
    chdir(nextTok);
  }

  //Check for album
  struct dirent *alb_pt;
  found =1;
  dir = opendir(".");
  while(((alb_pt = readdir(dir)) != NULL) && found == 1)
  {
    if(strcmp(nextTok, alb_pt->d_name) ==0)
    {
      chdir(nextTok);
      found = 0;
    }
  }

  if(found == 1)
  {
    mkdir(nextTok, 0777);
    chdir(nextTok);
  }

  //Now that we are in album create final file
  int newFile;
  //Create a new name for the song
  char* songExt = malloc((sizeof(char) * strlen(nextTok)) + (sizeof(char) * strlen(findExtension(fileName))) + 1);
  strcpy(songExt, nextTok);
  strcat(songExt, findExtension(fileName));

  if((newFile = open(songExt, O_CREAT | O_WRONLY, 0644)) == -1)
  {
    fprintf(stderr, "%s\n", "The file couldn't be moved");
    exit(1);
  }

  int n_read, o_read;
  while ((o_read = read(oldFile, buffer, buffer_size)) > 0)
  {
    n_read = write(newFile, buffer, buffer_size);
  }

  close(newFile); close(oldFile);

  chdir("../..");
  remove(fileName);
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

    chdir("..");

    remove(fileName);

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
