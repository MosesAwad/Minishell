/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6-arg_list_level.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moses <moses@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 01:50:29 by moses             #+#    #+#             */
/*   Updated: 2024/02/09 01:50:29 by moses            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_ASTree	*arg_list(t_token **token_list)
{
	t_token		*save;
	t_ASTree	*node;

	save = *token_list;
	node = arg_list1(token_list);
	if (node)
		return (node);
	*token_list = save;
	node = arg_list2(token_list);
	if (node)
		return (node);
	*token_list = save;
	return (NULL);
}

t_ASTree	*arg_list1(t_token **token_list)
{
	char		*arg;
	t_ASTree	*arglist_node;
	t_ASTree	*rdrlist_node;
	t_ASTree	*result;

	arg = NULL;
	if (!base_case(CHAR_GEN, token_list, &arg))
		return (NULL);
	rdrlist_node = redirlist(token_list);
	arglist_node = arg_list(token_list);
	result = (t_ASTree *)malloc(sizeof(*result));
	if (!result)
		return (delete_node(arglist_node), warn_message(),
			delete_node(rdrlist_node), free(arg), NULL);
	set_node_type(result, NODE_ARG);
	set_node_data(result, arg);
	attach_branch(result, arglist_node, rdrlist_node);
	return (result);
}

t_ASTree	*arg_list2(t_token **token_list)
{
	(void) token_list;
	return (NULL);
}
