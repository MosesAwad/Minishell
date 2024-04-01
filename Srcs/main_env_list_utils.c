/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_env_list_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 17:48:37 by mawad             #+#    #+#             */
/*   Updated: 2024/03/04 17:51:34 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//This function is used to append the current directory to the PATH
//variable at launch so that the user can run minishell within
//minishell. The original minishell will always be in this folder
//and thus, if we add the current directory to the PATH when the
//user first launches the program, then execve would be able to
//find our minishell executable and run it. If path has been unset,
//then this function before minishell is launched, then this function
//would do nothing (just not SEGFAULT lol).
//Malloc with a plus 2 because I want to allocate space for the ':'
//symbol and for the null terminator.
char	*append_to_path(char *path)
{
	char	*current_dir;
	char	*new_path;
	int		i;
	int		j;

	current_dir = getcwd(NULL, 0);
	if (!current_dir)
		return (ft_strdup(path));
	i = 0;
	new_path = malloc(ft_strlen(path) + ft_strlen(current_dir) + 2);
	if (!new_path)
		return (warn_message(), free(current_dir), ft_strdup(path));
	while (path[i])
	{
		new_path[i] = path[i];
		i++;
	}
	new_path[i++] = ':';
	j = 0;
	while (current_dir[j])
		new_path[i++] = current_dir[j++];
	new_path[i] = '\0';
	return (free(current_dir), new_path);
}

char	**setup_envlist(char *env[])
{
	int		i;
	char	**buffer;

	i = 0;
	if (!env)
		return (NULL);
	while (env[i])
		i++;
	buffer = (char **)malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], "PATH=", 5))
			buffer[i] = append_to_path(env[i]);
		else
			buffer[i] = ft_strdup(env[i]);
		i++;
	}
	buffer[i] = NULL;
	return (buffer);
}

void	print_env_list(t_minishell minishell)
{
	int	i;

	i = 0;
	while (minishell.env[i])
		printf("%s\n", minishell.env[i++]);
	printf("\n");
}

void	delete_env_list(t_minishell *minishell)
{
	int	i;

	i = 0;
	if (!minishell->env)
		return ;
	while (minishell->env[i])
		free(minishell->env[i++]);
	free(minishell->env);
}
