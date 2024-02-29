/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 21:47:32 by mawad             #+#    #+#             */
/*   Updated: 2024/02/28 22:35:20 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

//This is to handle cases when the user tries to redirect to a
//directory instead of to a file. I wrote sample cases for this
//chicanery in my notes.
static void	dir_handle_err_msg(t_minishell *shell, char *expnd, t_ASTree *node, t_ASTree *rdrnode)
{
	if (!expnd || !rdrnode->data)
		return ;
	if (rdrnode->type == NODE_RDIN && node->left != NULL)
	{
		printf("minishell: stdin: Is a directory\n");
		shell->exit_status = 1;
	}
	else if (rdrnode->type == NODE_RDIN && node->left == NULL)
		shell->exit_status = 0;
	else if (rdrnode->type == NODE_RDOUT)
	{
		printf("minishell: %s: Is a directory\n", expnd);
		shell->exit_status = 1;
	}
	free(expnd);
}

static void	file_handle_err_msg(t_minishell *shell, char *expnd, t_ASTree *rdrnode)
{
	if (!expnd || !rdrnode->data)
		return ;
	if (rdrnode->data[0] == '\"')
		printf("-minishell: %s: No such file or directory\n",
			expnd);
	else if (rdrnode->data[0] == '$')
		printf("-minishell: %s: ambiguous redirect\n",
			rdrnode->data);
	else
		printf("-minishell: %s:  No such file or directory\n",
			expnd);
	shell->exit_status = 1;
	free(expnd);
}

static int	file_handling_helper(t_command *cmd, t_minishell *shell, char **expnd, t_ASTree **rdrnode)
{
	int	i;

	i = 0;
	while (*rdrnode)
	{
		if ((*rdrnode)->type != NODE_HDOC)
		{
			*expnd = ft_strdup((*rdrnode)->data);
			*expnd = expand_str(*expnd, shell->env, shell->exit_status);
			cmd->files[i].fname = (char *)malloc(ft_strlen(*expnd) + 1);
			ft_strlcpy(cmd->files[i].fname, *expnd, ft_strlen(*expnd) + 1);
			cmd->files[i].type = (*rdrnode)->type;
			cmd->files[i].fd = assign_fds(*rdrnode, cmd->files[i].fname);
			if (cmd->files[i].fd == -1 || cmd->files[i].fd == -2)
				break ;
			free(*expnd);
			i++;
		}
		*rdrnode = (*rdrnode)->left;
	}
	return (i);
}

int	file_handling(t_ASTree *node, t_minishell *shell, t_command *cmd)
{
	t_ASTree	*rdrnode;
	int			i;
	char		*expnd;

	rdrnode = node->right;
	i = 0;
	expnd = NULL;
	i = file_handling_helper(cmd, shell, &expnd, &rdrnode);
	if (rdrnode)
	{
		if (cmd->files[i].fd == -2)
			return (dir_handle_err_msg(shell, expnd, node, rdrnode), 0);
		else
			return (file_handle_err_msg(shell, expnd, rdrnode), 0);
	}
	return (1);
}
