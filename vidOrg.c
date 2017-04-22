#include "library.h"
#include "constants.h"
#include "token.h"
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
  if(strcmp(fileCent, CENT2)==0 || strcmp(fileCent, CENT1) == 0)
  {
    //get the movie year and movie name
    char* year = getTokened(fileName, '-');
    char* movieName = getRemaining(fileName, year);

    //Move into movie directory
    chdir("./movies"); DIR* dir = opendir(".");
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
    chdir("./shows");
    DIR *dir = opendir(".");

    char* showDir = malloc(sizeof(char)*strlen(show) +2);
    strcpy(showDir, "./"); strcat(showDir, show);

    struct dirent *show_dir;
    int found =1;
    while(((show_dir = readdir(dir)) != NULL) && found == 1)
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
        found =0;
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
