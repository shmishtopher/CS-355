/**
 * ```
 * This is your last chance. After this, there is 
 * no turning back. You take the blue pill — the 
 * story ends, you wake up in your bed and
 * believe whatever you want to believe. You take 
 * the red pill — you stay in Wonderland and I
 * show you how deep the rabbit-hole goes
 * ```
 *
 * @author  Christopher K. Schmitt
 * @version 11.25.2020
 */


#include <curses.h>
#include <unistd.h>
#include <stdlib.h>


/**
 * Shift column takes an array and shifts its
 * contents down by one cell, chopping off the
 * last value.
 *
 * @param {char[]} column - The array to shift by one
 * @param {int} length - The length of the array
 */
void shiftColumn(char column[], int length) {
  for (int i = length - 1; i > 0; i -= 1) {
    column[i] = column[i - 1];
  }
}


/**
 * renderColumn takes a column and renders it to
 * the screen using the curses `addch` function.
 *
 * @param {char[]} column - The column to render to the screen
 * @param {int} length - The length of the array
 * @param {int} position - The column number to print the array into
 */
void renderColumn(char column[], int length, int position) {
  for (int i = 0; i < length; i += 1) {
    move(i, position);
    addch(column[i]);
  }
}


/**
 * randomChar produces one of the three following
 * values with equal probability: space, one, or
 * zero.
 *
 * @return {char} Either zero, one, or a space
 */
char randomChar() {
  switch (rand() % 3) {
    case 0: return '0';
    case 1: return '1';
    case 2: return ' ';
  }
}


int main(int argc, char** argv) {
  // Curses setup
  initscr();
  if (has_colors()) {
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    attron(COLOR_PAIR(1));
  }

  // Get matrix speed
  long sleepDuration = 0;
  if (argc == 2) {
    sleepDuration = 100000 - atoi(argv[1]) * 1000;
  }
  else {
    printf("Usage: matrix speed\r\n");
    printf("Speed is [1-99]\r\n");
    endwin();
    return -1;
  }

  // Create row of columns
  char matrix[COLS][LINES];

  for (int i = 0; i < COLS; i += 1) {
    for (int j = 0; j < LINES; j += 1) {
      matrix[i][j] = ' ';
    }
  }

  // Produce random characters until interrupted
  while (true) {
    for (int i = 0; i < COLS; i += 1) {
      shiftColumn(matrix[i], LINES);
      matrix[i][0] = randomChar();
      renderColumn(matrix[i], LINES, i);
    }

    refresh();
    usleep(sleepDuration);
  }

  endwin();
}