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

static void	exec_built_ins(t_minishell *shell, t_command *cmd)
{
	built_in_dup(cmd);
	call_built_ins(shell, cmd);
	dup2(cmd->save_stdout, STDOUT_FILENO);
	dup2(cmd->save_stdin, STDIN_FILENO);
}

//If you just say in the commandline [Dirname/subdirname] then 
//it prints the error message shown below and sets the exit status
//to 126. But if you just say in the commandline [Dirname], then it
//does't print an error message or do anything. It just jumps to the
//next line giving you the prompt again and it even sets the exit
//status to 0.
void	check_dir_message(t_minishell *shell, char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '/')
		i++;
	if (str[i])
	{
		printf("minishell: %s: is a directory\n", str);
		shell->exit_status = 126;
	}
	else
		shell->exit_status = 0;
}

//We need to check if what has been inserted into the commandline
//where you would place the command, aka cmdpath, is the name of
//a directory. If it is, then it shouldn't say command not found!
//It should say blah blah is a directory. So let's give an example.
//I say in the commandline [dirname "hello world"]. In that case,
//it wouldn't say bash: dirname: command not found. It would instead
//say bash: dirname: is a directory. So we need to hadnle that issue.
int	check_directory(t_minishell *shell, t_command *cmd)
{
	DIR		*dir_res;
	int		result;

	result = 0;
	dir_res = NULL;
	if (!(cmd->cmd_path))
		return (0);
	dir_res = opendir(cmd->cmd_path);
	if (dir_res && !check_in_binlist(shell, cmd->cmd_path))
	{
		check_dir_message(shell, cmd->cmd_path);
		closedir(dir_res);
		result = 1;
	}
	else if (dir_res && check_in_binlist(shell, cmd->cmd_path))
		closedir(dir_res);
	return (result);
}

//This function is to check if the user has already inserted
//a path into the commandline. In that case, no need to go to
//check access. So if a user says /bin/ls, then that just run it
//straight to execve. If the command exists (and it is not a directory),
//then execve would ismply execute it, if it does not exist (and it is
//not a directory), then execve would simply return -1. But if the user
//just says something like ls, then the command might actually exist so
//lets prepend a path form the path_list in the check_access function till
//we find a match. If we find a match, then we return the full path and run
//it through execve (full path means prepending the path to the command given
//by the user likes this: the path/command_name). If we do not find a match,
//then that command does not exist and the my check_access function simply
//returns NULL which gets passed to execve and returns a -1.
int	bool_path_given(char *str)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (str[i] && str[i] != '/')
		i++;
	if (str[i])
		flag = 1;
	return (flag);
}

//	if (cmd->pids[(cmd->pid_i)++] != 0)
//		return ;
//That line up above is just to exit if you are the parent process.
//The second if (cmd->stdin_pipe || cmd->stdout_pipe) is needed
//for the last job. It's always best to explain with an example.
//Imagine the cmdline is <infile cat | wc -l >outfile. Job1 would
//use fd = 3 for piperead, fd = 4 for pipewrite, and fd = 5 for
//infile. Then we close pipewrite and infile. But piperead is still
//open because the last job, which is job2 in this case needs to
//read from it. Thus, fd = 3 is still being used. Hence, we get
//fd = 4 for outfile. But now, since I am using this exec_smpl_cmd
//function as a multipurpose function, I always end up closing
//piperead and pipewrite. It is just easier. Now pipewrite has 
//been closed in the parent, that is right. So it was inherited
//closed. But, the thing is, pipewrite's value is still 4! So, I
//end up closing fd = 4, if I didn't have the second 
//if (cmd->stdin_pipe || cmd->stdout_pipe) and just closed all the
//pipe fd's in the first if statement. If I close that fd = 4, 
//then when I get to if (cmd->rdout_f) and attempt to 
//dup2(cmd->fd_out), which also has a value of 4, it wouldn't work;
//because that fd value has been closed already. In fact, dup2 would
//return -1. You can check it if you'd like by saying 
//if (dup2(cmd->fd_out, STDOUT_FILENO) == -1) then 
//printf("dup error\n") and it would dup error.
void	exec_smpl_command(t_minishell *shell, t_command *cmd)
{
	// printf("------------------------\n");
	// printf("   THIS IS THE OUTPUT\n");
	// printf("------------------------\n");

	if (check_directory(shell, cmd))
		return ;
	cmd->save_stdin = dup(STDIN_FILENO);
	cmd->save_stdout = dup(STDOUT_FILENO);
	if (check_built_ins(cmd))
	{
		// printf("we went built in\n");
		exec_built_ins(shell, cmd);
		return ;
	}
	cmd->pids[cmd->pid_i] = fork();
	if (cmd->pids[cmd->pid_i] == -1)
	{
		perror("fork");
		exit(1) ;
	}
	if (cmd->pids[(cmd->pid_i)++] != 0)
		return ;
	regular_dup(cmd);
	if (bool_path_given(cmd->cmd_path))
		cmd->cmd_full_path = ft_strdup(cmd->cmd_path);
	else
		cmd->cmd_full_path = check_access(shell, cmd);
	if (execve(cmd->cmd_full_path, cmd->cmd_args, shell->env) == -1)
		handle_execve_error(shell, cmd);
}

//This function is used when we want to execute a command that
//is not part of a pipeline. 
void	execute_command(t_minishell *shell, t_command *command, int proc_count)
{
	t_ASTree *tree;

	tree = shell->ast;
	if (tree->type == NODE_RDRGEN)
	{
		//node redirection tree->right (this will alter command node
		//that goes below to execsimplecommand)
		if (!redir_handling(tree, shell, command))
		{
			//DONT DELETE JUST YET, WE STILL HAVE TO GO TO HDOC IF PRESENT
			delete_command(command);
			return ;
		}

		// printf("This is the fdata struct post-build:\n");
		// print_fdata(command);
		// printf("This is the delims array:\n");
		// print_delims_arr(command->delims);
		// printf("\033[0m");

		// if (command->rdin_f)
		// 	printf("infile is %s and its fd is %d\n", command->rdin_f, command->fd_in);
		// else
		// 	printf("infile is null and its fd is %d\n", command->fd_in);

		// if (command->rdout_f)
		// 	printf("outfile is %s and its fd is %d\n", command->rdout_f, command->fd_out);
		// else
		// 	printf("outfile is null and its fd is %d\n", command->fd_out);

		// if (command->append_f)
		// 	printf("appendfile is %s and its fd is %d\n", command->append_f, command->fd_apnd);
		// else
		// 	printf("appendfile is null and its fd is %d\n", command->fd_apnd);

		if (tree->left)
		{
			set_up_command(tree->left, shell, command);
			exec_smpl_command(shell, command);
		}
	}
	else if (tree->type == NODE_CMDPATH)
	{
		set_up_command(tree, shell, command);
		exec_smpl_command(shell, command);
	}
	waiter(shell, command, proc_count);

	// printf("------------------------\n");
	// printf("   END OF OUTPUT\n");
	// printf("------------------------\n");

	printf("(From parent) This is the command: %s\n", command->cmd_path);
	printf("(From parent) These are the command arguments:\n");
	if (command->cmd_args)
		print_cmd_args(command);

	//THIS DELETE HERE IS JUST TO CHECK WHETHER WE ARE LEAKING DURING TESTING
	//BUT LATER, WE MIGHT HAVE TO REPOSITION IT
	delete_command(command);
}
