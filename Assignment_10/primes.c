/**
 * primes uses a brute-force approach to
 * find prime numbers.  When an interrupt
 * signal is recived, primes prints out
 * the current highest prime and prompts
 * the user to quit or continue.
 *
 * @author    Christopher K. Schmitt
 * @version   11.18.2020
 */

#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <limits.h>
#include <signal.h>


/**
 * interruptHandler replaces the default SIGINT
 * handler.  interruptHandler prints the current
 * maxPrime and prompts the user to quit.
 *
 * @param {int} maxPrime - The current highest prime (global scope)
 * @param {int} sigNum - iterrupt ID
 */
int maxPrime = 2;
void interruptHandler(int sigNum) {
  printf("%d\tQuit [y/n]? ", maxPrime);
  fflush(stdout);

  int ch = getchar();
  printf("%c\r\n", ch);

  if (ch == 'y' || ch == 'Y') {
    endwin();
    signal(SIGINT, SIG_DFL);
    raise(SIGINT);
  }
}


/**
 * isPrime tests if a number is prime.  isPrime
 * operates by testing each prime number below
 * n for divisibility.
 *
 * @param {int} n - The prime candidate
 * @param {int[]} primes - All of the primes below n
 * @param {int} length - the length of primes
 * @return {int} one if n is prime, zero if n is composite
 */
int isPrime(int n, int primes[], int length) {
  for (int i = 0; i < length; i += 1) {
    if (n % primes[i] == 0) {
      return 0;
    }
  }

  return 1;
}


int main() {
  // Set interrupt handler
  signal(SIGINT, interruptHandler);

  // Setup curses
  initscr();
  cbreak();

  // Array-backed list implementation
  int size = 2;
  int length = 2;
  int* list = (int*) malloc(sizeof(int) * 2);

  // Seed list with first two primes
  list[0] = 2;
  list[1] = 3;

  // Test all the integers through INT_MAX
  for (int i = 5; i <= INT_MAX; i += 2) {
    if (isPrime(i, list, length)) {
      if (length == size) {
        size *= 2;
        list = realloc(list, sizeof(int) * size);

        if (list == NULL) {
          printf("Out of memory\n");
          return 1;
        }
      }

      maxPrime = i;
      list[length] = i;
      length += 1;
    }
  }

  return 0;
}