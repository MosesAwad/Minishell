/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_error_handling2.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 11:34:47 by mawad             #+#    #+#             */
/*   Updated: 2024/03/04 12:39:56 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_execve_error(t_minishell *shell, t_command *cmd)
{
	int	exit_code;

	if (errno == 13)
	{
		ft_dprintf(2, "-minishell: %s: %s\n", cmd->cmd_path, strerror(errno));
		exit_code = 126;
	}
	else
	{
		ft_dprintf(2, "-minishell: %s: command not found\n", cmd->cmd_path);
		exit_code = 127;
	}
	dup2(cmd->save_stdout, STDOUT_FILENO);
	dup2(cmd->save_stdin, STDIN_FILENO);
	close(cmd->save_stdout);
	close(cmd->save_stdin);
	delete_command(cmd);
	free(shell->cmd_line);
	delete_env_list(shell);
	delete_list(shell->tok_list);
	delete_binlist(shell->binlist);
	delete_node(shell->ast);
	exit(exit_code);
}

//If you give a dirname (without a slash or a path) in place of a command
//(even with args), bash says "dirname: command not found". If you give a 
//dirname (with a slash or a path) in place of a command (even with args),
//bash says "Dirname is a directory".
static void	check_dir_message(t_minishell *shell, t_command *cmd, char *str)
{
	if (!ft_strchr(cmd->cmd_path, '/'))
	{
		ft_dprintf(2, "-minishell: %s: command not found\n", cmd->cmd_path);
		shell->exit_status = 127;
		return ;
	}
	else
	{
		ft_dprintf(2, "-minishell: %s: is a directory\n", str);
		shell->exit_status = 126;
	}
}

//We need to check if what has been inserted into the commandline
//where you would place the command, aka cmdpath, is the syntax of
//a directory. If it is, and even if the folder does not exist
// then it shouldn't say command not found! It should say blah blah
//no such file or directory. But sometimes,
//if we have dir_syntax, it could be the path to an executable like
//for example, /bin/ls; so in that case, I want to use access to see
//whether it exists or not, if it does, don't return 0. But if we
//have something like /bin/happydays; there is no executable with
//that name so It should say no such file or directory for hapyydays.
static int	check_dir_syntax(char *str)
{
	if (str[0] != '.' && ft_strchr(str, '/'))
	{
		if (access(str, F_OK) == -1)
			return (1);
	}
	return (0);
}

static int	dir_res_scenarios(t_minishell *shell, t_command *cmd, DIR *dir_res)
{
	if (dir_res && !check_in_binlist(shell, cmd->cmd_path))
	{
		check_dir_message(shell, cmd, cmd->cmd_path);
		closedir(dir_res);
		return (1);
	}
	else if (dir_res && check_in_binlist(shell, cmd->cmd_path))
	{
		closedir(dir_res);
		return (0);
	}
	else if (!dir_res && check_dir_syntax(cmd->cmd_path))
	{
		ft_dprintf(2, "-minishell: %s: No such file or directory\n",
			cmd->cmd_path);
		shell->exit_status = 127;
		return (1);
	}
	else
		return (0);
}

//We need to check if what has been inserted into the commandline
//where you would place the command, aka cmdpath, is the name of
//a directory. If it is, then it shouldn't say command not found!
//It should say blah blah is a directory. So let's give an example.
//I say in the commandline [dirname "hello world"]. In that case,
//it wouldn't say bash: dirname: command not found. It would instead
//say bash: dirname: is a directory. So we need to handle that issue.
int	check_directory(t_minishell *shell, t_command *cmd)
{
	DIR		*dir_res;
	int		result;

	result = 0;
	dir_res = NULL;
	if (!(cmd->cmd_path))
		return (0);
	dir_res = opendir(cmd->cmd_path);
	result = dir_res_scenarios(shell, cmd, dir_res);
	if (result == 1)
		cmd->pids[(cmd->pid_i)++] = -2;
	return (result);
}
