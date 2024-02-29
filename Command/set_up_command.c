/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_up_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 23:33:31 by mawad             #+#    #+#             */
/*   Updated: 2024/02/29 21:29:56 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	void	free_split(char	**buffer)
{
	int	i;

	i = 0;
	while (buffer[i])
		free(buffer[i++]);
	free(buffer);
}

static int	count_args(t_ASTree *rdrnode, t_minishell *shell)
{
	char	*expansion;
	char	**split_expansion;
	int		arg_count;
	int		i;

	arg_count = 0;
	while (rdrnode)
	{
		i = 0;
		expansion = ft_strdup(rdrnode->data);
		expansion = expand_str(expansion, shell->env, shell->exit_status);
		split_expansion = ft_split(expansion, ' ');
		while (split_expansion[i])
		{
			arg_count++;
			i++;
		}
		free(expansion);
		free_split(split_expansion);
		rdrnode = rdrnode->left;
	}
	return (arg_count);
}

static void	copy_cmd_path(char *str, t_minishell *shell, t_command *cmd)
{
	char	*expansion;
	int		len;

	expansion = ft_strdup(str);
	expansion = expand_str(expansion, shell->env, shell->exit_status);
	len = ft_strlen(expansion);
	cmd->cmd_path = (char *)malloc(len + 1);
	ft_strlcpy(cmd->cmd_path, expansion, len + 1);
	free(expansion);
}

void	print_cmd_args(t_command *cmd)
{
	int	i;

	i = 0;
	while (cmd->cmd_args[i])
		printf("%s\n", cmd->cmd_args[i++]);
}

void	set_up_cmd_members(t_ASTree *rdrnode, t_minishell *shell, t_command *cmd)
{
  	char	*expansion;
	char	**split_expansion;
	int		i;
	int		j;

	j = 0;
	copy_cmd_path(rdrnode->data, shell, cmd);
	while (rdrnode)
	{
		i = 0;
		expansion = ft_strdup(rdrnode->data);
		expansion = expand_str(expansion, shell->env, shell->exit_status);
		split_expansion = ft_split(expansion, ' ');
		while (split_expansion[i])
		{
			cmd->cmd_args[j++] = ft_strdup(split_expansion[i]);
			i++;
		}
		free_split(split_expansion);
		rdrnode = rdrnode->left;
	}
	cmd->cmd_args[j] = NULL;
}

void	set_up_command(t_ASTree *rdrnode, t_minishell *shell, t_command *cmd)
{
	int	arg_count;

	
	arg_count = count_args(rdrnode, shell);
	cmd->cmd_args = (char **)malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->cmd_args)
	{
		warn_message();
		return ;
	}
	set_up_cmd_members(rdrnode, shell, cmd);
	// while (rdrnode)
	// {
	// 	if (i == 0)
	// 		copy_cmd_path(rdrnode->data, shell, cmd);
	// 	copy_cmd_args(rdrnode->data, shell, cmd, j++);
	// 	rdrnode = rdrnode->left;
	// 	i++;
	// }
	// cmd->cmd_args[j] = NULL;
}
