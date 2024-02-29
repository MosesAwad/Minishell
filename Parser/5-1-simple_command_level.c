/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   5-simple_command_level.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moses <moses@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 01:50:24 by moses             #+#    #+#             */
/*   Updated: 2024/02/09 01:50:24 by moses            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_ASTree	*smpl_command(t_token **token_list)
{
	t_token		*save;
	t_ASTree	*node;

	save = *token_list;
	node = smpl_command1(token_list);
	if (node)
		return (node);
	*token_list = save;
	return (NULL);
}

static t_ASTree	*get_arg_node(t_token **token_list, t_ASTree **rdrlistnode)
{
	t_ASTree	*argnode;
	t_ASTree	*argrightnode;

	argnode = arg_list(token_list);
	if (argnode)
	{
		argrightnode = push_rdlst_up(argnode);
		argnode->right = argrightnode;
		if (argnode->right)
		{
			if (*rdrlistnode)
				append_to_rdrlist(*rdrlistnode, argnode);
			else
			{
				*rdrlistnode = copy_node(argnode->right);
				delete_node(argnode->right);
				argnode->right = NULL;
			}
		}
	}
	return (argnode);
}

t_ASTree	*smpl_command1(t_token	**token_list)
{
	char		*cmdpathname;
	t_ASTree	*cmdnode;
	t_ASTree	*rdrlistnode;
	t_ASTree	*argnode;

	cmdpathname = NULL;
	if (base_case(CHAR_GEN, token_list, &cmdpathname))
	{
		cmdnode = (t_ASTree *)malloc(sizeof(*cmdnode));
		if (!cmdnode)
			return (free(cmdpathname), warn_message(), NULL);
		set_node_type(cmdnode, NODE_CMDPATH);
		set_node_data(cmdnode, cmdpathname);
	}
	else
		return (NULL);
	rdrlistnode = redirlist(token_list);
	argnode = get_arg_node(token_list, &rdrlistnode);
	attach_branch(cmdnode, argnode, rdrlistnode);
	return (cmdnode);
}
