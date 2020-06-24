#ifndef __CMDPARSE_H_
#define __CMDPARSE_H_

#include <types.h>

command_t* parse_cmd(char* cmdline);
void       free_cmd(command_t* cmd);

#endif
