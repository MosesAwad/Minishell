/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   4-redir_list_level.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moses <moses@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 01:50:20 by moses             #+#    #+#             */
/*   Updated: 2024/02/09 01:50:20 by moses            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_ASTree	*redirlist(t_token **token_list)
{
	t_token		*save;
	t_ASTree	*node;

	save = *token_list;
	node = redirlist1(token_list);
	if (node)
		return (node);
	*token_list = save;
	node = redirlist2(token_list);
	if (node)
		return (node);
	*token_list = save;
	node = redirlist3(token_list);
	if (node)
		return (node);
	*token_list = save;
	node = redirlist4(token_list);
	if (node)
		return (node);
	*token_list = save;
	return (NULL);
}


t_ASTree	*redirlist1(t_token	**token_list)
{
	char		*filename;
	t_ASTree	*rdrlst_node;
	t_ASTree	*result;

	filename = NULL;
	if (!base_case(CHAR_RDIN, token_list, NULL))
		return (NULL);
	if (!base_case(CHAR_GEN, token_list, &filename))
		return (NULL);
	rdrlst_node = redirlist(token_list);
	result = (t_ASTree *)malloc(sizeof(*result));
	if (!result)
		return (delete_node(rdrlst_node), free(filename),
			warn_message(), NULL);
	set_node_type(result, NODE_RDIN);
	set_node_data(result, filename);
	attach_branch(result, rdrlst_node, NULL);
	return (result);
}

t_ASTree	*redirlist2(t_token	**token_list)
{
	char		*filename;
	t_ASTree	*rdrlst_node;
	t_ASTree	*result;

	filename = NULL;
	if (!base_case(CHAR_RDOUT, token_list, NULL))
		return (NULL);
	if (!base_case(CHAR_GEN, token_list, &filename))
		return (NULL);
	rdrlst_node = redirlist(token_list);
	result = (t_ASTree *)malloc(sizeof(*result));
	if (!result)
		return (delete_node(rdrlst_node), free(filename),
			warn_message(), NULL);
	set_node_type(result, NODE_RDOUT);
	set_node_data(result, filename);
	attach_branch(result, rdrlst_node, NULL);
	return (result);
}

t_ASTree	*redirlist3(t_token	**token_list)
{
	char		*filename;
	t_ASTree	*rdrlst_node;
	t_ASTree	*result;

	filename = NULL;
	if (!base_case(CHAR_HDOC, token_list, NULL))
		return (NULL);
	if (!base_case(CHAR_GEN, token_list, &filename))
		return (NULL);
	rdrlst_node = redirlist(token_list);
	result = (t_ASTree *)malloc(sizeof(*result));
	if (!result)
		return (delete_node(rdrlst_node), free(filename),
			warn_message(), NULL);
	set_node_type(result, NODE_HDOC);
	set_node_data(result, filename);
	attach_branch(result, rdrlst_node, NULL);
	return (result);
}

t_ASTree	*redirlist4(t_token	**token_list)
{
	char		*filename;
	t_ASTree	*rdrlst_node;
	t_ASTree	*result;

	filename = NULL;
	if (!base_case(CHAR_APND, token_list, NULL))
		return (NULL);
	if (!base_case(CHAR_GEN, token_list, &filename))
		return (NULL);
	rdrlst_node = redirlist(token_list);
	result = (t_ASTree *)malloc(sizeof(*result));
	if (!result)
		return (delete_node(rdrlst_node), free(filename),
			warn_message(), NULL);
	set_node_type(result, NODE_APND);
	set_node_data(result, filename);
	attach_branch(result, rdrlst_node, NULL);
	return (result);
}
