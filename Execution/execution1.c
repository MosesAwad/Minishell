/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 02:31:37 by mawad             #+#    #+#             */
/*   Updated: 2024/02/22 02:31:37 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	exec_node(t_ASTree *node, t_minishell *shell, t_command *cmd)
{
	if (node->type == NODE_RDRGEN)
	{
		if (redir_handling(node, shell, cmd))
		{
			
			// printf("This is the fdata struct post-build:\n");
			// print_fdata(cmd);
			// printf("This is the delims array:\n");
			// print_delims_arr(cmd->delims);
			// printf("\033[0m");

			if (node->left)
			{
				set_up_command(node->left, shell, cmd);

				// printf("\033[1;35m");	
				// printf("This is the command and its arguments\n");
				// if (cmd->cmd_args)
				// 	print_cmd_args(cmd);
				// printf("\033[0m");
				// printf("bruh\n");

				exec_smpl_command(shell, cmd);
			}
		}
	}
	else if (node->type == NODE_CMDPATH)
	{
		set_up_command(node, shell, cmd);

		// printf("This is the command and its arguments\n");
		// if (cmd->cmd_args)
		// 	print_cmd_args(cmd);

		exec_smpl_command(shell, cmd);
	}
}

static void	do_pipe(int *fd)
{
	if (pipe(fd) == -1)
	{
		perror("pipe");
		exit(2);
	}
}

static void	pipe_chain_exec(t_ASTree **tree, t_command *cmd, t_minishell *shell, int *fd)
{
	while ((*tree)->right->type == NODE_PIPE)
	{
		*tree = (*tree)->right;
		init_pipe_command(cmd);
		close(cmd->pipewrite);
		cmd->stdin_pipe = 1;
		cmd->stdout_pipe = 1;
		do_pipe(fd);
		cmd->copy_rd_fd = fd[0];
		cmd->pipewrite = fd[1];
		exec_node((*tree)->left, shell, cmd);
		close(cmd->piperead);
		cmd->piperead = fd[0];
		delete_pipe_command(cmd);
	}
}

////For arguments sake, I will say I have 2 pipes so 3 jobs. I had to copy the fd[0]
//from the second job (or basically the second call to pipe()) into copy_rd_fd when
//we have multiple pipes. Here is why:
//I don't close fd[0], aka piperead from the first job, until the second job is
//done, because the second job needs to read from it. But what happens is
//since I did not close the fd[0] from the first job but I pipe on int fd[2] again,
//this creates a new fd[0] which the child still inherits. Thus, I need to close 
//that one too in the child process even though I don't have to use it for this job,
//I only have to use it for the third job. Nevertheless, I still have to close it
//to avoid fd leaks so what I do is I copy it into cmd->copy_rd_fd and close it 
//anyways in the child process.
static void	execute_pipeline(t_minishell *shell, t_command *cmd, int proc_count)
{
	int			fd[2];
	t_ASTree	*tree;

	do_pipe(fd);
	tree = shell->ast;
	cmd = (t_command *)malloc(sizeof(t_command));
	if (!init_command(cmd, proc_count))
		return ;
	cmd->piperead = fd[0];
	cmd->pipewrite = fd[1];
	cmd->copy_rd_fd = fd[0];
	cmd->stdin_pipe = 0;
	cmd->stdout_pipe = 1;
	exec_node(tree->left, shell, cmd);
	delete_pipe_command(cmd);
	pipe_chain_exec(&tree, cmd, shell, fd);
	init_pipe_command(cmd);
	close(cmd->pipewrite);
	cmd->stdin_pipe = 1;
	cmd->stdout_pipe = 0;
	exec_node(tree->right, shell, cmd);
	close(cmd->piperead);
	waiter(shell, cmd, proc_count);
	delete_command(cmd);
}

void	execute_job(t_minishell *shell)
{
	t_ASTree	*tree;
	t_command	*command;
	int			proc_count;

	tree = shell->ast;
	command = NULL;
	if (!tree)
		return ;
	// if (check_directory(shell, tree))
	// 	return ;
	if (tree->type == NODE_PIPE)
	{
		proc_count = count_procs(shell->ast, shell);
		// printf("THIS IS THE PROC_COUNT: %d\n", proc_count);
		execute_pipeline(shell, command, proc_count);
	}
	else if (tree->type == NODE_RDRGEN || tree->type == NODE_CMDPATH)
	{
		proc_count = count_procs(shell->ast, shell);
		// printf("THIS IS THE PROC_COUNT: %d\n", proc_count);
		command = (t_command *)malloc(sizeof(t_command));
		if (!init_command(command, proc_count))
			return ;
		command->stdin_pipe = 0;
		command->stdout_pipe = 0;
		command->pipewrite = 0;
		command->piperead = 0;
		execute_command(shell, command, proc_count);
	}
	remove("hdoc_temp");
}
