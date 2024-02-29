/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3-command_level.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moses <moses@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 01:50:10 by moses             #+#    #+#             */
/*   Updated: 2024/02/09 01:50:10 by moses            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_ASTree	*command(t_token **token_list)
{
	t_token		*save;
	t_ASTree	*node;

	save = *token_list;
	node = command1(token_list);
	if (node)
		return (node);
	*token_list = save;
	node = command2(token_list);
	if (node)
		return (node);
	*token_list = save;
	return (NULL);
}

t_ASTree	*command1(t_token **token_list)
{
	t_ASTree	*rdrlist1;
	t_ASTree	*smplcmdnode;
	t_ASTree	*rdrlist2;
	t_ASTree	*result;

	rdrlist1 = redirlist(token_list);
	smplcmdnode = smpl_command(token_list);
	if (smplcmdnode)
	{
		if (smplcmdnode->right)
			rdrlist2 = assign_rdrlist2(smplcmdnode);
		else
			rdrlist2 = NULL;
	}
	else
		return (delete_node(rdrlist1), NULL);
	result = case_checker1(rdrlist1, rdrlist2, smplcmdnode);
	if (result == smplcmdnode)
		result = case_checker2(rdrlist2, smplcmdnode);
	return (result);
}

t_ASTree	*command2(t_token **token_list)
{
	t_ASTree	*rdrlist1;
	t_ASTree	*result;

	rdrlist1 = redirlist(token_list);
	result = NULL;
	if (rdrlist1)
	{
		result = (t_ASTree *)malloc(sizeof(*result));
		if (!result)
			return (delete_node(rdrlist1), warn_message(), NULL);
		set_node_type(result, NODE_RDRGEN);
		set_node_data(result, NULL);
		attach_branch(result, NULL, rdrlist1);
	}
	return (result);
}
