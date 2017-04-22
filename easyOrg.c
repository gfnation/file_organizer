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
  }
  else if (strcmp(fileExt, DOC) ==0 || strcmp(fileExt, TEXT) ==0)
  {
    chdir("./documents");

  }
  else
  {

    chdir("./other");
  }

  int buf_size = statbuf.st_size;
  char buffer[buf_size];


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
