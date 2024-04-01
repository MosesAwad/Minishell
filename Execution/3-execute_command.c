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

//This function is used when we want to execute a command that
//is not part of a pipeline. 
void	execute_command(t_minishell *shell, t_command *command, int proc_count)
{
	t_ASTree	*tree;
	int			stat_flag;

	tree = shell->ast;
	stat_flag = 1;
	if (tree->type == NODE_RDRGEN)
	{
		if (!redir_handling(tree, shell, command))
		{
			delete_command(command);
			return ;
		}
		if (tree->left)
		{
			set_up_command(tree->left, shell, command);
			stat_flag = exec_smpl_command(shell, command);
		}
	}
	else if (tree->type == NODE_CMDPATH)
	{
		set_up_command(tree, shell, command);
		stat_flag = exec_smpl_command(shell, command);
	}
	waiter(shell, command, proc_count, stat_flag);
	delete_command(command);
}
