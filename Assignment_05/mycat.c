/**
 * mycat is a utility to dump the
 * contents of a file to standard
 * output.
 *
 * Usage:
 * [-b] Number the non-blank output lines, starting at 1.
 * [-e] Display a dollar sign ('$') at the end of each line.
 * [-n] Number the output lines, starting at 1.
 * [-s] Squeeze (remove) all empty lines.
 *
 * @author    Christopher K. Schmitt
 * @version   10.7.2020
 */


#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>


/**
 * isEmpty takes as input a pointer to
 * a string and scans the string for
 * any non-whitespace characters,
 * returning "0" if it finds one and "1"
 * otherwise.
 *
 * @param {char*} string - the string to scan for whitespace
 * @returns {int} - `1` on true and `0` on false
 */
int isEmpty(char* string) {
  for (int i = 0; string[i] != '\0'; i += 1) {
    if (!isspace(string[i])) {
      return 0;
    }
  }

  return 1;
}


/**
 * getChar reads a single byte from an
 * open file and seeks the file by s
 * single byte.  getChar returns the
 * byte that was read and -1 if there
 * is an error.
 *
 * @param {int} fd - the file descriptor to point to
 * @return {char} the byte that was read (-1 on error)
 */
char getChar(int fd) {
  char symbol = 0;

  if (read(fd, &symbol, 1) != 0) {
    return symbol;
  }
  else {
    return -1;
  }
}


/**
 * readLine reads a full line (delimited 
 * by newlines) into a char buffer.
 * readLine return -1 on the end of file.
 *
 * @param {int} fd - the file descriptor to point to
 * @param {char*} buffer - the buffer ro read into
 */
char readLine(int fd, char* buffer) {
  int offset = 0;
  char symbol = 0;

  symbol = getChar(fd);
  while (symbol != '\n' && symbol != -1 && symbol != 0) {
    buffer[offset] = symbol;
    offset += 1;
    symbol = getChar(fd);
  }

  if (buffer[offset - 1] == '\r') {
    buffer[offset - 1] = '\0';
  }
  else {
    buffer[offset] = '\0';
  }

  return symbol;
}


int main(int argc, char** argv) {
  int b_flag = 0;  // [-b]
  int e_flag = 0;  // [-e]
  int n_flag = 0;  // [-n]
  int s_flag = 0;  // [-s]

  char opt;
  while ((opt = getopt(argc, argv, "bens")) != -1) {
    switch (opt) {
      case 'b': b_flag = 1; break;
      case 'e': e_flag = 1; break;
      case 'n': n_flag = 1; break;
      case 's': s_flag = 1; break;
    }
  }

  // Loop over each file in the arg list
  for (int i = b_flag + e_flag + n_flag + s_flag == 0 ? 1 : 2; i < argc; i += 1) {
    int fd;
    int lineCount_a = 1;
    int lineCount_b = 1;
    char line[300];
    char symbol = 0;

    // Open the file for reading
    if ((fd = open(argv[i], O_RDONLY)) != -1) {
      // Process the file line-by-line
      while (symbol = readLine(fd, line)) {
        if (s_flag && isEmpty(line)) {
          if (symbol == -1) break;
          continue;
        }
        if (n_flag) printf("%d ", lineCount_a++);
        if (b_flag && !isEmpty(line)) printf("%d ", lineCount_b++);

        if (e_flag) printf("%s$\n", line);
        else printf("%s\n", line);

        if (symbol == -1) break;
      }
    }
    else {
      printf("Failed to open file: %s\n", argv[i]);
    }
  }

  return 0;
}