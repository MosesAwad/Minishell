/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 10:47:01 by mawad             #+#    #+#             */
/*   Updated: 2024/03/03 10:47:01 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//The reason why I have cmd->cmd_args[i][0] != '\0'
//condition (where i = 0 because I am doing this
//before i gets incremented, meaning I am checking the
//first argument) is because if the user passes [echo "" hi]
//into the commandline argument, then
//!ft_strncmp("-n", cmd->cmd_args[i], ft_strlen(cmd->cmd_args[i]))
//would return 0 eveb though "-n" and "" don't match up. That's
//because ft_strlen(cmd->cmd_args[i]) would end up being 0 and
//so, ft_strncmp returns 0 and it would seem like a match with
//"-n" but it is not!
int	ft_echo(t_minishell *shell, t_command *cmd)
{
	int		i;
	int		flag;

	i = 1;
	flag = 0;
	if (cmd->cmd_args[i] && cmd->cmd_args[i][0]
		&& !ft_strncmp("-n", cmd->cmd_args[i],
			ft_strlen(cmd->cmd_args[i])))
	{
		flag = 1;
		i++;
	}
	while (cmd->cmd_args[i])
	{
		ft_dprintf(1, "%s", cmd->cmd_args[i]);
		if (cmd->cmd_args[i + 1])
			ft_dprintf(1, " ");
		i++;
	}
	if (!flag)
		ft_dprintf(1, "\n");
	return (shell->exit_status = 0, 0);
}
