/*
 * This file is part of 06-ipcq task
 *
 * server.c
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 */
#include "common.h"

// communication
int32_t request_handle (request_t *request);

// teardown
void teardown (int32_t signal);

// globals
int64_t ipcq_server;
int64_t ipcq_client;

dict_t dict[DEFAULT_DICT_SIZE] =
{
  { .pl = "rower", .en = "bicycle" },
  { .pl = "jablko", .en = "apple" },
  { .pl = "pomarancza", .en = "orange" },
  { .pl = "muzyka", .en = "music" },
  { .pl = "kalkulator", .en = "calculator" }
};

int32_t main (int argc, char *argv[])
{
  int32_t status = EXIT_SUCCESS;

  // exit gracefully on SIGINT
  signal (SIGINT, teardown);

  // set ipcqs
  ipcq_server = msgget (DEFAULT_SERVER_IPCQ, DEFAULT_SERVER_IPCQ_PERMISSIONS | IPC_CREAT);
  ipcq_client = msgget (DEFAULT_CLIENT_IPCQ, DEFAULT_SERVER_IPCQ_PERMISSIONS | IPC_CREAT);

  // active loop
  request_t msg;
  while (1<2)
  {
    msgrcv (ipcq_server, &msg, sizeof (request_t) - sizeof (int64_t), 0, 0);
    status = request_handle (&msg);
    if (status) {
      fprintf (stderr, "main: Unable to find entry in the dictionary.\n");
    }
    msgsnd (ipcq_client, &msg, sizeof (request_t) - sizeof (int64_t), 0);
    memset (&msg, 0, sizeof (response_t));
  }

  return EXIT_FAILURE;
}

int32_t request_handle (request_t *req)
{
  int32_t status = EXIT_SUCCESS;
  for (int32_t i = 0; i < DEFAULT_DICT_SIZE; ++i)
  {
    if (!strcmp (dict[i].pl, req->word)) {
      strncpy (req->word, dict[i].en, strlen (dict[i].en));
      if (!strlen (req->word)) {
        status = EXIT_FAILURE;
      }
      return status;
    }
  }

  strncpy (req->word, DEFAULT_MSG_NOT_FOUND, strlen (DEFAULT_MSG_NOT_FOUND) + 1);
  if (!strlen (req->word)) {
    status = EXIT_FAILURE;
  }
  return status;
}

void teardown (int32_t signal)
{
  msgctl (ipcq_server, IPC_RMID, 0);
  msgctl (ipcq_client, IPC_RMID, 0);
  exit (EXIT_SUCCESS);
}
