/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 19:00:59 by mawad             #+#    #+#             */
/*   Updated: 2024/03/04 18:15:49 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//This version of expand keeps the quotes!
char	*q_expand_str(char *str, char *env[], int exit_status)
{
	char	*q_expanded;

	q_expanded = expansion(str, env, exit_status);
	return (q_expanded);
}

//This version of expand strips the quotes!
char	*expand_str(char *str, char *env[], int exit_status)
{
	char	*stripped;

	str = expansion(str, env, exit_status);
	stripped = strip_quotes(str);
	free(str);
	return (stripped);
}
