/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 10:38:37 by mawad             #+#    #+#             */
/*   Updated: 2024/03/03 10:38:37 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//stand_alone $ signs are $ signs that should not
//be expanded.
//	-- stand_alone examples --
//Ex: echo $ -> $
//Ex: echo $ lol -> $ lol
//Ex: echo "$ hi" -> $ hi
//Ex: echo "haha $" -> haha
//
//	-- Non-Stand_alone examples --
//Ex: echo $"lmfao" -> lmfao
//Ex: echo $NONEXISTENT_ENVAR lmfao -> lmfao
static int	bool_stand_alone(char c, int state)
{
	if (is_wspace(c) || c == '\0' || (c == '"'
			&& state == IN_DQUOTE))
		return (printf("stand_alone\n"), 1);
	return (0);
}

int	not_stand_alone(char c1, char c2, int state)
{
	if (c1 == '$' && state != IN_SQUOTE
		&& !bool_stand_alone(c2, state))
		return (1);
	return (0);
}
