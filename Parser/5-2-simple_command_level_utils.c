/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   5-2-simple_command_level_utils.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moses <moses@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 04:20:32 by moses             #+#    #+#             */
/*   Updated: 2024/02/16 04:21:11 by moses            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_ASTree	*get_right_node(t_ASTree *node)
{
	t_ASTree	*right_copy;

	if (node->right)
	{
		right_copy = copy_node(node->right);
		delete_node(node->right);
		node->right = NULL;
		return (right_copy);
	}
	return (NULL);
}

t_ASTree	*push_rdlst_up(t_ASTree *node)
{
	t_ASTree	*store_right;
	t_ASTree	*temp;

	store_right = NULL;
	if (node->left)
		store_right = push_rdlst_up(node->left);
	if (node->left)
	{
		if (node->right)
		{
			temp = node->right;
			while (temp->left)
				temp = temp->left;
			temp->left = store_right;
		}
		else
			node->right = store_right;
	}
	store_right = get_right_node(node);
	return (store_right);
}

void	append_to_rdrlist(t_ASTree *rdrlistnode, t_ASTree *argnode)
{
	while (rdrlistnode->left)
		rdrlistnode = rdrlistnode->left;
	rdrlistnode->left = copy_node(argnode->right);
	delete_node(argnode->right);
	argnode->right = NULL;
}
