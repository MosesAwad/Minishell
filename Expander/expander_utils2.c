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
//Ex: echo $ -> $ ($ followed by a \0 in the token)
//Ex: echo $ lol -> $ lol ($ followed by a \0 in the token)
//Ex: echo "$ hi" -> $ hi ($ followed by a whitespace)
//Ex: echo "haha $" -> haha ($ followed by a " within the double
//quote state)
//In addition to that, any $ sign within single quotes should
//never be expanded no matter what.
//Any other $ sign scenario, I consider as "non-stand_alone" and
//here are just two basic examples.
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
