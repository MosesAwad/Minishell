/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 19:27:35 by mawad             #+#    #+#             */
/*   Updated: 2024/03/04 12:15:30 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_in_binlist(t_minishell *shell, char *str)
{
	t_binlist	*binlist;
	int			i;
	int			j;

	binlist = shell->binlist;
	i = 0;
	while (i < 6)
	{
		j = 0;
		while (binlist[i].sublist && binlist[i].sublist[j])
		{
			if (!strncmp(binlist[i].sublist[j++], str, ft_strlen(str + 1)))
				return (1);
		}
		i++;
	}
	return (0);
}

int	check_built_ins(t_command *command)
{
	char	*cmd;
	int		len;

	if (!command->cmd_args || !command->cmd_args[0])
		return (0);
	cmd = command->cmd_args[0];
	len = ft_strlen(cmd);
	if (!ft_strncmp(cmd, "echo", len))
		return (1);
	if (!ft_strncmp(cmd, "cd", len))
		return (1);
	if (!ft_strncmp(cmd, "pwd", len))
		return (1);
	if (!ft_strncmp(cmd, "export", len))
		return (1);
	if (!ft_strncmp(cmd, "unset", len))
		return (1);
	if (!ft_strncmp(cmd, "env", len))
		return (1);
	if (!ft_strncmp(cmd, "exit", len))
		return (1);
	return (0);
}

int	call_built_ins(t_minishell *shell, t_command *command)
{
	char	*cmd;
	int		len;

	cmd = command->cmd_args[0];
	len = ft_strlen(cmd);
	if (!ft_strncmp(cmd, "echo", len))
		return (ft_echo(shell, command), 1);
	if (!ft_strncmp(cmd, "cd", len))
		return (ft_cd(shell, command), 1);
	if (!ft_strncmp(cmd, "pwd", len))
		return (ft_pwd(shell), 1);
	if (!ft_strncmp(cmd, "export", len))
		return (ft_export(shell, command), 1);
	if (!ft_strncmp(cmd, "unset", len))
		return (ft_unset(shell, command), 1);
	if (!ft_strncmp(cmd, "env", len))
		return (ft_env(shell), 1);
	if (!ft_strncmp(cmd, "exit", len))
		return (ft_exit(shell, command), 1);
	return (0);
}

void	waiter(t_minishell *shell, t_command *cmd,
				int proc_count, int stat_flag)
{
	int	i;
	int	save_status;

	i = 0;
	save_status = 0;
	while (i < proc_count)
	{
		if (cmd->pids[i] == -2)
		{
			i++;
			continue ;
		}
		waitpid(cmd->pids[i], &save_status, 0);
		i++;
	}
	if (stat_flag != 0)
	{
		if (WIFSIGNALED(save_status))
			save_status = 128 + WTERMSIG(save_status);
		else if (WIFEXITED(save_status))
			save_status = WEXITSTATUS(save_status);
		shell->exit_status = save_status;
	}
}
