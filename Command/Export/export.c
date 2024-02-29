/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 20:15:54 by mawad             #+#    #+#             */
/*   Updated: 2024/02/23 20:51:13 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../minishell.h"

int	ft_export(t_minishell *shell, t_command *cmd)
{
	shell->exit_status = 0;
	if (cmd->cmd_args[1])
		export_w_flags(shell, cmd);
	else
		export_no_flags(shell);
	return (shell->exit_status);
}
