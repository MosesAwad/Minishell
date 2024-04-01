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
	if (cmd->hdoc_amnt)
	{
		hdoc_handling(node->right, cmd);
		hdoc(shell, cmd);
		if (g_sigstat == OUT_HDOC)
		{
			shell->exit_status = 130;
			g_sigstat = IN_SHELL;
			return (0);
		}
	}
	if (cmd->f_amnt)
	{
		if (!file_handling(node, shell, cmd))
		{
			error_close_fds(cmd);
			return (0);
		}
	}
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
