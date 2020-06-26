#ifndef COMMON_H
#define COMMON_H

// Command line processing
int find_option(int argc, char **argv, const char *option);
char *read_string(int argc, char **argv, const char *option, char *default_value);
int read_int(int argc, char **argv, const char *option, int default_value);

#endif