#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <cmdparse.h>
#include <util.h>
#include <types.h>

command_t* parse_cmd(char* cmdline) {
    command_t* retval = calloc(1,sizeof(command_t));
    retval->cmdline   = strdup(cmdline);

    retval->simple_cmds       = NULL;
    retval->simple_cmds_count = 0;

    char** pipe_cmds = split_str(retval->cmdline,"|");
    char** s         = pipe_cmds;

    while(*s) {
          retval->simple_cmds_count++;
	  retval->simple_cmds = realloc(retval->simple_cmds,sizeof(simple_command_t)*retval->simple_cmds_count);
	  retval->simple_cmds[retval->simple_cmds_count-1].cmdline = strdup(trim(*s++," "));
	  retval->simple_cmds[retval->simple_cmds_count-1].argv    = split_str(retval->simple_cmds[retval->simple_cmds_count-1].cmdline," ");
    }
    return retval;
}

void free_cmd(command_t* cmd) {
     int i=0;
     for(i=0; i<cmd->simple_cmds_count; i++) {
          free(cmd->simple_cmds[i].cmdline);
     } 
     free(cmd->simple_cmds);
     free(cmd->cmdline);
     free(cmd);
}
