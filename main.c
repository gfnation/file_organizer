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
#define CENT1 "19"
#define CENT2 "20"

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
  //Using file Cent to store the century the movie was made if that's the case
  char* fileCent = malloc(sizeof(char)*3);
  int i =0;
  for(; i<2;i++)
  {
    fileCent[i] = fileName[i];
  }
  fileCent[2] = '\0';



  //open the old file
  int oldFile;
  if(oldFile = open(fileName, O_RDONLY) == -1)
  {
    fprintf(stderr, "%s\n", "The old movie file couldn't be opened.");
    exit(1);
  }

  //Get the stats for the file
  struct stat statbuf;
  if(stat(fileName, &statbuf) == -1)
  {
    fprintf(stderr, "%s\n","Couldn't get stat on file" );
    exit(1);
  }
  //will be used later to write old data to the new file
  int buffer_size = statbuf.st_size;
  char* buffer[buffer_size];



  //If the file starts with 19 or 20 assume that it is a movie
  if(strcmp(fileCent, CENT2)==0 || strcmp(fileCent, CENT1))
  {
    //get the movie year and movie name
    char* year = getTokened(fileName, '-');
    char* movieName = getRemaining(fileName, year);

    //Move into movie directory
    chdir("./movies"); DIR* dir = opendir(".");
    printf("%s\n", "working in videos" );
    //Turn movie year into a folder
    char* yearDir = malloc((sizeof(char) * strlen(year))+ 2);
    strcpy(yearDir, "./"); strcat(yearDir, year);

    //Test to see if that year folder existed
    struct dirent *yr_dir;
    int found =1;
    while(((yr_dir = readdir(dir)) != NULL) && found ==1)
    {
      if(strcmp(yr_dir->d_name, year) == 0)
      {
        chdir(yearDir);
        found =0;
      }
    }
    //If found = 1 then the directory isn't found
    if(found == 1)
    {
      mkdir(yearDir, 0777);
      chdir(yearDir);
    }
    //Now we are in the year Directory place the new file in the folder.
    int newFile;
    if((newFile = open(movieName, O_CREAT | O_WRONLY, 0644)) == -1)
    {
      fprintf(stderr, "%s\n", "The new movie file couldn't be created." );
      exit(1);
    }
    int n_read, o_read;
    while ((o_read = read(oldFile, buffer, buffer_size)) > 0)
    {
      n_read = write(newFile, buffer, buffer_size);
    }

    close(newFile); close(oldFile);
    chdir("../..");
  }
  //if not got to tv shows
  else
  {

    //get show, season and episode
    char* show = getTokened(fileName, '-');
    char* showRemain = getRemaining(fileName, show);
    char* season = getTokened(showRemain, '-');
    char* episode = getRemaining(showRemain, season);

    //change directory to tv shows
    chdir("./shows"); DIR *dir = opendir(".");

    char* showDir = malloc(sizeof(char)*strlen(show) +2);
    strcpy(showDir, "./"); strcat(showDir, show);

    struct dirent *show_dir;
    int found =1;
    while((show_dir = readdir(dir) != NULL) && found == 1)
    {
      if(strcmp(show, show_dir->d_name) == 0)
      {
        chdir(showDir);
        found =0;
      }
    }
    if(found ==1)
    {
      mkdir(showDir, 0777);
      chdir(showDir);
    }

    struct dirent *season_dir;
    found =1;
    dir = opendir(".");

    while(((season_dir = readdir(dir)) != NULL) && found ==1)
    {
      if(strcmp(season, season_dir->d_name) ==0)
      {
        chdir(season);
      }
    }
    if(found ==1)
    {
      mkdir(season, 0777);
      chdir(season);
    }

    int newFile;
    if((newFile = open(episode, O_CREAT | O_WRONLY, 0644)) == -1)
    {
      fprintf(stderr, "%s\n", "The new tv show file couldn't be created." );
      exit(1);
    }
    int n_read, o_read;
    while ((o_read = read(oldFile, buffer, buffer_size)) > 0)
    {
      n_read = write(newFile, buffer, buffer_size);
    }

    close(newFile); close(oldFile);

    chdir("../../..");
  }
  remove(fileName);

}

void musicOrg(char* fileName)
{


  //Make own tokenizer

  char* artist = getTokened(fileName, '-');
  char* artRemaining = getRemaining(fileName, artist);
  char* album = getTokened(artRemaining, '-');
  char* albRemaining = getRemaining(artRemaining, album);


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
  char* artPath = malloc((sizeof(char) * strlen(artist)) +2);
  strcpy(artPath, "./");
  strcat(artPath, artist);
  struct dirent *dirent_pt;
  int found = 1;
  while(((dirent_pt = readdir(dir)) != NULL) && found ==1)
  {
    if(strcmp(artist, dirent_pt->d_name) == 0)
    {
      chdir(artPath);
      found =0;
    }
  }
  //if found = 1 the artist was not found
  if(found ==1)
  {
    mkdir(artPath, 0777);
    chdir(artPath);
  }

  //Check for album
  struct dirent *alb_pt;
  found =1;
  dir = opendir(".");
  while(((alb_pt = readdir(dir)) != NULL) && found == 1)
  {
    if(strcmp(album, alb_pt->d_name) ==0)
    {
      chdir(album);
      found = 0;
    }
  }

  if(found == 1)
  {
    mkdir(album, 0777);
    chdir(album);
  }

  //Now that we are in album create final file
  int newFile;
  //Create a new name for the song

  if((newFile = open(albRemaining, O_CREAT | O_WRONLY, 0644)) == -1)
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

  chdir("../../..");
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
