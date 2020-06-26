#include <iostream>
#include <string.h>
#include <stdlib.h>

#include "common.h"

using namespace std;

// Code used from Berkley Parallel Homework
int find_option(int argc, char **argv, const char *option){
    for(int i = 1; i < argc; i++)
        if(strcmp( argv[i], option) == 0 )
            return i;
    return -1;
}

// Code used from Berkley Parallel Homework
char *read_string(int argc, char **argv, const char *option, char *default_value){
    int iplace = find_option(argc, argv, option);
    if(iplace >= 0 && iplace < argc-1)
        return argv[iplace+1];
    return default_value;
}

// Code used from Berkley Parallel Homework
int read_int(int argc, char **argv, const char *option, int default_value){
    int iplace = find_option(argc, argv, option);
    if(iplace >= 0 && iplace < argc-1)
        return atoi(argv[iplace+1]);
    return default_value;
}
