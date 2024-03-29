/*
 * This file is part of 07-tictactoe task
 *
 * main.c
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

// defines
#define board_t int32_t**
#define cell_t  int32_t

#define ROWS   3
#define COLS   3

#define EMPTY    0
#define CROSS    10
#define CIRCLE   100
#define GAME_END -1

#define DEFAULT_SEMAPHORE_KEY 754
#define DEFAULT_SEMAPHORE_PERMISSIONS 0777

#define SEM_PLAYER_0 0
#define SEM_PLAYER_1 1

// prototypes
void   game_print_board (board_t board);
int8_t game_move_check  (board_t board);
int8_t game_move_make   (board_t board, cell_t row, cell_t col, cell_t player_sign);

void teardown (int32_t signal);

// globals
int32_t shared_memory;
int32_t semaphore;

int32_t main (int argc, char *argv[])
{
  cell_t *shared_memory_ptr;
  cell_t  player_sign;
  board_t board;

  struct sembuf try_move_X = {SEM_PLAYER_0, -1, 0},
                try_move_O = {SEM_PLAYER_1, -1, 0},
                try_X = {SEM_PLAYER_0, 1, 0},
                try_O = {SEM_PLAYER_1, 1, 0},
                *move,
                *lock;

  // gracefully quit on Ctrl+C
  signal (SIGINT, teardown);
  shared_memory = shmget (DEFAULT_SEMAPHORE_KEY, ROWS * COLS * sizeof (cell_t), DEFAULT_SEMAPHORE_PERMISSIONS | IPC_CREAT);
  shared_memory_ptr = shmat (shared_memory, 0, 0);

  // prepare board
  board = malloc (ROWS * sizeof (board[0]));
  for (int32_t i = 0; i < ROWS; ++i)
  {
    board[i] = shared_memory_ptr + i*COLS;
  }

  // decide who starts the game on semaphore creation
  if ((semaphore = semget (DEFAULT_SEMAPHORE_KEY, 2, DEFAULT_SEMAPHORE_PERMISSIONS | IPC_CREAT | IPC_EXCL)) > 0) {
    fprintf (stdout, "You are player nr 1 = X\n");
    player_sign = CROSS;

    semctl (semaphore, SEM_PLAYER_0, SETVAL, 0);
    semctl (semaphore, SEM_PLAYER_1, SETVAL, 0);

    move = &try_move_X;
    lock = &try_O;

    for (int i = 0; i < 3; ++i)
    {
      for (int j = 0; j < 3; ++j)
      {
        board[i][j] = EMPTY;
      }
    }
  } else {
    fprintf (stdout, "You are player nr 2 = O\n");
    player_sign = CIRCLE;
    semaphore = semget (DEFAULT_SEMAPHORE_KEY, 2, 077 | IPC_CREAT);
    move = &try_move_O;
    lock = &try_X;
    semop (semaphore, lock, 1);
  }

  // game loop
  while (1<2) {
    fprintf (stdout, "Waiting for opposite player.\n");
    semop (semaphore, move, 1);

    game_print_board (board);
    if (game_move_check (board) == GAME_END) {
      teardown (0);
    }

    cell_t row, col;
    do {
      uint8_t coordinates_valid = 1;
      do {
        printf ("Your turn, enter coordinates in format: \"X Y\": ");
        scanf ("%i %i", &row, &col);

        if (row < 0 || row >= ROWS) {
          coordinates_valid = 0;
        }
        if (col < 0 || col >= COLS) {
          coordinates_valid = 0;
        }
      }
      while (!coordinates_valid);
    } while (game_move_make (board, row, col, player_sign) < 0);
    game_print_board (board);
    semop (semaphore, lock, 1);
  }

  if (board) {
    free (board);
  }

  teardown (0);
  return EXIT_SUCCESS;
}

void game_print_board (board_t board)
{
  fprintf (stdout, " ");
  for (size_t i = 0; i < COLS; ++i)
  {
    fprintf (stdout, " %li", i);
  }
  fprintf (stdout, "\n");

  for (size_t i = 0; i < ROWS; ++i)
  {
    fprintf (stdout, "%li|", i);
    for (int j = 0; j < COLS; ++j)
    {
      if (board[i][j] == CIRCLE)
      {
        fprintf (stdout, "O");
      } else if (board[i][j] == CROSS) {
        fprintf (stdout, "X");
      } else {
        fprintf (stdout, " "); // when empty
      }
      fprintf (stdout, "|");
    }
    fprintf (stdout, "\n");
  }
  fprintf (stdout, "\n");
}

int8_t game_move_check (board_t board)
{
  cell_t fields = 0;
  for (size_t i = 0; i < ROWS; ++i)
  {
    int vertical_sum   = 0;
    int horizontal_sum = 0;
    int diagonal_sum = 0;

    for (size_t j = 0; j < COLS; ++j) {
      if (board[i][j] != EMPTY) {
        fields++;
      }
      vertical_sum   += board[i][j];
      horizontal_sum += board[j][i];
      diagonal_sum   += board[j][j];
    }


    if ((vertical_sum == 3 * CROSS) || (horizontal_sum == 3 * CROSS) || (diagonal_sum == 3 * CROSS)) {
      fprintf (stdout, "Player X wins!\n");
      return GAME_END;
    }

    if ((vertical_sum == 3 * CIRCLE) || (horizontal_sum == 3 * CIRCLE) || (diagonal_sum == 3 * CIRCLE)) {
      fprintf (stdout, "Player O wins!\n");
      return GAME_END;
    }
  }

  if (fields == ROWS*COLS) {
    printf ("DRAW!\n");
    return GAME_END;
  }
  return 0;
}

int8_t game_move_make (board_t board, cell_t row, cell_t col, cell_t player_sign)
{
  if (board[row][col] == EMPTY) {
    board[row][col] = player_sign;
    return 0;
  } else {
    fprintf (stderr, "Try one more time.\n");
    return -1;
  }
}

void teardown (int32_t signal)
{
  semctl (semaphore, 0, IPC_RMID, 0);
  shmctl (shared_memory, IPC_RMID, 0);
  exit (EXIT_SUCCESS);
}
