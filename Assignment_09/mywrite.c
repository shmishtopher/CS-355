#include <stdio.h>
#include <unistd.h>
#include <utmp.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>


/**
 * Fix `nonstring` warning by sanitizing the
 * ut_line field.  Adding a null terminator
 * guarantees a valid string.
 *
 * @param {char*} line - the buffer to sanitize
 * @return {char*} a reference to the line
 */
char* sanitizeLine(char* line) {
  line[UT_LINESIZE - 1] = '\0';
  return line;
}


/**
 * Fix `nonstring` warning by sanitizing the
 * ut_user field.  Adding a null terminator
 * guarantees a valid string.
 *
 * @param {char*} line - the buffer to sanitize
 * @return {char*} a reference to the line
 */
char* sanitizeUser(char* name) {
  name[UT_NAMESIZE - 1] = '\0';
  return name;
}


/**
 * getTTY takes a username and a buffer and
 * searches UTMP records for a username match.
 * getTTy then copues the tty address to the
 * passed buffer.  If the user cannot be found,
 * getTTy returns NULL.
 *
 * @param {char*} user - the username to search for
 * @param {char*} buffer - the buffer to copy the address to
 * @return {char*} a reference to buffer
 */
char* getTTY(char* user, char* buffer) {
  int fd = open(UTMP_FILE, O_RDONLY);
  int found = 0;
  struct utmp log;
  
  if (fd >= 0) {
    while ((read(fd, &log, sizeof(struct utmp)))) {
      if (strcmp(user, sanitizeUser(log.ut_user)) == 0) {
        strcpy(buffer, "/dev/");
        strcat(buffer, sanitizeLine(log.ut_line));
        found = 1;
      }
    }
  }

  if (found) return buffer;

  return NULL;
}


int main(int argc, char** argv) {
  char lineBuffer[1024];
  char ttyFile[1024];
  char hostName[1024];
  char greetingBuffer[2048];
  int tty = -1;

  // Get and format the local system time
  time_t rawTime;
  struct tm* timeInfo;
  char timeBuffer[256];

  time(&rawTime);
  timeInfo = localtime(&rawTime);
  sprintf(timeBuffer, "%02d:%02d", timeInfo->tm_hour, timeInfo->tm_min);

  // Assemble the greeting string
  gethostname(hostName, 1024);
  strcpy(greetingBuffer, "Message from ");
  strcat(greetingBuffer, getlogin());
  strcat(greetingBuffer, "@");
  strcat(greetingBuffer, hostName);
  strcat(greetingBuffer, " on ");
  strcat(greetingBuffer, ttyname(STDIN_FILENO));
  strcat(greetingBuffer, " at ");
  strcat(greetingBuffer, timeBuffer);
  strcat(greetingBuffer, "\n");

  if (argc != 2) {
    fprintf(stderr, "Usage: ./write user\n");
    return -1;
  }
  
  // Loop while there is text left in the buffer
  // Write each line to the target user's terminal
  if (getTTY(argv[1], ttyFile) != NULL) {
    if ((tty = open(ttyFile, O_WRONLY)) >= 0) {      
      write(tty, greetingBuffer, strlen(greetingBuffer));
      while (fgets(lineBuffer, 1024, stdin) != NULL) {
        write(tty, lineBuffer, strlen(lineBuffer));
      }

      write(tty, "EOF\n", 4);
      close(tty);
    }
  }
  else {
    printf("User %s is not logged in\n", argv[1]);
  }
}