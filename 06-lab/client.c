/*
 * This file is part of 06-ipcq task
 *
 * client.c
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 */
#include "common.h"

// globals
int64_t ipcq_server;
int64_t ipcq_client;

int32_t main (int argc, char *argv[])
{
  if (argc < 2 || strlen (argv[1]) >= 256) {
    fprintf (stderr, "Invalid parameters!\n");
    return EXIT_FAILURE;
  }

  // set ipcqs
  ipcq_server = msgget (DEFAULT_SERVER_IPCQ, DEFAULT_SERVER_IPCQ_PERMISSIONS);
  ipcq_client = msgget (DEFAULT_CLIENT_IPCQ, DEFAULT_SERVER_IPCQ_PERMISSIONS);

  // request
  request_t req;
  memset (&req, 0, sizeof (req));

  strncpy (req.word, argv[1], strlen (argv[1]));
  req.type = 0x1;
  req.pid  = getpid ();

  msgsnd (ipcq_server, &req, sizeof (request_t) - sizeof (int64_t), 0);

  // response
  response_t res;
  memset (&res, 0, sizeof (res));

  msgrcv (ipcq_client, &res, sizeof (response_t) - sizeof (int64_t), 0, 0);

  printf ("pl: %s, en: %s\n", req.word, res.word);
  return EXIT_SUCCESS;
}
