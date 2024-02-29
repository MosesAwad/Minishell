/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_procs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 22:08:08 by mawad             #+#    #+#             */
/*   Updated: 2024/02/29 18:11:02 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//We need to check if what has been inserted into the commandline
//where you would place the command, aka cmdpath, is the name of
//a directory. If it is, then it shouldn't say command not found!
//It should say blah blah is a directory. So let's give an example.
//I say in the commandline [dirname "hello world"]. In that case,
//it wouldn't say bash: dirname: command not found. It would instead
//say bash: dirname: is a directory. So we need to hadnle that issue.
static int	comp_dir(t_minishell *shell, t_ASTree *node)
{
	DIR		*dir_res;
	int		result;
	char	*store;

	result = 0;
	dir_res = NULL;
	store = NULL;
	if (node->type == NODE_RDRGEN)
	{
		store = node->left->data;
		dir_res = opendir(store);
	}
	else if (node->type == NODE_CMDPATH)
	{
		store = node->data;
		dir_res = opendir(store);
	}
	if (dir_res)
	{
		closedir(dir_res);
		if (!check_in_binlist(shell, store))
			result = 1;
	}
	return (result);
}

static int	comp_built_in(char *str, t_minishell *shell)
{
	char	*expansion;
	int		len;

	expansion = ft_strdup(str);
	expansion = expand_str(expansion, shell->env, shell->exit_status);
	len = ft_strlen(expansion);
	if (!ft_strncmp(expansion, "echo", len))
		return (free(expansion), 1);
	if (!ft_strncmp(expansion, "cd", len))
		return (free(expansion), 1);
	if (!ft_strncmp(expansion, "pwd", len))
		return (free(expansion), 1);
	if (!ft_strncmp(expansion, "export", len))
		return (free(expansion), 1);
	if (!ft_strncmp(expansion, "unset", len))
		return (free(expansion), 1);
	if (!ft_strncmp(expansion, "env", len))
		return (free(expansion), 1);	
	if (!ft_strncmp(str, "exit", len))
		return (free(expansion), 1);	
	return (free(expansion), 0);
}

static t_ASTree	*inter_cmds(t_ASTree *node, t_minishell *shell, int *proc_count)
{
	while (node->type == NODE_PIPE)
	{
		if (node->left->type == NODE_RDRGEN)
		{
			if (node->left->left && node->left->left->type == NODE_CMDPATH)
			{
				if (!comp_built_in(node->left->left->data, shell))
					(*proc_count)++;
			}
		}
		else if (node->left->type == NODE_CMDPATH)
		{
			if (!comp_built_in(node->left->data, shell))
				(*proc_count)++;
		}
		node = node->right;
	}
	return (node);
}

static void	last_cmd(t_ASTree *node, t_minishell *shell, int *proc_count)
{
	if (node->type == NODE_RDRGEN)
	{
		if (node->left && node->left->type == NODE_CMDPATH)
		{
			if (!comp_built_in(node->left->data, shell))
				(*proc_count)++;
		}
	}
	else if (node->type == NODE_CMDPATH)
	{
		if (!comp_built_in(node->data, shell))
			(*proc_count)++;
	}
}
//I need to count how many processes there are in total. But,
//commands that are not built in, they do should not get forked.
//Thus, I shouldn't wait for them and I shouldn't assign them
//a pid. I want to wait only for the processes that get forked.
//Previously, I was just counting how many pipes I had and then
//return pipe_count + 1 for the total number of processes. But
//now, since some functions can be built-in and they shouldn't
//be counted in proc_counts, I have to go through the tree and
//check which commands are built in by using the comp_built_in
//function to compare whether they are built_in or not. First,
//if the node does not have a pipe, I simply check if it is
//built in, I return 0 and if not, I return 1. If We do have
//a pipe, then the if-statement was never entered and thus, we
//go to inter_procs which checks all the intermediate commands
//to see whether they are built in via the inter_cmds function.
//Then I check the last command which is after the last pipe and
//check whether that one is a built-in or not via the last_cmd 
//function.
//Similarly, if the command is just a directory name, then I do
//not want to wait I either. Hence, I don't want to assign it
//a spot in cmd->pids array.
//The else if (!node->left) is to handle cases where
//we have a commandline like [>outfile], in that case, I also
//do not want to consider that as a process because I dont want
//to wait for it. The only time I want to wait is if I have
//a command in my commandline that is not a built=in.
int	count_procs(t_ASTree *node, t_minishell *shell)
{
	int			proc_count;
	t_ASTree	*pickup_node;

	proc_count = 0;
	if (node->type != NODE_PIPE)
	{
		if (node->type == NODE_RDRGEN)
		{
			if (node->left && node->left->type == NODE_CMDPATH)
			{
				if (comp_built_in(node->left->data, shell)
					|| comp_dir(shell, node))
					return (0);
			}
			else if (!node->left)
				return (0);
		}
		else if (node->type == NODE_CMDPATH)
		{
			if (comp_built_in(node->data, shell) || comp_dir(shell, node))
				return (0);
		}
		return (1);
	}
	pickup_node = inter_cmds(node, shell, &proc_count);
	last_cmd(pickup_node, shell, &proc_count);
	return (proc_count);
}
