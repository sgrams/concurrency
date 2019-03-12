/*
 * common.c
 * Stanisław Grams <sgrams@fmdx.pl>
 *
*/
#include "common.h"

int write_buffer (const char *out_file, char *buffer, int size)
{
  int fd;
  fd = open (out_file, O_CREAT | O_WRONLY);
  if (!fd) {
    return -1;
  }
  write (fd, buffer, size);
  close (fd);

  return 0;
}

int file_length (const char *filepath)
{
  struct stat st;
  stat (filepath, &st);
  return st.st_size;
}

int read_buffer (const char *in_file, char **buffer, int size)
{
  if (size < 1) {
    return -1;
  }

  int fd;
  *buffer = malloc (sizeof (char) * (size + 1));
  if ((*buffer) == NULL) {
    return -1;
  }
  fd = open (in_file, O_RDONLY);
  read (fd, *buffer, size);
  buffer[size] = '\0';
  close (fd);

  return 0;
}

void lockfile_loop (const char *loopfile)
{
  while (open (loopfile, O_CREAT|O_EXCL, 0) == -1)
  {
    printf("warning: server temporarily unavailable, please wait...\n");
    sleep (DEFAULT_SLEEP_VALUE);
  }
}

void response_loop (const char *responsefile)
{
  while (access (responsefile, F_OK) != -1);
}

char *get_message ()
{
  char *buffer;
  char ch;
  int i, counter = 0;

  buffer = malloc (255 * sizeof (char));
  for (i = 0; (ch = getchar ()); ++i)
  {
    if (i && (i+1 % 255 == 0)) {
      buffer = realloc (buffer, ((counter + 1) * 255) * sizeof (char));
      counter++;
    }
    buffer[i] = ch;
    if (ch == DEFAULT_MESSAGE_END) {
      break;
    }
  }
  buffer[i] = '\0'; // end of text

  return buffer;
}

char *get_filepath (const char *username, const char *filename)
{
  if (!username || !filename)
  {
    return NULL;
  }
  // declare buffer alike "/home/sgrams/lockfile"
  int size = strlen ("/home/") + strlen (username) + 1 + strlen (filename) + 1;
  char *buffer = malloc (size * sizeof (char));

  snprintf (buffer, size, "/home/%s/%s", username, filename);

  return buffer;
}