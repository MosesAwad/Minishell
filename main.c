/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 16:09:52 by moses             #+#    #+#             */
/*   Updated: 2024/02/29 21:21:00 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	warn_message(void)
{
	ft_putstr_fd("Warning: Insuffcient memory", 2);
	ft_putstr_fd(". Undefined behavior to be expected\n", 2);
}

void	init_shell(t_minishell *minishell)
{
	minishell->cmd_line = NULL;
	minishell->ast = NULL;
	minishell->tok_list = NULL;
}

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
	while (minishell->env[i])
		free(minishell->env[i++]);
	free(minishell->env);
}

int	main(int argc, char *argv[], char *env[])
{
	t_minishell	minishell;

	(void) argc;
	(void) argv;
	//the minishell.env list should only be freed via signal or exit
	//because it has to stay for the whole program's run time. For
	//instance, I dont think this is really a signal but I free it
	//when use hits ctrl + d and returns NULL to get_next_line.
	minishell.binlist = assign_binlist();
	// minishell.binlist = NULL;
	minishell.env = setup_envlist(env);
	minishell.exit_status = 0;
	while (1)
	{
		init_shell(&minishell);
	
		ft_putstr_fd("minishell: ", STDOUT_FILENO);
		minishell.cmd_line = get_next_line(0);
		if (!minishell.cmd_line)		//otherwise we get a SEGV and we must exit to mimic actual Bash
		{
			delete_env_list(&minishell);
			delete_binlist(minishell.binlist);
			printf("exit\n");
			exit(0);
		}

		// printf("this is the env list\n");
		// print_env_list(minishell);

		tokenizer(&minishell.tok_list, minishell.cmd_line);
	
		// printf("---START OF LIST PRINTING---\n\n");
		// print_token_list(minishell.tok_list);
		// printf("---END OF LIST PRINTING---\n");

		if (!check_list_quotes(minishell.tok_list))
		{
			free(minishell.cmd_line);
			delete_list(minishell.tok_list);
			minishell.exit_status = 2;
			continue ;
		}

		// printf("\n\n\n");

		// printf("---START OF EXPANDED LIST PRINTING---\n\n");

		//expand_toklist(minishell.tok_list, minishell.env, minishell.exit_status);

		// print_token_list(minishell.tok_list);
		// printf("---END OF EXPANDED LIST PRINTING---\n");

		// printf("\n\n\n");

		printf("---START OF TREE PRINTING---\n\n");
		if (generate_ast(&minishell) == 0)
		{
			free(minishell.cmd_line);
			delete_list(minishell.tok_list);
			delete_node(minishell.ast);
			continue ;
		}
		printf("\033[0;34m");
		print_tree(minishell.ast, 0);
		printf("\033[0m");
		printf("---END OF TREE PRINTING---\n");

		// printf("\n\n\n");

		// printf("---START OF LIST CHECK POST TREE BUILD---\n\n");
		// print_token_list(minishell.tok_list);
		// printf("---END OF LIST CHECK POST TREE BUILD---\n");

		// printf("\n\n\n");
	
		// printf("---START OF TREE EXEC---\n\n");
		execute_job(&minishell);
		// printf("---END OF TREE EXEC--\n");

		//must free list and tree now after every iteration of the loop
		free(minishell.cmd_line);
		delete_list(minishell.tok_list);
		delete_node(minishell.ast);
	}
	return (0);
}
