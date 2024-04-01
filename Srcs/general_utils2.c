/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 09:13:21 by mawad             #+#    #+#             */
/*   Updated: 2024/03/04 17:51:38 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//is_sg_wspace means if it is a whitespace
//a in state_general (meaning not located
//within single quotes or double quotes)
static int	is_sg_wspace(char c, int state)
{
	if (state != STATE_GENERAL)
		return (0);
	if ((c >= 9 && c <= 13) || (c == 32))
		return (1);
	return (0);
}

static int	get_size(char *str)
{
	int	i;
	int	state;
	int	size;

	i = 0;
	size = 0;
	state = STATE_GENERAL;
	while (str[i])
	{
		while (str[i] && is_sg_wspace(str[i], state))
			state = choose_state2(str[i++], state);
		if (str[i] && state == STATE_GENERAL)
			size++;
		else if (str[i] && state != STATE_GENERAL)
		{
			while (str[i] && state != STATE_GENERAL)
				state = choose_state2(str[i++], state);
			if (str[i])
				size++;
		}
		while (str[i] && !is_sg_wspace(str[i], state))
			state = choose_state2(str[i++], state);
	}
	return (size);
}

static char	**pt_split(char	*str, int state, int size)
{
	int		i;
	char	**buffer;
	int		j;
	int		k;

	i = 0;
	k = 0;
	buffer = (char **)malloc(sizeof(char *) * (size + 1));
	while (str[i])
	{
		if (str[i] && is_sg_wspace(str[i], state))
			state = choose_state2(str[i++], state);
		else
		{
			j = 0;
			while (str[i + j] && !is_sg_wspace(str[i + j], state))
				state = choose_state2(str[i + (j++)], state);
			buffer[k] = (char *)malloc(j + 1);
			ft_strlcpy(buffer[k++], str + i, j + 1);
			i = i + j;
		}
	}
	buffer[size] = NULL;
	return (buffer);
}

//If there are quotes, don't split because I want
//to keep the spaces! The reason why I want to split
//is because if an environment variable had spaces in
//it, then bash would interpret them as different
//arguments. So $HEY="hi lol", then command $HEY
//would have 2 arguments, hi and lol. Not just one.
//But I am using this function for all arguments,
//regardless of whether they environemnt variables
// to be expanded or not.
//So, in that case, if I had an argument like
//echo "hello world . " where there are no environ
//variables, then I don't want it to actually
//split it into multiple arguments, that's actually
//considered as one argument. So it shouldn't be
//split. The proof that it doesn't get split is when
//you do echo "hello world . " | cat -e, then the
//output would be [hello world . $] meaning that the
//space was left!
//I also had to check if the expanded string is an empty string.
//Because if it is, then ft_split would return a NULL double
//pointer. I do not want that! If it is am empty string, I
//want a double pointer whose first string is an empty string.
//So in other words, I do not want buffer = NULL, I want
//buffer = {"", NULL}
char	**split_with_quotes(char *str)
{
	char	**buffer;
	int		i;
	int		size;

	buffer = NULL;
	i = 0;
	if (str[i] == '\0')
	{
		buffer = (char **)malloc(sizeof(char *) * (2));
		buffer[0] = ft_strdup(str);
		buffer[1] = NULL;
	}
	else
	{
		size = get_size(str);
		buffer = pt_split(str, STATE_GENERAL, size);
	}
	return (buffer);
}
