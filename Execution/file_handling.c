/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 21:47:32 by mawad             #+#    #+#             */
/*   Updated: 2024/03/04 12:32:52 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//This is to handle cases when the user tries to redirect to a
//directory instead of to a file. I wrote sample cases for this
//chicanery in my notes. The first if statement checks whether
//there are quotes in the dir name. If there aren't, then it
//would actually say ambigous redirect, not that the folder is
//a directory. The issue arises when we have a dir name with a
//space in it and environ var that expands to that dir name
//with the space in it too. So let's say we have a directory called
//'hey ho' and we do export HI='hey ho'. Now, if we say
//[echo "hello" >$HI] then it would say ambigous redirect
//because HI expanded into [hey ho] but since there is a space, it
//doesn't know what to do and says ambigous redirect. But if we
//do [echo "hello" >"$HI"], then HI expanded into []"hey ho"] and
//now, it actually finds our directory called 'hey ho' and says
//[bash: hey ho: Is a directory]. This issue only arises when
//we have an environ that expands to dir name with a space in it.
//In other instances, if I say [>hey ho] directly, then we wouldn't
//have that problem because only hey is considered as the filename to
//of the redirection by the tokenizer. Then ho would be interpreted as
//the command. If I say [echo "hello" >hey ho], then what happens is that
//echo's arguments would be hello and ho and they redirected output destination
//would be a file called hey. So basically, a file called hey would contain the
//string [hey ho].
static void	dir_handle_err_msg(t_minishell *shell, char *expnd,
									t_ASTree *node, t_ASTree *rdrnode)
{
	if (!expnd || !rdrnode->data)
		return ;
	if (rdrnode->type == NODE_RDIN && node->left != NULL)
	{
		ft_dprintf(2, "minishell: -: Is a directory\n");
		shell->exit_status = 1;
	}
	else if (rdrnode->type == NODE_RDIN && node->left == NULL)
		shell->exit_status = 0;
	else if (rdrnode->type == NODE_RDOUT || rdrnode->type == NODE_APND)
	{
		ft_dprintf(2, "minishell: %s: Is a directory\n", expnd);
		shell->exit_status = 1;
	}
	free(expnd);
}

static void	file_handle_err_msg(t_minishell *shell, char *expnd,
									t_ASTree *rdrnode)
{
	int	temp;

	(void) temp;
	if (!expnd || !rdrnode->data)
		return ;
	if (rdrnode->data[0] == '\"')
		ft_dprintf(2, "-minishell: %s: No such file or directory\n",
			expnd);
	else if (rdrnode->data[0] == '$')
		ft_dprintf(2, "-minishell: %s: ambiguous redirect\n",
			rdrnode->data);
	else
	{
		temp = open(expnd, O_RDWR, 0777);
		ft_dprintf(2, "-minishell: %s\n", strerror(errno));
	}
	shell->exit_status = 1;
	free(expnd);
}

static int	file_handling_helper(t_command *cmd, t_minishell *shell,
									char **expnd, t_ASTree **rdrnode)
{
	int	i;

	i = 0;
	while (*rdrnode)
	{
		if ((*rdrnode)->type != NODE_HDOC)
		{
			if (check_ambiguous(shell, (*rdrnode)->data))
				return (-1);
			*expnd = expand_str((*rdrnode)->data, shell->env,
					shell->exit_status);
			cmd->files[i].fname = (char *)malloc(ft_strlen(*expnd) + 1);
			ft_strlcpy(cmd->files[i].fname, *expnd, ft_strlen(*expnd) + 1);
			cmd->files[i].type = (*rdrnode)->type;
			cmd->files[i].fd = assign_fds(*rdrnode, cmd->files[i].fname);
			if (cmd->files[i].fd == -1 || cmd->files[i].fd == -2
				|| cmd->files[i].fd == -3)
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
	if (i == -1)
		return (free(expnd),
			ft_dprintf(2, "-minishell: %s: ambiguous redirect\n",
				rdrnode->data), 0);
	if (rdrnode)
	{
		if (cmd->files[i].fd == -2)
			return (dir_handle_err_msg(shell, expnd, node, rdrnode), 0);
		else
			return (file_handle_err_msg(shell, expnd, rdrnode), 0);
	}
	return (1);
}

// static void	file_handle_err_msg(t_minishell *shell, char *expnd,
// 									t_ASTree *rdrnode)
// {
// 	if (!expnd || !rdrnode->data)
// 		return ;
// 	if (access(expnd, F_OK) == 0)
// 	{
// 		printf("but did we is the question?\n");
// 		if (access(expnd, R_OK | W_OK) == -1)
// 			ft_dprintf(2, "-minishell: %s:  Operation not permitted\n",
// 				expnd);
// 	}
// 	else if (rdrnode->data[0] == '\"')
// 		ft_dprintf(2, "-minishell: %s: No such file or directory\n",
// 			expnd);
// 	else if (rdrnode->data[0] == '$')
// 		ft_dprintf(2, "-minishell: %s: ambiguous redirect\n",
// 			rdrnode->data);
// 	else
// 		ft_dprintf(2, "-minishell: %s:  No such file or directory\n",
// 			expnd);
// 	shell->exit_status = 1;
// 	free(expnd);
// }
