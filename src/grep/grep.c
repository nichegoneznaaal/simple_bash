#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>

#define BUFSIZE 1024

int grep(int argc, char **argv);

int main(int argc, char **argv) {
  grep(argc, argv);
  return 0;
}

int grep(int argc, char **argv) {
  if (argc < 3) {
    fprintf(stderr, "Use more option");
    return (1);
  }

  int f = 0, eflag = 0, iflag = 0, vflag = 0, cflag = 0, lflag = 0, nflag = 0,
      c = 0, hflag = 0;
  char pattern[BUFSIZE] = {};
  char buffer[BUFSIZE] = {};
  int currentFile = 0;
  int flag_file = 0;
  regex_t regex;
  int reti = 0;
  int flags = 0;

  const char *short_options = "e:ivclnh";

  const struct option long_options[] = {{NULL, 0, NULL, 0}};

  int option_index = -1;
  while ((f = getopt_long(argc, argv, short_options, long_options,
                          &option_index)) != -1) {
    switch (f) {
      case 'e':
        eflag = 1;
        if (c == 0) {
          strcat(pattern, optarg);
          c++;
        } else {
          strcat(pattern, "|");
          strcat(pattern, optarg);
        }
        flags = 1;
        break;
      case 'i':
        iflag = 1;
        flags = 1;
        break;
      case 'v':
        vflag = 1;
        flags = 1;
        break;
      case 'c':
        cflag = 1;
        flags = 1;
        break;
      case 'l':
        lflag = 1;
        flags = 1;
        break;
      case 'n':
        nflag = 1;
        flags = 1;
        break;
      case 'h':
        hflag = 1;
        flags = 1;
        break;
    }
    option_index = -1;
  }
  if (eflag) {
    currentFile = optind;
  } else {
    currentFile = optind + 1;
  }

  if (eflag && (argc - optind >= 2)) {
    flag_file = 1;
  } else if (!eflag && (argc - optind != 2)) {
    flag_file = 1;
  }

  if (!flags) {
    strcat(pattern, argv[optind]);
  } else if (!eflag) {
    strcat(pattern, argv[optind]);
  }

  if (iflag) {
    reti = regcomp(&regex, pattern, REG_ICASE);
  } else {
    reti = regcomp(&regex, pattern, REG_EXTENDED);
  }
  if (reti) {
    fprintf(stderr, "Could not compile regex\n");
    regfree(&regex);
  }
  FILE *fp = NULL;
  while (currentFile < argc) {
    while (currentFile != argc) {
      fp = fopen(argv[currentFile], "r");
      if (fp == NULL) {
        fprintf(stderr, "%s: %s: No such file or directory\n", argv[0],
                argv[currentFile]);
        currentFile++;
      } else {
        break;
      }
    }

    if (fp == NULL) {
      fprintf(stderr, "%s: %s: No such file or directory\n", argv[0],
              argv[currentFile]);
      regfree(&regex);
      return (1);
    }
    int count_str = 0;
    int lineNumber = 1;
    while (1) {
      if (fgets(buffer, BUFSIZE, fp) == NULL) {
        if (feof(fp) != 0) {
          break;
        } else {
          fprintf(stderr, "Errro read file");
          break;
        }
      }
      reti = regexec(&regex, buffer, 0, NULL, 0);
      int length = strlen(buffer);
      if (buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';
      }
      if (!reti) {
        if (!cflag && !lflag) {
          if (!vflag && flag_file && !hflag) {
            fprintf(stdout, "%s:", argv[currentFile]);
          }
          if (nflag && !vflag) {
            fprintf(stdout, "%d:", lineNumber);
          }
          if (!vflag) {
            fprintf(stdout, "%s\n", buffer);
          }
        }
        if (!vflag && (cflag || lflag)) {
          count_str++;
        }
      }
      if (reti) {
        if (vflag) {
          if (!cflag && !lflag) {
            if (flag_file && !hflag) {
              fprintf(stdout, "%s:", argv[currentFile]);
            }
            if (nflag) {
              fprintf(stdout, "%d:", lineNumber);
            }
            fprintf(stdout, "%s\n", buffer);
          }
          if (cflag || lflag) {
            count_str++;
          }
        }
      }
      lineNumber++;
    }
    if (hflag) {
      flag_file = 0;
    }
    if (cflag && flag_file) {
      fprintf(stdout, "%s:", argv[currentFile]);
    }
    if (cflag) {
      if (lflag) {
        if (count_str > 0) {
          fprintf(stdout, "%d\n", 1);
        } else {
          fprintf(stdout, "%d\n", 0);
        }
      } else {
        fprintf(stdout, "%d\n", count_str);
      }
    }
    if (lflag) {
      if (count_str > 0) {
        fprintf(stdout, "%s\n", argv[currentFile]);
      }
    }
    fclose(fp);
    count_str = 0;
    currentFile++;
  }
  regfree(&regex);
  return (0);
}
