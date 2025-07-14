#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#ifdef _WIN32
    #define popen  _popen
    #define pclose _pclose
    #define SHELL  "cmd.exe /C"
#else
    #define SHELL  "sh -c"
#endif
int shell(const char *cmd);
extern char *get_variable_value(const char *name);
int hnd_shell(char **argv, size_t argc)
{
    char *command = malloc(argc*(strlen(argv[1])));

    size_t command_size = (argc*(strlen(argv[1])));
    if(command == NULL)
    {
        return -1;
    }
    memset(command,0,command_size);
    for (size_t i = 1; i < argc; i++)
    {
        char *part = NULL;
        if(argv[i][0] == '$')
        {
           part =  get_variable_value(argv[i]+1);
        }
        else
        {
            part = argv[i];
        }
        if(strlen(part)+strlen(command) >=command_size)
        {
            command_size+=command_size;
            char *temp = realloc(command,command_size);
            command = temp;
        }
        // printf("PART: %s\n",part);
        strcat(command," ");
        strcat(command,part);
        // printf("COMMAND: %s\n",command);
    }
    int ret = shell(command);
    free(command);
    return ret;
}

int shell(const char *cmd) {
    char full_cmd[1024];
    snprintf(full_cmd, sizeof(full_cmd), "%s \"%s\"", SHELL, cmd);

    FILE *fp = popen(full_cmd, "r");
    if (fp == NULL) {
        perror("popen failed");
        return -1;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer); // or log, etc.
    }

    int status = pclose(fp);
    if (status != 0) {
        fprintf(stderr, "Command exited with status %d\n", status);
    }
    return status;
}
