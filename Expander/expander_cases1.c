/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_cases1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 10:37:58 by mawad             #+#    #+#             */
/*   Updated: 2024/03/03 10:37:58 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	delpotenvar_len(char *str, int state)
{
	int		i;
	int		len;

	i = 0;
	len = 0;
	while (str[i])
	{
		state = choose_state2(str[i], state);
		if (not_stand_alone(str[i], str[i + 1], state))
		{
			i += 1;
			break ;
		}
		len++;
		i++;
	}
	while (str[i] && trailor_validity(str[i]))
		i++;
	while (str[i++])
		len++;
	return (len);
}

static char	*allocator(char **buffer, int len)
{
	*buffer = (char *)malloc(len + 1);
	if (!(*buffer))
		return (NULL);
	return (*buffer);
}

//THIS FUNCTION IS UTILIZED WHEN WE HAVE A ${ FOLLOWED BY A TRAILOR WHICH
//IS VALID BUT NOT INCLUDED IN ENVAR LIST. THUS, THIS FUNCTION COPIES THE
//STRING WITHOUT THE ${ AND WITHOUT ITS TRAILOR EITHER.
char	*delpotenvar(char *str, int state)
{
	int		i;
	int		j;
	char	*buffer;
	int		len;

	i = 0;
	len = delpotenvar_len(str, STATE_GENERAL);
	if (!allocator(&buffer, len))
		return (warn_message(), NULL);
	j = 0;
	while (str[i])
	{
		state = choose_state2(str[i], state);
		if (not_stand_alone(str[i], str[i + 1], state))
		{
			i += 1;
			break ;
		}
		buffer[j++] = str[i++];
	}
	while (str[i] && trailor_validity(str[i]))
		i++;
	while (str[i])
		buffer[j++] = str[i++];
	return (buffer[j] = '\0', buffer);
}

//DEL DOLLAR ARE STANDALONE DOLLARS THAT HAVE TO BE DELETED. THEY ARE EXPLAINED
//IN THE NOTES OF THE keep_standalone() FUNCTION ABOVE. AN EXAMPLE OF A STAND
//ALONE DOLLAR THAT HAS TO BE DELETED IS [${${lol] or [${${]. IN THESE SCENARIOS
//THE FIRST DOLLAR SIGNS HAVE TO BE DELETED BECAUSE THEY ARE NOT FOLLOWED BY A
//NULL TERMINATOR OR A WHITESPACE, INSTEAD THEY ARE FOLLOWED BY ANOTHER ${.
char	*deldollar(char *str, int state)
{
	int		i;
	int		j;
	char	*buffer;

	buffer = (char *)malloc(1 * (ft_strlen(str) - 1 + 1));
	if (!buffer)
		return (warn_message(), NULL);
	j = 0;
	i = 0;
	while (str[i])
	{
		state = choose_state2(str[i], state);
		if (not_stand_alone(str[i], str[i + 1], state))
		{
			i += 1;
			break ;
		}
		buffer[j++] = str[i++];
	}
	while (str[i])
		buffer[j++] = str[i++];
	return (buffer[j] = '\0', buffer);
}
