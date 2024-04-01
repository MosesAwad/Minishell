/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_w_flags.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 17:10:49 by mawad             #+#    #+#             */
/*   Updated: 2024/03/03 10:49:15 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//If I have HI=5 then I do export HI, nothing happens.
//For detecting that scenario, go to function bool_no_update()
int	bool_no_update(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] && s2[i])
		i++;
	if (s1[i] == '=' && !s2[i])
		return (1);
	return (0);
}

//applies the bool_no_update to the export arguments
int	check_no_update(char **env_list, char *str)
{
	int	i;

	i = 0;
	while (env_list[i])
	{
		if (bool_no_update(env_list[i], str))
			return (1);
		i++;
	}
	return (0);
}

char	**add_to_loc_list(char **env_list, char *str)
{
	char	**buffer;
	int		i;
	int		j;
	int		len;

	i = 0;
	j = 0;
	while (env_list[i])
		i++;
	buffer = (char **)malloc(sizeof(char *) * (i + 1 + 1));
	i = 0;
	buffer[j++] = ft_strdup(str);
	while (env_list[i])
	{
		len = ft_strlen(env_list[i]);
		buffer[j] = (char *)malloc(len + 1);
		ft_strlcpy(buffer[j++], env_list[i], len + 1);
		free(env_list[i++]);
	}
	buffer[j] = NULL;
	return (free(env_list), buffer);
}

void	export_w_flags(t_minishell *shell, t_command *cmd)
{
	int	i;

	i = 1;
	while (cmd->cmd_args[i])
	{
		if (check_export_syntax(cmd->cmd_args[i]))
		{
			if (check_no_update(shell->env, cmd->cmd_args[i]))
			{
				i++;
				continue ;
			}
			if (check_same_key(shell->env, cmd->cmd_args[i]))
			{
				i++;
				continue ;
			}
			shell->env = add_to_loc_list(shell->env, cmd->cmd_args[i]);
		}
		else
			shell->exit_status = 1;
		i++;
	}
}
