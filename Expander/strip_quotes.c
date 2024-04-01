/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strip_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 16:48:40 by mawad             #+#    #+#             */
/*   Updated: 2024/02/13 16:48:40 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_quotes(char c)
{
	if (c == CHAR_SQUOTE || c == CHAR_DQUOTE)
		return (1);
	return (0);
}

static int	count_quotes(char *str, int state)
{
	int	i;
	int	count;
	int	quote;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (check_quotes(str[i]) && (state == STATE_GENERAL))
		{
			quote = str[i];
			state = set_state(quote);
			count++;
		}
		else if (check_quotes(str[i]) && (state != STATE_GENERAL))
		{
			if (str[i] == quote)
			{
				state = STATE_GENERAL;
				count++;
			}
		}
		i++;
	}
	return (count);
}

static void	copy_stripped(char *buffer, char *str, int state)
{
	int	i;
	int	j;
	int	quote;

	i = 0;
	j = 0;
	while (str[i])
	{
		if ((str[i] == CHAR_SQUOTE || str[i] == CHAR_DQUOTE)
			&& (state == STATE_GENERAL))
		{
			quote = str[i++];
			state = set_state(quote);
			continue ;
		}
		else if ((str[i] == CHAR_SQUOTE || str[i] == CHAR_DQUOTE)
			&& (state != STATE_GENERAL) && str[i] == quote)
		{
			state = STATE_GENERAL;
			i++;
			continue ;
		}
		buffer[j++] = str[i++];
	}
	buffer[j] = '\0';
}

char	*strip_quotes(char *str)
{
	char	*buffer;
	int		sub_len;

	if (!str)
		return (NULL);
	sub_len = count_quotes(str, STATE_GENERAL);
	buffer = (char *)malloc(1 * (ft_strlen(str) - sub_len + 1));
	if (!buffer)
		return (warn_message(), NULL);
	copy_stripped(buffer, str, STATE_GENERAL);
	return (buffer);
}
