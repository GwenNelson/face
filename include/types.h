#ifndef __TYPES_H_
#define __TYPES_H_

#include <stdbool.h>

typedef struct simple_command_t {
	char*  cmdline;
	char** argv;
} simple_command_t;

typedef struct command_t {
	char*		  cmdline;
	simple_command_t* simple_cmds;
        size_t            simple_cmds_count;
	char*             infile_name;
	char*             outfile_name;
	char*             errfile_name;
	bool              background;
} command_t;

#endif
