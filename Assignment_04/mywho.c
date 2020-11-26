/**
 * mywho is a utility to read and display
 * UTMP records.  Usage ```mywho```
 *
 * @author      Christopher K. Schmitt
 * @version     9.30.2020
 */


#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <utmp.h>
#include <time.h>


int main() {
  int fd;
  long time;
  char buffer[60];
  struct utmp log;

  // Open the UTMP file for reading
  // Handle any errors in opening
  if ((fd = open(UTMP_FILE, O_RDONLY)) >= 0) {
    // Read UTMP file into `utmp struct` until the file
    // cursor is past the end of the file
    while (read(fd, &log, sizeof(struct utmp))) {
      time = log.ut_tv.tv_sec;
      strftime(buffer, 60, "%F %R", localtime(&time));
      printf("%s\t%s\t%s\t(%s)\n", log.ut_user, log.ut_line, buffer, log.ut_host);
    }
  }
  else {
    printf("Faliled to read utmp file");
  }
}