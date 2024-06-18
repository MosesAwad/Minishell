/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_dups.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 10:36:36 by mawad             #+#    #+#             */
/*   Updated: 2024/03/03 10:36:36 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

// For built_in_dup, we do not close the piperead and
// pipewrite fd's because those have to be inherited 
// from the parent process by the next child in the  
// pipeline. I already have proper closing in the
// execute_pipeline and pipe_chain_exec functions.
// Those 'close' calls happen at the right time there. 
// If I close them in here, they would close prematurely
// and the pipe, for the lack of a better term would "break"
// before it gets to the next child process in the pipe.
//
// In the regular_dup() function though, we can close the
// piperead and pipewrite fd's because that happens within
// the child process and they are inherited versions of the
// piperead and pipewrite fd's. So, I close them in the child
// process just to avoid any fd leaks. But, once I write to 
// a pipe in regular_dup, read end of the pipe (piperead) is
// still available in the parent process and can give it to
// the next command to read from it. BUT, in built_in_dup, since
// the built_in commands are not executed by child processes 
// (cuz we don't use execve for them) but rather from the parent
// process itself, if I close pipewrite here, I closed the pipe,
// that's it! It's not a copy of the pipe ends, it is the
// pipe itself! So the next command won't be able to read from the 
// pipe cuz piperead is closed.
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

// --------------	 (IGNORE) OUTDATED COMMENT	------------------
// The second if (cmd->stdin_pipe || cmd->stdout_pipe) is needed
// for the last job. It's always best to explain with an example.
// Imagine the cmdline is <infile cat | wc -l >outfile. Job1 would
// use fd = 3 for piperead, fd = 4 for pipewrite, and fd = 5 for
// infile. Then we close pipewrite and infile. But piperead is still
// open because the last job, which is job2 in this case needs to
// read from it. Thus, fd = 3 is still being used. Hence, we get
// fd = 4 for outfile. But now, since I am using this exec_smpl_cmd
// function as a multipurpose function, I always end up closing
// piperead and pipewrite. It is just easier. Now pipewrite has 
// been closed in the parent, that is right. So it was inherited
// closed. But, the thing is, pipewrite's value is still 4! So, I
// end up closing fd = 4, if I didn't have the second 
// if (cmd->stdin_pipe || cmd->stdout_pipe) and just closed all the
// pipe fd's in the first if statement. If I close that fd = 4, 
// then when I get to if (cmd->rdout_f) and attempt to 
// dup2(cmd->fd_out), which also has a value of 4, it wouldn't work;
// because that fd value has been closed already. In fact, dup2 would
// return -1. You can check it if you'd like by saying 
// if (dup2(cmd->fd_out, STDOUT_FILENO) == -1) then 
// printf("dup error\n") and it would dup error.
// --------------	 (IGNORE) OUTDATED COMMENT	------------------
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
