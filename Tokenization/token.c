/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moses <moses@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 16:11:48 by moses             #+#    #+#             */
/*   Updated: 2024/02/08 17:19:18 by moses            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_type(t_token *tok_list, char c1, char c2)
{
	if (c1 == CHAR_PIPE)
		tok_list->type = CHAR_PIPE;
	else if (c1 == CHAR_RDIN && c2 == CHAR_RDIN)
		tok_list->type = CHAR_HDOC;
	else if (c1 == CHAR_RDOUT && c2 == CHAR_RDOUT)
		tok_list->type = CHAR_APND;
	else if (c1 == CHAR_RDIN)
		tok_list->type = CHAR_RDIN;
	else if (c1 == CHAR_RDOUT)
		tok_list->type = CHAR_RDOUT;
	else
		tok_list->type = CHAR_GEN;
}

int	case_set1(t_token **tok_list, t_token *tok_node, char *str, int *i)
{
	if (str[*i] == CHAR_PIPE)
	{
		add_node_data(tok_node, CHAR_PIPE);
		add_to_list(tok_list, tok_node);
		(*i)++;
		return (1);
	}
	else if (str[*i] == CHAR_RDIN && str[*i + 1] == CHAR_RDIN)
	{
		add_node_data(tok_node, CHAR_HDOC);
		add_to_list(tok_list, tok_node);
		(*i) += 2;
		return (1);
	}
	else if (str[*i] == CHAR_RDOUT && str[*i + 1] == CHAR_RDOUT)
	{
		add_node_data(tok_node, CHAR_APND);
		add_to_list(tok_list, tok_node);
		(*i) += 2;
		return (1);
	}
	return (0);
}

int	case_set2(t_token **tok_list, t_token *tok_node, char *str, int *i)
{
	int	j;

	j = 0;
	if (str[*i] == CHAR_RDIN)
	{
		add_node_data(tok_node, CHAR_RDIN);
		add_to_list(tok_list, tok_node);
		(*i) += 1;
		return (1);
	}
	else if (str[*i] == CHAR_RDOUT)
	{
		add_node_data(tok_node, CHAR_RDOUT);
		add_to_list(tok_list, tok_node);
		(*i) += 1;
		return (1);
	}
	else
	{
		j = general_case_handler(tok_node, str + (*i), j);
		add_to_list(tok_list, tok_node);
		(*i) = (*i) + j;
		return (1);
	}
	return (0);
}

void	tokenizer(t_token **tok_list, char *str)
{
	int		i;
	t_token	*tok_node;

	i = 0;
	tok_node = NULL;
	while (str[i] != '\0')
	{
		if (str[i] && !is_wspace(str[i]))
		{
			tok_node = (t_token *)malloc(sizeof(t_token));
			if (!tok_node)
			{
				add_to_list(tok_list, NULL);
				warn_message();
				return ;
			}
			set_type(tok_node, str[i], str[i + 1]);
			if (!case_set1(tok_list, tok_node, str, &i))
				case_set2(tok_list, tok_node, str, &i);
			continue ;
		}
		i++;
	}
}
