/*
 * This file is part of 06-ipcq task
 *
 * common.h
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 */
#ifndef CONCURRENCY_IPCQ_H
#define CONCURRENCY_IPCQ_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdint.h>

// ipcq names and permissions
#define DEFAULT_SERVER_IPCQ (int32_t)0xdeadbeef
#define DEFAULT_CLIENT_IPCQ (int32_t)0x0badc0de
#define DEFAULT_SERVER_IPCQ_PERMISSIONS 0666
#define DEFAULT_CLIENT_IPCQ_PERMISSIONS 0666

// default sizes
#define DEFAULT_WORD_SIZE 128
#define DEFAULT_DICT_SIZE 5

// defaults msgs
#define DEFAULT_MSG_NOT_FOUND "Nie znam takiego slowa"

// dictionary
typedef struct dict {
  char pl[DEFAULT_WORD_SIZE];
  char en[DEFAULT_WORD_SIZE];
} dict_t;

// request
typedef struct request {
  int64_t type; // 8 bytes
  int32_t pid;  // 4 bytes
  char    word[DEFAULT_WORD_SIZE]; // 128 bytes
} request_t;

// response
typedef request_t response_t;
#endif // CONCURRENCY_IPCQ_H
