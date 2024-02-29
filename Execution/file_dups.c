
#include "../minishell.h"

static void	in_dup(t_command *cmd)
{
	if (cmd->rdin_f)
	{
		dup2(cmd->fd_in, STDIN_FILENO);
		close(cmd->fd_in);
	}
	else if (cmd->hdoc_amnt)
	{
		dup2(cmd->fd_hdoc, STDIN_FILENO);
		close(cmd->fd_hdoc);
		if (cmd->stdout_pipe)
			dup2(cmd->pipewrite, STDOUT_FILENO);
	}
}

static void	out_dup(t_command *cmd)
{
	if (cmd->rdout_f)
	{
		dup2(cmd->fd_out, STDOUT_FILENO);
		close(cmd->fd_out);
	}
	else if (cmd->fd_apnd)
	{
		dup2(cmd->fd_apnd, STDOUT_FILENO);
		close(cmd->fd_apnd);
	}
}

//For built_in_dup, we do not close the piperead and
//pipewrite fd's because those have to be inherited 
//from the parent process by the next child in the  
//pipeline. I already have proper closing in the
//execute_pipeline and pipe_chain_exec functions.
//Those 'close' calls happen at the right time there. 
//If I close them in here, they would close prematurely
//and the pipe, for the lack of a better term would "break"
//before it gets to the next child process in the pipe.
void	built_in_dup(t_command *cmd)
{
	if (cmd->stdin_pipe || cmd->stdout_pipe)
	{
		if (cmd->stdin_pipe && cmd->in_mode != MODE_HDOC)
			dup2(cmd->piperead, STDIN_FILENO);
		if (cmd->stdout_pipe && cmd->in_mode != MODE_HDOC)
			dup2(cmd->pipewrite, STDOUT_FILENO);
	}
	in_dup(cmd);
	out_dup(cmd);
}

void	regular_dup(t_command *cmd)
{
	if (cmd->stdin_pipe || cmd->stdout_pipe)
	{
		if (cmd->stdin_pipe && cmd->in_mode != MODE_HDOC)
			dup2(cmd->piperead, STDIN_FILENO);
		if (cmd->stdout_pipe && cmd->in_mode != MODE_HDOC)
			dup2(cmd->pipewrite, STDOUT_FILENO);
	}
	in_dup(cmd);
	out_dup(cmd);
	if (cmd->stdin_pipe || cmd->stdout_pipe)
	{
		close(cmd->piperead);
		close(cmd->pipewrite);
		close(cmd->copy_rd_fd);		
	}
	close(cmd->save_stdout);
	close(cmd->save_stdin);
}
