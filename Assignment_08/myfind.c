/**
 * myfind implementation.  myfind accepts
 * the following parameters:
 * ```
 * $ myfind filename [startpath]
 * ```
 * When a matching file is found, myfind
 * prints its full path and mode.
 *
 * @author    Christopher K. Schmitt
 * @version   11.4.2020
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>


/**
 * modeToString turns a file mode into a human
 * readable string.  The string shows user,
 * group, and other permissions.
 *
 * @param {int} mode - the filemode to convert
 * @param {char*} str - an array of bytes to write to
 * @return {char*} a refrence to str
 */
char* modeToString(int mode, char str[]) {
  strcpy(str, "----------");

  if (S_ISDIR(mode)) str[0] = 'd';
  if (S_ISCHR(mode)) str[0] = 'c';
  if (S_ISBLK(mode)) str[0] = 'b';
  if (S_ISLNK(mode)) str[0] = 'l';

  // User bits
  if (mode & S_IRUSR) str[1] = 'r';
  if (mode & S_IWUSR) str[2] = 'w';
  if (mode & S_IXUSR) str[3] = 'x';

  // Group bits
  if (mode & S_IRGRP) str[4] = 'r';
  if (mode & S_IWGRP) str[5] = 'w';
  if (mode & S_IXGRP) str[6] = 'x';

  // Other bits
  if (mode & S_IROTH) str[7] = 'r';
  if (mode & S_IWOTH) str[8] = 'w';
  if (mode & S_IXOTH) str[9] = 'x';

  return str;
}


/**
 * search recursivly traverses the directory
 * tree.  If search encounters an entry that
 * matches the search term, it prints it to
 * standard output.
 *
 * @param {char*} path - the path of the directory to search
 * @param {char*} term - the text to search for in a file name
 */
void search(char* path, char* term) {
  DIR* dir = opendir(path);

  if (dir == NULL) {
    perror("Cannot open directory");
  }

  char* subpaths[256];
  int subpathCount = 0;
  int firstFlag = 1;
  struct dirent* entry;

  while ((entry = readdir(dir)) != NULL) {
    // Filter out current and parent directory
    // entries
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    // Test to see if there is a substring match.
    // If there is, print out the entry.  If no
    // match is found, and the entry is a
    // directory, colect it in a list.
    if (entry->d_type != DT_DIR) {
      if (strstr(entry->d_name, term) != NULL) {
        if (firstFlag) {
          printf("%s\n", path);
          firstFlag = 0;
        }

        struct stat info;
        char buffer[1024];

        strcpy(buffer, path);
        strcat(buffer, "/");
        strcat(buffer, entry->d_name);

        stat(buffer, &info);

        printf("\t%s", entry->d_name);
        printf("\t(%d/%s)\n", info.st_mode, modeToString(info.st_mode, buffer));
      }
    }
    else {
      subpaths[subpathCount] = entry->d_name;
      subpathCount += 1;
    }
  }

  // For each path collected in the subpaths
  // list, perform search
  for (int i = 0; i < subpathCount; i += 1) {
    char buffer[1024];
    
    strcpy(buffer, path);
    strcat(buffer, "/");
    strcat(buffer, subpaths[i]);

    search(buffer, term);
  }

  // Clean up the open directory
  closedir(dir);
}


int main(int argc, char** argv) {
  char buffer[1024];

  if (argc == 2) {
    search(realpath(".", buffer), argv[1]);
  }
  else if (argc == 3) {
    search(realpath(argv[2], buffer), argv[1]);
  }
  else {
    printf("Incorrect number of arguments, see usage\n");
    return 1;
  }

  return 0;
}