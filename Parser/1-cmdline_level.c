/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdline_level.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moses <moses@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 18:28:36 by moses             #+#    #+#             */
/*   Updated: 2024/02/08 18:35:04 by moses            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_ASTree	*cmdline(t_token **token_list)
{
	t_token		*save;
	t_ASTree	*node;

	save = *token_list;
	node = cmdline5(token_list);
	if (node)
		return (node);
	*token_list = save;
	return (NULL);
}

t_ASTree	*cmdline5(t_token **token_list)
{
	return (job(token_list));
}
