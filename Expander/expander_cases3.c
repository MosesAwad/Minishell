/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_cases3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 10:38:16 by mawad             #+#    #+#             */
/*   Updated: 2024/03/03 10:38:16 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//Minus two because I am going to delete from the string
//the $ sign and the '?' mark itself.
static int	expand_exstat_len(char *str, int exit_status)
{
	char	*buffer;
	int		i;
	int		len;

	i = 0;
	len = 0;
	buffer = ft_itoa(exit_status);
	while (str[i])
		i++;
	len = ft_strlen(buffer) + i - 2;
	return (free(buffer), len);
}

static void	copy_extstat_helper(char *str, char *buffer, int *i, int *j)
{
	int	state;

	state = STATE_GENERAL;
	while (str[*i])
	{
		state = choose_state2(str[*i], state);
		if (not_stand_alone(str[*i], str[(*i) + 1], state))
		{
			if (str[(*i) + 1] && str[(*i) + 1] == '?')
			{
				(*i) += 2;
				break ;
			}
		}
		buffer[(*j)++] = str[(*i)++];
	}
}

char	*copy_exit_status(char *str, int exit_status)
{
	char	*buffer;
	char	*extstat_str;
	int		i;
	int		j;
	int		k;

	buffer = (char *)malloc(expand_exstat_len(str, exit_status) + 1);
	if (!buffer)
		return (warn_message(), NULL);
	i = 0;
	j = 0;
	k = 0;
	copy_extstat_helper(str, buffer, &i, &j);
	extstat_str = ft_itoa(exit_status);
	while (extstat_str[k])
		buffer[j++] = extstat_str[k++];
	while (str[i])
		buffer[j++] = str[i++];
	buffer[j] = '\0';
	return (free(extstat_str), buffer);
}

//This function is to handle cases like
//echo "heelo" "$42" which outputs heelo 2.
//Dont worry about it interfering with expansion
//of environment variables, that's because an
//environment variable's name is not supposed to
//start with a number. Thus, if that is the case
char	*delnumber(char *str, int state)
{
	int		i;
	int		j;
	char	*buffer;

	i = 0;
	j = 0;
	buffer = (char *)malloc(ft_strlen(str) - 2 + 1);
	while (str[i])
	{
		state = choose_state2(str[i], state);
		if (str[i] == '$' && state != IN_SQUOTE)
			i += 2;
		buffer[j++] = str[i++];
	}
	buffer[j] = '\0';
	return (buffer);
}
