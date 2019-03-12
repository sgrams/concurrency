/*
 * server.c
 * Stanisław Grams <sgrams@fmdx.pl>
 *
*/
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "common.h"

int parse_buffer (char *in_buffer, char **hello, int size);

int main (void) {
  char *username = NULL;
  char *hello    = NULL;
  char *buffer   = NULL;
  char *msg      = NULL;
  
  char *lockfile_path   = NULL;
  char *server_buf_path = NULL;
  char *client_buf_path = NULL;

  int size = 0;
  int client_buf_path_len = 0;

  // get strings
  username = getenv ("USER");

  lockfile_path   = get_filepath (username, DEFAULT_LOCKFILE_NAME);
  server_buf_path = get_filepath (username, DEFAULT_SERVER_BUF_NAME);
  chmod (server_buf_path, 777);
  
  while (1)
  {
    // wait for lockfile
    if (access (lockfile_path, F_OK) == 0) {
      buffer = NULL;
      hello  = NULL;
      msg    = NULL;

      // read client message
      chmod (server_buf_path, 777);
      size = file_length (server_buf_path);
      read_buffer (server_buf_path, &buffer, size);
      parse_buffer (buffer, &hello, size);

      // get client buffer path
      client_buf_path_len = strlen ("/home/") + strlen (hello) + 1 + strlen (DEFAULT_CLIENT_BUF_NAME);
      client_buf_path = malloc (sizeof (char) * (client_buf_path_len + 1));
      chmod (client_buf_path, 777);
      snprintf (client_buf_path, client_buf_path_len + 1, "/home/%s/%s", hello, DEFAULT_CLIENT_BUF_NAME);

      // read message
      printf ("%s\n", buffer);

      // write msg to client buffer
      msg  = get_message ();
      size = strlen (msg) + 1;
      msg[size - 1] = '\0';
      write_buffer (client_buf_path, msg, size);
    
      // remove lockfile
      unlink (lockfile_path);
      unlink (server_buf_path);

      if (buffer) {
        free (buffer);
      }
      if (client_buf_path) {
        free (client_buf_path);
      }
      if (msg) {
        free (msg);
      }
      if (hello) {
        free (hello);
      }
    }
  }
  if (lockfile_path) {
    free (lockfile_path);
  }
  if (server_buf_path) {
    free (server_buf_path);
  }

  return EXIT_SUCCESS;
}

int parse_buffer (char *in_buffer, char **hello, int size)
{
  int i = 0;

  if (!in_buffer) {
    return -1;
  }

  *hello = malloc ((size + 1) * sizeof (char));
  for (i = 0; in_buffer[i] != '\n' && i < size; ++i)
  {
    (*hello)[i] = in_buffer[i];
  }
  
  return 0;
}