/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   4-execute_simple_command.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 11:43:39 by mawad             #+#    #+#             */
/*   Updated: 2024/03/04 15:56:05 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	exec_built_ins(t_minishell *shell, t_command *cmd)
{
	cmd->pids[(cmd->pid_i)++] = -2;
	built_in_dup(cmd);
	call_built_ins(shell, cmd);
	dup2(cmd->save_stdout, STDOUT_FILENO);
	dup2(cmd->save_stdin, STDIN_FILENO);
}

//	if (cmd->pids[(cmd->pid_i)++] != 0)
//		return ;
//That line up above is just to exit if you are the parent process.
//The second if (cmd->stdin_pipe || cmd->stdout_pipe) is needed
//for the last job. It's always best to explain with an example.
//Imagine the cmdline is <infile cat | wc -l >outfile. Job1 would
//use fd = 3 for piperead, fd = 4 for pipewrite, and fd = 5 for
//infile. Then we close pipewrite and infile. But piperead is still
//open because the last job, which is job2 in this case needs to
//read from it. Thus, fd = 3 is still being used. Hence, we get
//fd = 4 for outfile. But now, since I am using this exec_smpl_cmd
//function as a multipurpose function, I always end up closing
//piperead and pipewrite. It is just easier. Now pipewrite has 
//been closed in the parent, that is right. So it was inherited
//closed. But, the thing is, pipewrite's value is still 4! So, I
//end up closing fd = 4, if I didn't have the second 
//if (cmd->stdin_pipe || cmd->stdout_pipe) and just closed all the
//pipe fd's in the first if statement. If I close that fd = 4, 
//then when I get to if (cmd->rdout_f) and attempt to 
//dup2(cmd->fd_out), which also has a value of 4, it wouldn't work;
//because that fd value has been closed already. In fact, dup2 would
//return -1. You can check it if you'd like by saying 
//if (dup2(cmd->fd_out, STDOUT_FILENO) == -1) then 
//printf("dup error\n") and it would dup error.
int	exec_smpl_command(t_minishell *shell, t_command *cmd)
{
	if (!cmd->cmd_path)
		return (0);
	cmd->save_stdin = dup(STDIN_FILENO);
	cmd->save_stdout = dup(STDOUT_FILENO);
	if (check_built_ins(cmd))
		return (exec_built_ins(shell, cmd), 0);
	if (check_directory(shell, cmd))
		return (0);
	if (no_permissions(shell, cmd))
		return (0);
	init_child_signals();
	cmd->pids[cmd->pid_i] = fork();
	if (cmd->pids[cmd->pid_i] == -1)
	{
		perror("fork");
		ft_cleanup(shell, cmd);
		exit(1);
	}
	if (cmd->pids[(cmd->pid_i)++] != 0)
		return (1);
	regular_dup(cmd);
	if (execve(cmd->cmd_full_path, cmd->cmd_args, shell->env) == -1)
		handle_execve_error(shell, cmd);
	return (1);
}
