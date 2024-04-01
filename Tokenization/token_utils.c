/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 17:26:38 by moses             #+#    #+#             */
/*   Updated: 2024/03/04 15:58:27 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_wspace(char c)
{
	if ((c >= 9 && c <= 13) || (c == 32))
		return (1);
	return (0);
}

void	add_to_list(t_token **lst, t_token *new)
{
	t_token	*ptr;

	if (!lst)
		return ;
	new->next = NULL;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	else
	{
		ptr = *lst;
		while (ptr->next != NULL)
		{
			ptr = ptr->next;
		}
		ptr->next = new;
	}
}

void	add_node_data(t_token *tok_node, t_tokenType type)
{
	if (type == CHAR_PIPE)
		tok_node->data = ft_strdup("|");
	else if (type == CHAR_HDOC)
		tok_node->data = ft_strdup("<<");
	else if (type == CHAR_APND)
		tok_node->data = ft_strdup(">>");
	else if (type == CHAR_RDIN)
		tok_node->data = ft_strdup("<");
	else if (type == CHAR_RDOUT)
		tok_node->data = ft_strdup(">");
	if (tok_node->data == NULL)
		warn_message();
}

void	delete_list(t_token *token_list)
{
	if (token_list)
		delete_list(token_list->next);
	else
		return ;
	if (token_list->data)
		free(token_list->data);
	free(token_list);
}

// void	print_token_list(t_token *list)
// {
// 	int	i;

// 	i = 0;
// 	while (list != NULL)
// 	{
// 		printf("--TOKEN %d--\n", i);
// 		if (list->type == CHAR_PIPE)
// 			printf("token type is: CHAR_PIPE\n");
// 		else if (list->type == CHAR_SQUOTE)
// 			printf("token type is: CHAR_SQUOTE\n");
// 		else if (list->type == CHAR_DQUOTE)
// 			printf("token type is: CHAR_DQUOTE\n");
// 		else if (list->type == CHAR_RDIN)
// 			printf("token type is: CHAR_RDIN\n");
// 		else if (list->type == CHAR_RDOUT)
// 			printf("token type is: CHAR_RDOUT\n");
// 		else if (list->type == CHAR_HDOC)
// 			printf("token type is: CHAR_HDOC\n");
// 		else if (list->type == CHAR_APND)
// 			printf("token type is: CHAR_APND\n");
// 		else if (list->type == CHAR_GEN)
// 			printf("token type is: CHAR_GEN\n");
// 		printf("token data is: %s\n", list->data);
// 		printf("\n");
// 		list = list->next;
// 		i++;
// 	}
// }
