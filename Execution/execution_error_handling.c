/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_error_handling.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 11:30:40 by mawad             #+#    #+#             */
/*   Updated: 2024/03/04 12:32:26 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//This function is to check if the user has already inserted
//a path into the commandline. In that case, no need to go to
//check access. So if a user says /bin/ls, then that just run it
//straight to execve. If the command exists (and it is not a directory),
//then execve would ismply execute it, if it does not exist (and it is
//not a directory), then execve would simply return -1. But if the user
//just says something like ls, then the command might actually exist so
//lets prepend a path form the path_list in the check_access function till
//we find a match. If we find a match, then we return the full path and run
//it through execve (full path means prepending the path to the command given
//by the user likes this: the path/command_name). If we do not find a match,
//then that command does not exist and the my check_access function simply
//returns NULL which gets passed to execve and returns a -1.
static int	bool_path_given(char *str)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (str[i] && str[i] != '/')
		i++;
	if (str[i])
		flag = 1;
	return (flag);
}

static int	check_executable_validity(t_minishell *shell, t_command *cmd)
{
	int	result;

	result = 0;
	if (!ft_strchr(cmd->cmd_full_path, '/'))
		return (result = 0, 0);
	if (access(cmd->cmd_full_path, F_OK) == 0)
	{
		if (access(cmd->cmd_full_path, X_OK) == -1)
		{
			ft_dprintf(2, "-minishell: %s: Permission denied\n",
				cmd->cmd_path);
			shell->exit_status = 126;
			result = 1;
		}
	}
	else
	{
		ft_dprintf(2, "-minishell: %s: No such file or directory\n",
			cmd->cmd_path);
		shell->exit_status = 127;
		result = 1;
	}
	return (result);
}

int	no_permissions(t_minishell *shell, t_command *cmd)
{
	int	result;

	result = 0;
	if (bool_path_given(cmd->cmd_path))
		cmd->cmd_full_path = ft_strdup(cmd->cmd_path);
	else
		cmd->cmd_full_path = check_access(shell, cmd);
	if (!(cmd->cmd_full_path))
		result = 0;
	result = check_executable_validity(shell, cmd);
	if (result == 1)
		cmd->pids[(cmd->pid_i)++] = -2;
	return (result);
}
