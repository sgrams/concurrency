/*
 * common.h
 * Stanisław Grams <sgrams@fmdx.pl>
 *
*/
#ifndef CONCURRENCY_MSG_COMMON_H
#define CONCURRENCY_MSG_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define DEFAULT_SERVER_BUF_NAME "srv/dane"
#define DEFAULT_CLIENT_BUF_NAME "client/wyniki"
#define DEFAULT_LOCKFILE_NAME   "srv/lockfile"
#define DEFAULT_MESSAGE_END 0x1B // Esc
#define DEFAULT_SLEEP_VALUE 3 // in seconds

int  write_buffer (const char *out_file, char *buffer, int size);
int  read_buffer  (const char *in_file, char **buffer, int size);
int  file_length  (const char *filepath);
void lockfile_loop (const char *loopfile);
void response_loop (const char *responsefile);

char *get_message ();
char *get_filepath (const char *username, const char *filename);

#endif // CONCURRENCY_MSG_COMMON_H