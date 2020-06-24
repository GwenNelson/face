#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <readline/readline.h>
#include <readline/history.h>

#include <util.h>
#include <types.h>
#include <cmdparse.h>

bool isrunning = true;

static char *cmd_line   = NULL;
static char *prompt_str = NULL;

char* gen_prompt_str() {
      if(prompt_str) {
         free(prompt_str);
	 prompt_str = NULL;
      }
      prompt_str = strdup("$ "); // placeholder prompt for now
      return prompt_str;
}

char* read_cmdline() {
      if(cmd_line) {
         free(cmd_line);
	 cmd_line = NULL;
      }

      prompt_str = gen_prompt_str();
      cmd_line   = readline(prompt_str);
      if(cmd_line && *cmd_line) add_history(cmd_line);
      return cmd_line;
}

pid_t spawn_proc(int in, int out, simple_command_t* cmd) {
    pid_t pid;

    if((pid = fork()) == 0) {
        if(in != 0) {
           dup2(in, 0);
	   close(in);
	}
	if(out != 1) {
	   dup2(out, 1);
	   close(out);
	}
	return execvp(cmd->argv[0], (char* const*)cmd->argv);
     }
}

int fork_pipes(int n, simple_command_t *cmd) {
    int tmpin = dup(0);
    int tmpout = dup(1);
    int i;
    pid_t pid;
    int in, fd[2];
    in = 0;
    for(i=0; i < n-1; i++) {
        pipe(fd);
	spawn_proc(in, fd[1], cmd+i);
	close(fd[1]);
	in = fd[0];
    }
    close(fd[1]);
    if(in != 0) dup2(in,0);
    if((pid = fork()) == 0) {
       execvp(cmd[i].argv[0],(char* const*)cmd[i].argv);
    } else {
       int retval;	
       waitpid(pid,&retval,0);
       dup2(tmpin,0);
       dup2(tmpout,1);
       close(tmpin);
       close(tmpout);
       return retval;
    }
    return 0;
}

void cmd_loop() {
     cmd_line = read_cmdline();
     if(cmd_line) {
        command_t* cmd = parse_cmd(cmd_line);
        fork_pipes(cmd->simple_cmds_count, cmd->simple_cmds);
        free_cmd(cmd);
     }
}

int main(int argc, char** argv) {
    if(isatty(0)) {
       while(isrunning) cmd_loop();
    }
    return 0;
}
