/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 19:27:35 by mawad             #+#    #+#             */
/*   Updated: 2024/02/29 18:27:56 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_execve_error(t_minishell *shell, t_command *cmd)
{
	dup2(cmd->save_stdout, STDOUT_FILENO);
	dup2(cmd->save_stdin, STDIN_FILENO);
	ft_putstr_fd("command not found: ", 2);
	ft_putstr_fd(cmd->cmd_path, 2);
	ft_putchar_fd('\n', 2);
	close(cmd->save_stdout);
	close(cmd->save_stdin);
	delete_command(cmd);
	free(shell->cmd_line);
	delete_env_list(shell);
	delete_list(shell->tok_list);
	delete_binlist(shell->binlist);
	delete_node(shell->ast);
	exit(127);
}

int	check_in_binlist(t_minishell *shell, char *str)
{
	t_binlist	*binlist;
	int			i;
	int			j;

	binlist = shell->binlist;
	i = 0;
	while (i < 6)
	{
		j = 0;
		while (binlist[i].sublist && binlist[i].sublist[j])
		{
			if (!strncmp(binlist[i].sublist[j++], str, ft_strlen(str + 1)))
				return (1);
		}
		i++;
	}
	return (0);
}

int	check_built_ins(t_command *command)
{
	char	*cmd;
	int		len;

	if (!command->cmd_args)
		return (0);
	cmd = command->cmd_args[0];
	len = ft_strlen(cmd);
	if (!ft_strncmp(cmd, "echo", len))
		return (1);
	if (!ft_strncmp(cmd, "cd", len))
		return (1);
	if (!ft_strncmp(cmd, "pwd", len))
		return (1);
	if (!ft_strncmp(cmd, "export", len))
		return (1);
	if (!ft_strncmp(cmd, "unset", len))
		return (1);
	if (!ft_strncmp(cmd, "env", len))
		return (1);	
	if (!ft_strncmp(cmd, "exit", len))
		return (1);	
	return (0);
}

int	call_built_ins(t_minishell *shell, t_command *command)
{
	char	*cmd;
	int		len;

	cmd = command->cmd_args[0];
	len = ft_strlen(cmd);
	if (!ft_strncmp(cmd, "echo", len))
		return(ft_echo(shell, command), 1);
	if (!ft_strncmp(cmd, "cd", len))
		return (ft_cd(shell, command), 1);
	if (!ft_strncmp(cmd, "pwd", len))
		return (ft_pwd(shell), 1);
	if (!ft_strncmp(cmd, "export", len))
		return (ft_export(shell, command),1);
	if (!ft_strncmp(cmd, "unset", len))
		return (ft_unset(shell, command), 1);
	if (!ft_strncmp(cmd, "env", len))
		return (ft_env(shell), 1);	
	if (!ft_strncmp(cmd, "exit", len))
		return (ft_exit(shell, command), 1);	
	return (0);
}

//Since we only care about the exit status of the last command and
//I am using the same waiter function for when we execute_pipeline
//and when we execute_command. So whether we have a pipe or not,
//the same waiter function is being used. However, since I cannot
//wait on a built-in function, what happens is that inside both the
//execute_pipeline and execute_command functions, there is a waiter
//function call after the exec_simple_command function is done. So,
//in a scenario like [rcho "Hi" | env], what would happen is that
//env would set the shell->exit_status to 0 but after exec_smpl_cmd
//is done and we go to waiters, waiters now sets the shell->exit_status
//to something like 512 because command rcho is not found. But I only
//want the exit status of env because it is the last. That is why I 
//have the 	if (check_built_ins(cmd)), where the cmd passed as an 
//argument to the waiter will be that of the last command, I simply
//just return. Don't let rcho's status update the shell->exit_status.
//IT'S SO UGLY BUT IT WORKS AND I DON'T WANT TO REWORK MY METHOD AND
//FUNCTIONS AROUND THIS SILLY ISSUE JUST TO MAKE THE CODE FEEL MORE
//ELEGANT.
//Also the 	if (i != 0) condition is simply there to not alter
//the exit status to the save_status variable if we have a
//built_in function or if the cmdpath is the name of a directory.
//I handle the exit status for these elsewhere, but all commands
//irregardless go through waiter at the end. Not all would be waited
//for though, if I have a built-in command or if the command passed
//was the name of a directory, then I set proc_count to 0 so the while
//loop wont happen. But after the while loop, I do not want to alter
//the exit status to save_status unless the command was not
//built-in (even commands with incorrect names) as those are the
//only ones passed to execve; those are the only ones to be waited
//for, and these are the only ones.
void	waiter(t_minishell *shell, t_command *cmd, int proc_count)
{
	int	i;
	int	save_status;

	i = 0;
	save_status = 0;
	while (i < proc_count)
	{
		waitpid(cmd->pids[i], &save_status, 0);
		i++;
	}
	if (i != 0)
	{
		if (WIFEXITED(save_status))
			save_status = WEXITSTATUS(save_status);
		shell->exit_status = save_status;
	}
}
