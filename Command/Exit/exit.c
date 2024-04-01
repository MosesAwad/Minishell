/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 20:51:22 by mawad             #+#    #+#             */
/*   Updated: 2024/03/04 21:26:30 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	handle_multi_arg(t_command *cmd)
{
	if (bool_numeric(cmd->cmd_args[1]))
	{
		ft_dprintf(2, "-minishell: exit: too many arguments\n");
		return (1);
	}
	else
	{
		ft_dprintf(2, "-minishell: exit: %s: numeric argument required\n",
			cmd->cmd_args[1]);
		return (2);
	}
}

static int	handle_one_arg(t_command *cmd)
{
	if (bool_numeric(cmd->cmd_args[1]))
		return ((unsigned char) ft_atoi(cmd->cmd_args[1]));
	else
	{
		ft_dprintf(2, "-minishell: exit: %s: numeric argument required\n",
			cmd->cmd_args[1]);
		return (2);
	}
}

//we still exit no matter if the arguments are correct
//or not. But the exit status would differ
int	ft_exit(t_minishell *shell, t_command *cmd)
{
	unsigned char	status;
	int				arg_count;

	arg_count = count_exit_args(cmd);
	status = 0;
	ft_dprintf(1, "exit\n");
	if (arg_count > 1)
	{
		status = handle_multi_arg(cmd);
		if (status == 1)
		{
			shell->exit_status = status;
			return (status);
		}
	}
	else if (arg_count == 1)
		status = handle_one_arg(cmd);
	return (shell->exit_status = status, full_free(shell, cmd),
		exit(status), status);
}

// //we still exit no matter if the arguments are correct
// //or not. But the exit status would differ
// int	ft_exit(t_minishell *shell, t_command *cmd)
// {
// 	unsigned char	status;
// 	int				arg_count;

// 	arg_count = count_exit_args(cmd);
// 	status = 0;
// 	if (arg_count > 1)
// 	{
// 		if (bool_numeric(cmd->cmd_args[1]))
// 		{
// 			ft_dprintf(2, "-minishell: exit: too many arguments\n");
// 			status = 1;
// 		}
// 		else
// 		{
// 			ft_dprintf(2, "-minishell: exit: %s: numeric argument required\n",
// 				cmd->cmd_args[1]);
// 			status = 255;
// 		}
// 	}
// 	else if (arg_count == 1)
// 	{
// 		if (bool_numeric(cmd->cmd_args[1]))
// 			status = (unsigned char) ft_atoi(cmd->cmd_args[1]);
// 		else
// 		{
// 			ft_dprintf(2, "-minishell: exit: %s: numeric argument required\n",
// 				cmd->cmd_args[1]);
// 			status = 2;
// 		}
// 	}
// 	ft_dprintf(1, "exit\n");
// 	return (shell->exit_status = status, full_free(shell, cmd),
// 		exit(status), status);
// }
