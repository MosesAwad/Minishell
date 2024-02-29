/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_str_or_toklist.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moses <moses@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 19:00:59 by mawad             #+#    #+#             */
/*   Updated: 2024/02/26 05:13:37 by moses            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//we need this version of the expansion for hdoc because
//hdoc also allows for expansion

char	*expand_str(char *str, char *env[], int exit_status)
{
	str = expansion(str, env, exit_status);
	str = strip_quotes(str);
	return (str);
}

void	expand_toklist(t_token *tok_list, char *env[], int exit_status)
{
	while (tok_list)
	{
		tok_list->data = expansion(tok_list->data, env, exit_status);
		tok_list->data = strip_quotes(tok_list->data);
		tok_list = tok_list->next;
	}
}
