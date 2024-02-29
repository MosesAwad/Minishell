/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 20:32:54 by mawad             #+#    #+#             */
/*   Updated: 2024/02/23 20:52:11 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../minishell.h"

int	check_no_equal(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (str[i] == '=')
		return (0);
	return (1);
}

int	ft_env(t_minishell *shell)
{
	int	i;

	i = 0;
	shell->exit_status = 0;
	while (shell->env[i])
	{
		if (!check_no_equal(shell->env[i]))
			printf("%s\n", shell->env[i]);
		i++;
	}
	return (shell->exit_status = 0, 0);
}
