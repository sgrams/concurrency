/*
 * client.c
 * Stanisław Grams <sgrams@fmdx.pl>
 *
*/
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

int main (int argc, char *argv[]) {
  char *hello    = NULL;
  char *username = NULL;
  char *buffer   = NULL;
  char *msg      = NULL;
  
  char *lockfile_path   = NULL;
  char *server_buf_path = NULL;
  char *client_buf_path = NULL;

  int size = 0;
  int client_buf_path_len = 0;
  // obtain username by parameter
  if (argc < 2) {
    fprintf (stderr, "%s: username not provided. Unable to start program.\n", argv[0]);
    return EXIT_FAILURE;
  }

  // get strings
  username = strdup (argv[1]);
  msg      = get_message ();
  hello    = getenv ("USER");
  lockfile_path   = get_filepath (username, DEFAULT_LOCKFILE_NAME);
  server_buf_path = get_filepath (username, DEFAULT_SERVER_BUF_NAME);
  chmod (server_buf_path, 777);
  
  client_buf_path_len = strlen ("/home/") + strlen (hello) + 1 + strlen (DEFAULT_CLIENT_BUF_NAME);
  client_buf_path = malloc (sizeof (char) * (client_buf_path_len + 1));
  chmod (client_buf_path, 777);
  snprintf (client_buf_path, client_buf_path_len + 1, "/home/%s/%s", getenv("USER"), DEFAULT_CLIENT_BUF_NAME);

  // write server buffer
  size = strlen (hello) + strlen ("\n") + strlen (msg) + 1;
  char *tmp = malloc (size + 1);
  snprintf (tmp, size, "%s\n%s%c", hello, msg, DEFAULT_MESSAGE_END);
  write_buffer (server_buf_path, tmp, size);
  free (tmp);

  // wait for server
  lockfile_loop (lockfile_path);
  response_loop (lockfile_path);
  // read response
  chmod (client_buf_path, 777);
  size = file_length (client_buf_path);
  read_buffer (client_buf_path, &buffer, size);
  buffer[size] = '\0';

  // display client buffer (response)
  printf ("%s\n", buffer);

  // remove response file
  chmod (client_buf_path, 777);
  // open (client_buf_path, O_RDONLY | O_WRONLY | O_TRUNC); // clear client buffer
  unlink (client_buf_path);

  // free memory
  if (buffer) {
    free (buffer);
  }
  if (client_buf_path) {
    free (client_buf_path);
  }
  if (server_buf_path) {
    free (server_buf_path);
  }
  if (lockfile_path) {
    free (lockfile_path);
  }
  if (msg) {
    free (msg);
  }
  if (username) {
    free (username);
  }

  return EXIT_FAILURE;
}

