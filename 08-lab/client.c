/*
 * This file is part of 08-sockets task
 *
 * client.c
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

//#define DEFAULT_SERVER_ADDR "153.19.1.202"
//#define DEFAULT_SERVER_PORT 5000
#define DEFAULT_SERVER_ADDR "127.0.0.1"
#define DEFAULT_SERVER_PORT 12345

// prototypes
int32_t calculate_y (int32_t x, int32_t *y);

int32_t main (int32_t argc, char *argv[])
{
  int32_t status = 0;
  int32_t x, y;

  // get 4B number pre-allocated on the stack
  if (argc < 2) {
    fprintf (stdout, "Please provide a number as a parameter to the program.\n");
    return EXIT_SUCCESS;
  }
  x = strtol (argv[1], (char **)NULL, 10);

  // send and receive a request to the server
  status = calculate_y (x, &y);
  if (-1 == status) {
    fprintf (stderr, "%s: critical error, terminating...\n", argv[0]);
    return EXIT_FAILURE;
  }

  // handle stdout output
  printf ("   x = %i\n", x);
  printf ("f(x) = %i\n", y);
  return EXIT_SUCCESS;
}

int32_t
calculate_y (int32_t x, int32_t *y)
{
  int32_t  status = 0;
  int32_t  sockfd;
  int32_t  msg;
  uint32_t recv_size;
  struct   sockaddr_in addr;

  if (NULL == y) {
    status = -1;
    fprintf (stderr, "  unable to perform calculate_y (): y has to be pre-allocated\n");
    return status;
  }
  msg = htonl (x);

  addr.sin_family = PF_INET;
  addr.sin_port   = htons (DEFAULT_SERVER_PORT);
  status = inet_pton (PF_INET, DEFAULT_SERVER_ADDR, &(addr.sin_addr));
  if (1 > status) {
    status = -1;
    fprintf (stderr, "  unable to perform calculate_y (): inet_pton returned: %s\n", strerror (errno));
    return status; // return -1 when address invalid
  }

  // prepare socket and perform bind
  sockfd = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (-1 == sockfd) {
    status = sockfd;
    fprintf (stderr, "  unable to perform calculate_y (): socket returned: %s\n", strerror (errno));
    return status;
  }

  int32_t val = 1;
  status = setsockopt (sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&val, sizeof (val));
  if (0 > status) {
    fprintf (stderr, "  unable to perform calculate_y (): setsockopt returned: %s\n", strerror (errno));
    return status;
  }

  // handle communication
  printf("dupa!\n");
  status = sendto (sockfd, (uint8_t *)&msg, sizeof (int32_t), 0, (struct sockaddr *)&addr, sizeof (addr));
  if (-1 == status) {
    fprintf (stderr, "  unable to perform calculate_y (): sendto returned: %s\n", strerror (errno));
    return status;
  }
  printf("dupa!\n");
  status = recvfrom (sockfd, (uint8_t *)&msg, sizeof (int32_t), 0, (struct sockaddr *)&addr, &recv_size);
  if (-1 == status) {
    fprintf (stderr, "  unable to perform calculate_y (): recvfrom returned: %s\n", strerror (errno));
    return status;
  }
  printf("dupa!\n");

  // return with success
  *y = ntohl (msg);
  status = 0;
  return status;
}
