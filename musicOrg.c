#include "library.h"
#include "constants.h"
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
