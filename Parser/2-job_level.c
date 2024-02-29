/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2-job_level.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 18:30:12 by moses             #+#    #+#             */
/*   Updated: 2024/02/29 18:14:18 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_ASTree	*job(t_token **token_list)
{
	t_token		*save;
	t_ASTree	*node;

	save = *token_list;
	node = job1(token_list);
	if (node)
		return (node);
	*token_list = save;
	node = job2(token_list);
	if (node)
		return (node);
	*token_list = save;
	return (NULL);
}

t_ASTree	*job1(t_token **token_list)
{
	t_ASTree	*cmd_node;
	t_ASTree	*job_node;
	t_ASTree	*result;

	cmd_node = command(token_list);
	if (!cmd_node)
		return (NULL);
	if (!base_case(CHAR_PIPE, token_list, NULL))
		return (delete_node(cmd_node), NULL);
	job_node = job(token_list);
	if (!job_node)
		return (delete_node(cmd_node), NULL);
	result = (t_ASTree *)malloc(sizeof(*result));
	if (!result)
		return (delete_node(cmd_node), warn_message(), NULL);
	set_node_type(result, NODE_PIPE);
	set_node_data(result, NULL);
	attach_branch(result, cmd_node, job_node);
	return (result);
}

t_ASTree	*job2(t_token **token_list)
{
	return (command(token_list));
}
