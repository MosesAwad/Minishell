/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 20:51:22 by mawad             #+#    #+#             */
/*   Updated: 2024/02/28 22:53:48 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../minishell.h"

int	count_exit_args(t_command *cmd)
{
	int	i;

	i = 0;
	if (!cmd->cmd_args)
		return (0);
	while (cmd->cmd_args[i])
		i++;
	return (i - 1);
}

int	bool_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] && ft_isdigit(str[i]))
		i++;
	if (!str[i])
		return (1);
	return (0);
}

void	full_free(t_minishell *shell, t_command *cmd)
{
	free(shell->cmd_line);
	delete_command(cmd);
	delete_list(shell->tok_list);
	delete_node(shell->ast);
	delete_binlist(shell->binlist);
	delete_env_list(shell);
}

//we still exit no matter if the arguments are correct
//or not. But the exit status would differ
int	ft_exit(t_minishell *shell, t_command *cmd)
{
	unsigned char	status;
	int				arg_count;

	arg_count = count_exit_args(cmd);
	status = 0;
	if (arg_count > 1)
	{
		if (bool_numeric(cmd->cmd_args[1]))
		{
			printf("-minishell: exit: too many arguments\n");
			status = 1;
		}
		else
		{
			printf("-minishell: exit: %s: numeric argument required\n",
				cmd->cmd_args[1]);
			status = 255;
		}
	}
	else if (arg_count == 1)
	{
		if (bool_numeric(cmd->cmd_args[1]))
			status = (unsigned char) ft_atoi(cmd->cmd_args[1]);
		else
		{
			printf("-minishell: exit: %s: numeric argument required\n",
				cmd->cmd_args[1]);
			status = 255;
		}
	}
	printf("exit\n");
	return (shell->exit_status = status, full_free(shell, cmd),
		exit(status), status);
}
