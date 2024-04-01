/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hdoc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 18:47:13 by mawad             #+#    #+#             */
/*   Updated: 2024/03/04 16:25:19 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_loop_status(t_command *cmd, char *expansion, int i)
{
	if (g_sigstat == OUT_HDOC)
		return (e_break);
	if (!expansion)
	{
		ft_dprintf(1, "\n");
		return (e_break);
	}
	if (!expansion[0])
		return (e_continue);
	if (!ft_strncmp(expansion, cmd->delims[i], ft_strlen(expansion)))
		return (e_break);
	return (e_normal);
}

static void	hdoc_author(t_command *cmd, char *expansion, int i)
{
	if (!expansion)
		return ;
	if (!ft_strncmp(expansion, cmd->delims[i], ft_strlen(expansion)))
		return ;
	write(cmd->fd_hdoc, expansion, ft_strlen(expansion));
	write(cmd->fd_hdoc, "\n", 1);
}

static void	hdoc_loop(t_minishell *shell, t_command *cmd)
{
	char	*expansion;
	char	*line;
	int		i;

	i = 0;
	while (cmd->delims[i])
	{
		while (1)
		{
			if (g_sigstat == OUT_HDOC)
				return ;
			line = readline("heredoc> ");
			expansion = expand_str(line, shell->env, shell->exit_status);
			free(line);
			if (!cmd->delims[i + 1])
				hdoc_author(cmd, expansion, i);
			if (check_loop_status(cmd, expansion, i) == e_break)
				break ;
			else if (check_loop_status(cmd, expansion, i) == e_continue)
				continue ;
			free(expansion);
		}
		free(expansion);
		i++;
	}
}

void	hdoc(t_minishell *shell, t_command *cmd)
{
	g_sigstat = IN_HDOC;
	init_signals();
	cmd->fd_hdoc = open("hdoc_temp", O_WRONLY | O_CREAT | O_TRUNC, 0777);
	hdoc_loop(shell, cmd);
	close(cmd->fd_hdoc);
	cmd->fd_hdoc = open("hdoc_temp", O_RDONLY, 0777);
}
