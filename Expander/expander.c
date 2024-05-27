/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 10:39:27 by mawad             #+#    #+#             */
/*   Updated: 2024/03/03 10:39:27 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*choose_cases1(int exit_status, char *str, int i)
{
	char	*buffer;
	int		j;

	j = 0;
	if (str[i] && str[i] == '?')
	{
		buffer = copy_exit_status(str, exit_status);
		return (buffer);
	}
	if (str[i] && ft_isdigit(str[i]))
	{
		buffer = delnumber(str, STATE_GENERAL);
		return (buffer);
	}
	while (trailor_validity(str[i + j]) && str[i + j])
		j++;
	if (j == 0)
		return (buffer = ft_strdup(str), buffer);
	return (NULL);
}

static char	*choose_cases2(char *str, char *env[], int s_ind)
{
	char	*buffer;

	buffer = detector(str, s_ind, env);
	if (buffer)
		return (buffer);
	else
	{
		buffer = delpotenvar(str, STATE_GENERAL);
		return (buffer);
	}
	return (NULL);
}

static char	*expand_unit(char *str, char *env[], int exit_status, int state)
{
	char	*buffer;
	int		i;

	if (!str)
		return (NULL);
	i = 0;
	while (str[i])
	{
		state = choose_state2(str[i], state);
		if (not_stand_alone(str[i], str[i + 1], state))
		{
			i++;
			buffer = choose_cases1(exit_status, str, i);
			if (buffer)
				return (free(str), buffer);
			buffer = choose_cases2(str, env, i);
			if (buffer)
				return (free(str), buffer);
			else
				return (printf("returned -> %s\n", str), str);
		}
		i++;
	}
	return (str);
}

//Targets are the $ signs that are meant to be expanded.
//Not every $ sign has to be expanded. To see which ones
//do not get expanded, head on over to expander_utils2.c
//and read the comment above the bool_stand_alone() function.
//It has an explanation.
static int	count_targets(char *str)
{
	int	i;
	int	state;
	int	targets;

	i = 0;
	targets = 0;
	state = STATE_GENERAL;
	while (str[i])
	{
		state = choose_state2(str[i], state);
		if (not_stand_alone(str[i], str[i + 1], state))
			targets++;
		i++;
	}
	return (targets);
}

char	*expansion(char *str, char *env[], int exit_status)
{
	char	*expansion;
	int		i;
	int		target_count;

	if (!str)
		return (NULL);
	expansion = ft_strdup(str);
	i = 0;
	target_count = count_targets(str);
	while (i < target_count)
	{
		expansion = expand_unit(expansion, env,
				exit_status, STATE_GENERAL);
		i++;
	}
	return (expansion);
}
