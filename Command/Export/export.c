/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 20:15:54 by mawad             #+#    #+#             */
/*   Updated: 2024/03/03 10:49:44 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//The reason why we have if (cmd->stdin_pipe == 1 ||
//cmd->stdout_pipe == 1) check is because in the real
//bash, whenever you have a pipe, the commands are executed
//in subshells. So when you export anywhere where there is 
//a pipe, the environ variable only gets exported to that
//subshell. So for example, with export TROLL=123 | env | grep "TROLL"
//the export TROLL=123 only happened in that subshell. So the subshell
//of env didn't get TROLL=123. Thus, when env outputs its result to
//grep "TROLL" subshell, it would find the environ variables except
//TROLL, because TROLL was only exported to the subshell where
//export TROLL=123 occured. Even the main shell doesn't get it because
//a pipe was present. Export only exports to the main shell if no pipes
//are present.
//To mimic that, we just return in ft_export if there are pipes present,
//because the main process won't get it either.
int	ft_export(t_minishell *shell, t_command *cmd)
{
	shell->exit_status = 0;
	if (cmd->stdin_pipe == 1 || cmd->stdout_pipe == 1)
		return (shell->exit_status);
	if (cmd->cmd_args[1])
		export_w_flags(shell, cmd);
	else
		export_no_flags(shell);
	return (shell->exit_status);
}
