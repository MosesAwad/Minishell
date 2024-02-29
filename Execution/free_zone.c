/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_zone.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 22:34:06 by mawad             #+#    #+#             */
/*   Updated: 2024/02/23 22:36:38 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	delete_2d_arr(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

static void	delete_fdata(t_command *cmd)
{
	int	i;

	i = 0;
	if (!cmd->files)
		return ;
	while (i < cmd->f_amnt)
		free(cmd->files[i++].fname);
	free(cmd->files);
}

void	delete_command(t_command *cmd)
{
	free(cmd->pids);
	delete_2d_arr(cmd->delims);
	delete_fdata(cmd);
	delete_2d_arr(cmd->cmd_args);
	free(cmd->cmd_path);
	free(cmd->cmd_full_path);
	if (cmd->rdin_f)
		close(cmd->fd_in);
	if (cmd->hdoc_amnt)
		close(cmd->fd_hdoc);
	if (cmd->rdout_f)
		close(cmd->fd_out);
	if (cmd->append_f)
		close(cmd->fd_apnd);
	if (cmd->save_stdout)
		close(cmd->save_stdout);
	if (cmd->save_stdin)
		close(cmd->save_stdin);
	free(cmd->rdin_f);
	free(cmd->rdout_f);
	free(cmd->append_f);
	free(cmd);
}

void	delete_pipe_command(t_command *cmd)
{
	delete_2d_arr(cmd->delims);
	delete_fdata(cmd);
	delete_2d_arr(cmd->cmd_args);
	free(cmd->cmd_path);
	free(cmd->cmd_full_path);
	if (cmd->rdin_f)
		close(cmd->fd_in);
	if (cmd->hdoc_amnt)
		close(cmd->fd_hdoc);
	if (cmd->rdout_f)
		close(cmd->fd_out);
	if (cmd->append_f)
		close(cmd->fd_apnd);
	close(cmd->save_stdout);
	close(cmd->save_stdin);
	free(cmd->rdin_f);
	free(cmd->rdout_f);
	free(cmd->append_f);
}
