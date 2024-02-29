
#include "../minishell.h"

//I have the if (shell->env[i] == NULL) check
//just in case a user unsets the PATH variable.
//Don't want it to SEGFAULT if that happens.
char	**get_path_list(t_minishell *shell)
{
	char	**buffer;
	int	i;

	i = 0;
	while (shell->env[i])
	{
		if (!ft_strncmp("PATH", shell->env[i++], 4))
			break ;
	}
	if (shell->env[i] == NULL)
		return (NULL);
	i--;
	buffer = ft_split(shell->env[i] + 5, ':');
	i = 0;
	while (buffer[i])
	{
		buffer[i] = ft_strjoin(buffer[i], "/");
		i++;
	}
	return (buffer);
}

//ft_strjoin frees path_list[i] that is passed to it
//as an argument. So if the loop breaks, I don't need to
//reset i, just loop through the rest of the elements in
//path_list and free them because the previous ones have
//already been freed by path[i]. I designed it such that
//that this functionr returns NULL if the user unsets the
//envar list because then get_path_list (function) would
//return NULL. So, full_path in here is initalized to NULL
//and we wont enter any of the while loops so we end up
//returning NULL
char	*check_access(t_minishell *shell, t_command *cmd)
{
	int		i;
	char	**path_list;
	char	*full_path;

	i = 0;
	path_list = get_path_list(shell);
	if (!path_list)
		return (NULL);
	full_path = NULL;
	while (path_list[i])
	{
		full_path = ft_strjoin(path_list[i++], cmd->cmd_path);
		if (access(full_path, F_OK) == -1)
		{
			free(full_path);
			full_path = NULL;
		}
		else
			break ;
	}
	while (path_list[i])
		free(path_list[i++]);
	free(path_list);
	return (full_path);
}
