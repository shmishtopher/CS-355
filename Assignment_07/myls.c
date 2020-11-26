/**
 * myls implenetation.  Supports the following
 * command line flags:
 * [-a] Show hidden file
 * [-s] Show files in sorted order
 * [-r] Show files in reverse sorted order
 *
 * @author    Christopher K. Schmitt
 * @version   10.28.2020
 */

#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/ioctl.h>


/**
 * hidden tests a given file for visibility.
 * If a file begins with a "." character then
 * we consider it to be "hidden."
 *
 * @param {char*} entry - The name of the file to test
 * @return {int} 1 if the file is hidden, 0 otherwise
 */
int hidden(char* entry) {
  return entry[0] == '.';
}


/**
 * Wrap `strcmp` in comparators, one with the
 * order of arguments reversed.
 */
int compareForward(const void* a, const void* b) { return strcmp(*(const char**) a, *(const char**) b); }
int compareReverse(const void* a, const void* b) { return strcmp(*(const char**) b, *(const char**) a); }


int main(int argc, char** argv) {
  int A_flag = 0;
  int S_flag = 0;
  int R_flag = 0;

  // Determine the width of the 
  // terminal.
  struct winsize tty;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &tty);

  // Parse arguments to main and set
  // flags. Optind stores the index
  // of the next argument.
  int arg;
  while ((arg = getopt(argc, argv, "asr")) != -1) {
    switch (arg) {
      case 'a': A_flag = 1; break;
      case 's': S_flag = 1; break;
      case 'r': R_flag = 1; break;
    }
  }

  // Open the provided directory if
  // it exists. dir points to null
  // when there is an error.
  DIR* dir = optind == argc
    ? opendir(".")
    : opendir(argv[optind]);

  // Handle errors with `opendir`.
  // Exit if dir is null
  if (dir == NULL) {
    printf("Could not find directory\n");
    return 1;
  }

  // Collect all directory entries
  // into an array for sorting and
  // printing.  Filter out "hidden"
  // files unless A is set.
  int count = 0;
  int maxLength = 0;
  char* buffer[80];
  struct dirent* entry;
  while (entry = readdir(dir)) {
    if (!hidden(entry->d_name) || A_flag) {
      buffer[count] = entry->d_name;
      count += 1;

      int length = strlen(entry->d_name);
      if (length > maxLength) {
        maxLength = length;
      }
    }
  }

  // Sort buffer if either the S flag
  // or the R flag are set.
  if (S_flag) qsort(buffer, count, sizeof(char*), compareForward);
  if (R_flag) qsort(buffer, count, sizeof(char*), compareReverse);


  // Print out each entry into its column
  int columns = tty.ws_col / maxLength;
  int rows = ceil((double) count / columns);
  
  for (int i = 0; i < rows; i += 1) {
    for (int j = 0; j < columns; j += 1) {
      if (j * rows + i < count) {
        if (j == columns - 1) printf("%s", buffer[j * rows + i]);
        else printf("%-*s  ", maxLength, buffer[j * rows + i]);
      }
    }
    printf("\n");
  }

  return 0;
}