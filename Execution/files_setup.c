/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_setup_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 16:01:02 by mawad             #+#    #+#             */
/*   Updated: 2024/03/04 16:52:58 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	close_infiles(t_command *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->f_amnt)
	{
		if (cmd->files[i].type == NODE_RDIN)
			close(cmd->files[i].fd);
		i++;
	}
}

void	files_setup(t_ASTree *rdrnode, t_command *cmd)
{
	set_mode(rdrnode, cmd);
	if (cmd->in_mode == MODE_RDIN)
		set_infile(cmd);
	else if (cmd->in_mode == MODE_HDOC)
		close_infiles(cmd);
	if (cmd->out_mode == MODE_RDOUT)
		set_outfile(cmd);
	else if (cmd->out_mode == MODE_APND)
		set_appendfile(cmd);
}
