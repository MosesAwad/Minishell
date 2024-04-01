/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_handling_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 10:37:20 by mawad             #+#    #+#             */
/*   Updated: 2024/03/03 10:37:20 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//we need to memset it because sometimes, if one of the files
//in the tree node is invalid, it would return -1. In that case,
//I break the loop in the file_handling (function) and so, the
//remaining members of the array won't be initialized. So then,
//how I can loop through cmd->files to close the fd's that have,
//been assigned already before the fd = -1 file? I can't tell it 
//where to stop unless I say while (i < cmd->f_amnt). But cmd->
//f_amnt has been initalized before the files have been opened,
//its the total number of files the user has entered in the cmdline
//regardless of whether they are valid or not. So if I loop using
//while (i < cmd->f_amnt), I would end up looping through unitiliazed
//values of the files_arr and perhaps closing some random garbage
//values assigned to the unitilaized values for cmd->files[i].fd in
//the members of the array present after the fd = -1 file. So to
//prevent undefined behavior, its best to just memset all the
//attributes of the array and just loop like this
//while (cmd->files[i].fname != NULL) and increment i.
void	memset_files_arr(t_command *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->f_amnt)
	{
		cmd->files[i].fname = NULL;
		cmd->files[i].type = 0;
		cmd->files[i].fd = 0;
		i++;
	}
}

void	init_delims_and_file_arr(t_ASTree *rdrnode, t_command *cmd)
{
	while (rdrnode)
	{
		if (rdrnode->type != NODE_HDOC)
			(cmd->f_amnt)++;
		else
			(cmd->hdoc_amnt)++;
		rdrnode = rdrnode->left;
	}
	if (cmd->f_amnt)
	{
		cmd->files = (t_fdata *)malloc(sizeof(t_fdata) * (cmd->f_amnt));
		memset_files_arr(cmd);
	}
	if (cmd->hdoc_amnt)
		cmd->delims = (char **)malloc(sizeof(char *) * (cmd->hdoc_amnt + 1));
}

int	assign_fds(t_ASTree *rdrnode, char *str)
{
	int	fd;
	DIR	*dir_res;

	fd = 0;
	dir_res = opendir(str);
	if (dir_res != NULL)
		return (closedir(dir_res), -2);
	if (rdrnode->type == NODE_RDIN)
		fd = open(str, O_RDONLY, 0777);
	if (rdrnode->type == NODE_RDOUT)
		fd = open(str, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (rdrnode->type == NODE_APND)
		fd = open(str, O_WRONLY | O_CREAT | O_APPEND, 0777);
	return (fd);
}

//I guess I could also say while (i < cmd->f_amnt && 
//cmd->files[i].fd != -1) to prevent closing it, but
//this still works lol. It's not necessarily bad to close
//an fd that got returned a -1 but valgrind will complain.
//Technically it's not a valid fd since it is simply the
//return of open upon encountering an error, otherwise if
//no error occurs, then the actual fd is returned. So, it
//makes sense for it to complain, though it really does no
//harm to close it. I just don't want any trouble with valgrind
//so I decided to not close it in that scenario since it doesn't
//need closing anyways.
void	error_close_fds(t_command *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->f_amnt && cmd->files[i].fname != NULL)
	{
		if (!(cmd->files[i].fd == -1 || cmd->files[i].fd == -2))
			close(cmd->files[i].fd);
		i++;
	}
}
