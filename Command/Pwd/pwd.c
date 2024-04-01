/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 10:49:54 by mawad             #+#    #+#             */
/*   Updated: 2024/03/03 10:49:54 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_pwd(t_minishell *shell)
{
	char	*current_dir;

	current_dir = getcwd(NULL, 0);
	if (!current_dir)
		return (shell->exit_status = 1, 1);
	ft_dprintf(1, "%s\n", current_dir);
	free(current_dir);
	return (shell->exit_status = 0, 0);
}
