/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moses <moses@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 05:17:02 by moses             #+#    #+#             */
/*   Updated: 2024/02/26 05:17:02 by moses            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	hdoc_handling(t_ASTree *rdrnode, t_command *cmd)
{
	int	i;
	int	len;

	i = 0;
	while (rdrnode)
	{
		len = 0;
		if (rdrnode->type == NODE_HDOC)
		{
			len = ft_strlen(rdrnode->data);
			cmd->delims[i] = (char *)malloc(len + 1);
			ft_strlcpy(cmd->delims[i], rdrnode->data, len + 1);
			i++;
		}
		rdrnode = rdrnode->left;
	}
	cmd->delims[i] = NULL;
}

int	redir_handling(t_ASTree *node, t_minishell *shell, t_command *cmd)
{
	init_delims_and_file_arr(node->right, cmd);

	// printf("\033[0;34m");
	// printf("This is the initialized fdata struct\n");
	// print_fdata(cmd);
	// printf("\033[0m");

	
	if (cmd->hdoc_amnt)
	{
		hdoc_handling(node->right, cmd);
		hdoc(shell, cmd);
	}
	if (cmd->f_amnt)
	{
		if (!file_handling(node, shell, cmd))
		{
			error_close_fds(cmd);
			//free everything and exit process
			//or return a flag and free
			return (0);
		}
	}
	//If we reach here, it means all went well, so at this point set
	//mode and assign the right file name to either rd_outf and the right
	//fd to fd_out. Choose between append and rdout, whichever comes last.
	//Same for hdoc and rdin. If hdoc comes after rdin, then I guess I can
	//still assign the fdin to char *fd_in but the mode defientely has to
	//be set to MODE_HDOC and later when we dup, we dont dup the infile. In
	//order to avoid freeing it, it's best to keep it as NULL. Idk, doesnt
	//mateer what you choose. The point is, we have to let hdoc write to the
	//STDOUT or to the pipe, not a redirected STDIN or the STDIN itself.
	files_setup(node->right, cmd);
	return (1);
}

void	print_fdata(t_command *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->f_amnt)
	{
		if (cmd->files[i].fname)
			printf("File name is: %s\n", cmd->files[i].fname);
		else
			printf("File is NULL\n");
		if (cmd->files[i].type == NODE_RDIN)
			printf("Type is: NODE_RDIN\n");
		else if (cmd->files[i].type == NODE_RDOUT)
			printf("Type is: NODE_RDOUT\n");
		else if (cmd->files[i].type == NODE_APND)
			printf("Type is: NODE_APND\n");
		else
			printf("Type is %d\n", cmd->files[i].type);
		printf("Fd is: %d\n", cmd->files[i].fd);
		printf("\n\n");
		i++;
	}
}

void	print_delims_arr(char **arr)
{
	int	i;

	i = 0;
	if (arr)
	{
		while (arr[i])
		{
			printf("Delim[%d]: %s\n", i, arr[i]);
			i++;
		}
	}
	else
		printf("Delims arr is empty\n");
	printf("\n\n");
}
