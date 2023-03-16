#include <getopt.h>
#include <stdio.h>
#include <string.h>

#define BUFSIZE 1024

void dollar_func(char *buffer);
void V_flag(char *buffer, char *res_buf);

int main(int argc, char **argv) {
  int bflag = 0, eflag = 0, nflag = 0, sflag = 0, Tflag = 0, Eflag = 0,
      tflag = 0, vflag = 0;

  const char *short_options = "+benvstTE";
  const struct option long_options[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, 0, NULL, 0},
  };

  int opt = 0;
  int option_index = -1;

  while ((opt = getopt_long(argc, argv, short_options, long_options,
                            &option_index)) != -1) {
    switch (opt) {
      case 'b':
        bflag = 1;
        break;
      case 'e':
        eflag = 1;
        break;
      case 'n':
        nflag = 1;
        break;
      case 's':
        sflag = 1;
        break;
      case 't':
        tflag = 1;
        break;
      case 'T':
        Tflag = 1;
        break;
      case 'E':
        Eflag = 1;
        break;
      case 'v':
        vflag = 1;
        break;
    }
  }
  char buffer[BUFSIZ] = {};
  char res_buf[BUFSIZ] = {};
  FILE *fp = NULL;
  while (optind < argc) {
    if ((fp = fopen(argv[optind], "r")) == NULL) {
      fprintf(stderr, "%s: %s: No such file or directory", argv[0],
              argv[optind]);
      return (1);
    }
    optind++;
    int lineNumber = 1;
    int lastLineBlank = 0;
    while (1) {
      if (fgets(buffer, BUFSIZE, fp) == NULL) {
        if (feof(fp) != 0) {
          break;
        } else {
          fprintf(stderr, "Errro read file");
          break;
        }
      }

      int length = strlen(buffer);
      if (sflag) {
        length = strlen(buffer);
        int currentLine = (length <= 1) ? 1 : 0;
        if (lastLineBlank && currentLine) {
          continue;
        }
        lastLineBlank = currentLine;
      }

      if (bflag) {
        length = strlen(buffer);
        if (length >= 2) {
          fprintf(stdout, "%6d\t", lineNumber++);
        }
      } else if (nflag) {
        fprintf(stdout, "%6d\t", lineNumber++);
      }
      if (Eflag) {
        dollar_func(buffer);
      }
      if (eflag) {
        dollar_func(buffer);
        V_flag(buffer, res_buf);
      }

      if (vflag) {
        V_flag(buffer, res_buf);
      }
      if (tflag) {
        char *pr = NULL;
        char *p = NULL;
        for (p = buffer, pr = res_buf; *p != '\0'; p++) {
          if (*p < 32) {
            if (*p < 10 || *p > 10) {
              *pr++ = '^';
              *pr++ = *p + 64;
            } else {
              *pr++ = *p;
            }
          } else if (*p == 127) {
            *pr++ = '^';
            *pr++ = '?';
          } else {
            *pr++ = *p;
          }
        }
        *pr = '\0';
      }
      if (Tflag) {
        char *pr = NULL;
        char *p = NULL;
        for (p = buffer, pr = res_buf; *p != '\0'; p++) {
          if (*p == 9) {
            *pr++ = '^';
            *pr++ = 'I';
          } else {
            *pr++ = *p;
          }
        }
        *pr = '\0';
      }
      if (Tflag != 1 && eflag != 1 && tflag != 1 && vflag != 1) {
        fprintf(stdout, "%s", buffer);
      } else {
        fprintf(stdout, "%s", res_buf);
      }
    }
    fclose(fp);
    option_index = -1;
  }

  return (0);
}

void dollar_func(char *buffer) {
  int length = strlen(buffer);
  if (length) --length;
  if (buffer[length] == '\n') {
    buffer[length++] = '$';
    buffer[length++] = '\n';
    buffer[length] = '\0';
  }
}

void V_flag(char *buffer, char *res_buf) {
  char *pr = NULL;
  char *p = NULL;
  for (p = buffer, pr = res_buf; *p != '\0'; p++) {
    if (*p < 32) {
      if (*p == 9) {
        *pr++ = 9;
      } else if (*p == 10) {
        *pr++ = 10;
      } else {
        *pr++ = '^';
        *pr++ = *p + 64;
      }
    } else if (*p == 127) {
      *pr++ = '^';
      *pr++ = '?';
    } else {
      *pr++ = *p;
    }
  }
  *pr = '\0';
}