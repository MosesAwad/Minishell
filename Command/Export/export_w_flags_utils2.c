/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_w_flags_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 11:12:11 by mawad             #+#    #+#             */
/*   Updated: 2024/03/03 11:12:50 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	bool_syntax_validity(char c)
{
	if (ft_isalpha(c))
		return (1);
	if (c == '_')
		return (1);
	if (ft_isdigit(c))
		return (1);
	return (0);
}

//applies the bool_syntax_validity function
//to the export arguments
int	check_export_syntax(char *str)
{
	int	i;

	i = 0;
	if (!(ft_isalpha(str[i]) || str[i] == '_'))
	{
		ft_dprintf(2, "-minishell: export: '%s': not a valid identifier\n",
			str);
		return (0);
	}
	i++;
	while (str[i] != '=' && str[i])
	{
		if (!bool_syntax_validity(str[i]))
		{
			ft_dprintf(2, "-minishell: export: '%s': not a valid identifier\n",
				str);
			return (0);
		}
		i++;
	}
	return (1);
}
