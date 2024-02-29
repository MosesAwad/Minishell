/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builders.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 19:57:59 by mawad             #+#    #+#             */
/*   Updated: 2024/02/23 19:57:59 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	*set_pid_arr(int proc_count)
{
	int	*buffer;

	buffer = (int *)malloc(sizeof(int) * proc_count);
	return (buffer);
}

int	init_command(t_command *command, int proc_count)
{
	if (!command)
		return (warn_message(), 0);
	command->pids = set_pid_arr(proc_count);
	command->pid_i = 0;
	command->delims = NULL;
	command->rdin_f = NULL;
	command->rdout_f = NULL;
	command->append_f = NULL;
	command->files = NULL;
	command->f_amnt = 0;
	command->hdoc_amnt = 0;
	command->fd_in = 0;
	command->fd_hdoc = 0;
	command->fd_out = 0;
	command->fd_apnd = 0;
	command->cmd_path = NULL;
	command->cmd_full_path = NULL;
	command->cmd_args = NULL;
	command->in_mode = NO_MODE;
	command->out_mode = NO_MODE;
	command->save_stdin = 0;
	command->save_stdout = 0;
	return (1);
}

//this function effectively resets the command struct
//execept for command->pids and command->pid_i. Similarly,
//delete_pipe_comand frees everything except members of
//the command struct. Thus, when I have a pipe, I only call
//init_command at the very beginning of the execute_pipeline 
//function and I call delete_command only at the end of the
//execute_pipeline function. Everytime traverse the tree in
//between, I call the init_pipe_command and the delete_pipe_
//command functions. So that everything is freed or reset 
//except for those two members (command->pids and
//command->pid_i). That is because the waiter function at the
//end of execute_pipeline command needs to have the original
//int *pids attribute and the int pid_i iterator from when
//we first got into the execute_pipeline function.
void	init_pipe_command(t_command *command)
{
	command->delims = NULL;
	command->rdin_f = NULL;
	command->rdout_f = NULL;
	command->append_f = NULL;
	command->files = NULL;
	command->f_amnt = 0;
	command->hdoc_amnt = 0;
	command->fd_in = 0;
	command->fd_hdoc = 0;
	command->fd_out = 0;
	command->fd_apnd = 0;
	command->cmd_path = NULL;
	command->cmd_full_path = NULL;
	command->cmd_args = NULL;
	command->in_mode = NO_MODE;
	command->out_mode = NO_MODE;
}
