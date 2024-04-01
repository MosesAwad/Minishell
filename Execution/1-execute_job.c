/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 02:31:37 by mawad             #+#    #+#             */
/*   Updated: 2024/02/22 02:31:37 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_job(t_minishell *shell, t_ASTree *tree)
{
	t_command	*command;
	int			proc_count;

	command = NULL;
	if (!tree)
		return ;
	if (tree->type == NODE_PIPE)
	{
		proc_count = count_procs(shell->ast);
		execute_pipeline(shell, command, proc_count);
	}
	else if (tree->type == NODE_RDRGEN || tree->type == NODE_CMDPATH)
	{
		proc_count = count_procs(shell->ast);
		command = (t_command *)malloc(sizeof(t_command));
		if (!init_command(command, proc_count))
			return ;
		command->stdin_pipe = 0;
		command->stdout_pipe = 0;
		command->pipewrite = 0;
		command->piperead = 0;
		execute_command(shell, command, proc_count);
	}
	remove("hdoc_temp");
}
