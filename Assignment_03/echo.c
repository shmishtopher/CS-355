/**
 * @author    Christopher K. Schmitt
 * @version   9.23.2020
 *
 * A simple program which takes as input
 * any number of arguments and then echos
 * those arguments out acording to the
 * following rules:
 * - The first character of the first argument on the first line
 * - The first two on the second
 * - The first three on the third
 * - This pattern is repeted for all arguments
 * - Include only alphabetic characters
 */


#include <stdio.h>


/**
 * `validCharacter` takes a single character
 * and returns 1 if that character is
 * alphabetic and false otherwise
 *
 * @param {char} c - The character to test
 * @return {int} a boolean
 */
int validCharacter(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); 
}


/**
 * `staggerString` takes a point to a string
 * and prints it to standard output by applying
 * the rules enumerated at the top of this file
 *
 * @param {char*} s - a pointer to a string
 */
void staggerString(char* s) {
  for (int i = 0; s[i] != '\0'; i += 1) {
    if (validCharacter(s[i])) {
      for (int j = 0; j <= i; j += 1) {
        if (validCharacter(s[j])) {
          printf("%c", s[j]);
        }
      }
      
      printf("\n");
    }
  }
}


int main(int argc, char** argv) {
  for (int i = 1; i < argc; i += 1) {
    staggerString(argv[i]);
  }
}
