/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_case_handler.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 23:53:37 by mawad             #+#    #+#             */
/*   Updated: 2024/02/29 18:14:46 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	set_state(char quote)
{
	int	state;

	state = STATE_GENERAL;
	if (quote == CHAR_SQUOTE)
		state = IN_SQUOTE;
	else if (quote == CHAR_DQUOTE)
		state = IN_DQUOTE;
	return (state);
}

//white spaces are only allowed in between quotes so I need to handle that scenario
int	general_case_handler(t_token *tok_node, char *str, int j)
{
	int	state;
	int	quote;

	state = STATE_GENERAL;
	while (str[j] && ((!is_wspace(str[j])) && state == STATE_GENERAL))
	{
		if (str[j] == CHAR_SQUOTE || str[j] == CHAR_DQUOTE)
		{
			quote = str[j++];
			state = set_state(quote);
			while (str[j] && (str[j] != quote))
				j++;
			if (str[j])
				state = STATE_GENERAL;
			else
				break ;
		}
		j++;
	}
	tok_node->data = (char *)malloc(1 * (j + 1));
	if (!tok_node->data)
		return (warn_message(), j);
	ft_strlcpy(tok_node->data, str, j + 1);
	return (j);
}
