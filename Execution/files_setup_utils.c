/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_setup.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 10:36:55 by mawad             #+#    #+#             */
/*   Updated: 2024/03/03 10:36:55 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_mode(t_ASTree *rdrnode, t_command *cmd)
{
	while (rdrnode)
	{
		if (rdrnode->type == NODE_RDIN)
			cmd->in_mode = MODE_RDIN;
		if (rdrnode->type == NODE_HDOC)
			cmd->in_mode = MODE_HDOC;
		if (rdrnode->type == NODE_RDOUT)
			cmd->out_mode = MODE_RDOUT;
		if (rdrnode->type == NODE_APND)
			cmd->out_mode = MODE_APND;
		rdrnode = rdrnode->left;
	}
}

void	copy_file_data(t_command *cmd, int ind, int mode)
{
	int	len;

	len = ft_strlen(cmd->files[ind].fname);
	if (mode == MODE_RDIN)
	{
		cmd->rdin_f = (char *)malloc(len + 1);
		ft_strlcpy(cmd->rdin_f, cmd->files[ind].fname,
			len + 1);
		cmd->fd_in = cmd->files[ind].fd;
	}
	if (mode == MODE_RDOUT)
	{
		cmd->rdout_f = (char *)malloc(len + 1);
		ft_strlcpy(cmd->rdout_f, cmd->files[ind].fname,
			len + 1);
		cmd->fd_out = cmd->files[ind].fd;
	}
	if (mode == MODE_APND)
	{
		cmd->append_f = (char *)malloc(len + 1);
		ft_strlcpy(cmd->append_f, cmd->files[ind].fname,
			len + 1);
		cmd->fd_apnd = cmd->files[ind].fd;
	}
}

void	set_infile(t_command *cmd)
{
	int	i;
	int	get_ind;

	i = 0;
	get_ind = -1;
	while (i < cmd->f_amnt)
	{
		if (cmd->files[i].type == NODE_RDIN)
			get_ind = i;
		i++;
	}
	if (get_ind == -1)
		return ;
	i = 0;
	while (i < get_ind)
	{
		if (cmd->files[i].type == NODE_RDIN)
			close(cmd->files[i].fd);
		i++;
	}
	copy_file_data(cmd, get_ind, MODE_RDIN);
}

void	set_outfile(t_command *cmd)
{
	int	i;
	int	get_ind;

	i = 0;
	get_ind = -1;
	while (i < cmd->f_amnt)
	{
		if (cmd->files[i].type == NODE_RDOUT)
			get_ind = i;
		i++;
	}
	if (get_ind == -1)
		return ;
	i = 0;
	while (i < get_ind)
	{
		if (cmd->files[i].type == NODE_RDOUT
			|| cmd->files[i].type == NODE_APND)
			close(cmd->files[i].fd);
		i++;
	}
	copy_file_data(cmd, get_ind, MODE_RDOUT);
}

void	set_appendfile(t_command *cmd)
{
	int	i;
	int	get_ind;

	i = 0;
	get_ind = -1;
	while (i < cmd->f_amnt)
	{
		if (cmd->files[i].type == NODE_APND)
			get_ind = i;
		i++;
	}
	if (get_ind == -1)
		return ;
	i = 0;
	while (i < get_ind)
	{
		if (cmd->files[i].type == NODE_RDOUT
			|| cmd->files[i].type == NODE_APND)
			close(cmd->files[i].fd);
		i++;
	}
	copy_file_data(cmd, get_ind, MODE_APND);
}
