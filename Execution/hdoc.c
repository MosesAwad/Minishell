/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hdoc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 18:47:13 by mawad             #+#    #+#             */
/*   Updated: 2024/02/23 22:37:15 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

//get_next_line comes with a \n at the end while
//the delim is stored with a \0 at the end. Thus,
//we have to compare up to ft_strlen - 1.
void	hdoc(t_minishell *shell, t_command *cmd)
{
	int		i;
	char	*line;

	i = 0;
	cmd->fd_hdoc = open("hdoc_temp", O_WRONLY | O_CREAT | O_TRUNC, 0777);
	// printf("this is the hdoc file fd: %d\n", cmd->fd_hdoc);
	while (cmd->delims[i + 1])
	{
		while (1)
		{
			ft_putstr_fd("> ", STDOUT_FILENO);
			line = expand_str(get_next_line(0), shell->env, shell->exit_status);
			if (!line)
				break;
			if (!ft_strncmp(line, cmd->delims[i], ft_strlen(line) - 1))
				break ;
			free(line);
		}
		free(line);
		i++;
	}
	while (1)
	{
		ft_putstr_fd("> ", STDOUT_FILENO);
		line = expand_str(get_next_line(0), shell->env, shell->exit_status);
		// printf("line is %s\n", line);
		if (!line)
			break;
		if (!ft_strncmp(line, cmd->delims[i], ft_strlen(line) - 1))
			break ;
		write(cmd->fd_hdoc, line, ft_strlen(line));
		free(line);
	}
	close(cmd->fd_hdoc);
	cmd->fd_hdoc = open("hdoc_temp", O_RDONLY, 0777);
	free(line);
}
