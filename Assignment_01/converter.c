/**
 * @author 	  Christopher K. Schmitt
 * @version	  9.1.2020
 *
 * A grade conversion utility - takes as input a single
 * number representing a grade and returns the correct
 * corresponding alphabetic character 
 */


#include <stdio.h>


char toLetterGrade(int grade) {
  if (grade >= 90) return 'A';
  if (grade >= 80) return 'B';
  if (grade >= 70) return 'C';
  if (grade >= 60) return 'D';
  return 'F';
}


int main() {
  int grade = 0;

  printf("Please enter a numeric grade: ");

  if (scanf("%d", &grade) == 0) {
    printf("Your input could not be interpreted as an integer value.\n");
    return 1;
  }

  if (grade <= 100 && grade >= 0) {
    printf("Your grade is a %c\n", toLetterGrade(grade));
  }
  else {
    printf("You entered an invalid value.\n");
    return 1;
  }

  return 0;
}
