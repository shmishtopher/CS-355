/**
 * @author    Christopher K. Schmitt
 * @version   9.16.2020
 *
 * A simple Fizzbuzz program - takes as input from the
 * user a single number and then prints the range
 * between that number and zero.  If a number is a
 * multiple of 3, we instead output "fizz."  If a number
 * is a multiple of 5, we output "buzz."  In the case
 * that a number is a multiple of both 3 and 5, we
 * output "fizzbuzz."
 */


#include <stdio.h>


/**
 * fizzbuz takes a single integer value and
 * prints the correct string to standard
 * output as a side-effect
 *
 * @param {int} n - an interger value that my be divisible by 3, 5, or 15
 */
void fizzbuzz(int n) {
  if (n % 15 == 0) printf("FizzBuzz\n");
  else if (n % 3 == 0) printf("Fizz\n");
  else if (n % 5 == 0) printf("Buzz\n");
  else printf("%d\n", n);
}


int main() {
  int limit = 0;

  printf("Please enter an integer: ");
  scanf("%d", &limit);

  if (limit > 0) {
    for (int i = 0; i <= limit; i += 1) {
      fizzbuzz(i);
    }
  }
  else {
    for (int i = 0; i >= limit; i -= 1) {
      fizzbuzz(i);
    }
  }

  return 0;
}

