/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 16:09:52 by moses             #+#    #+#             */
/*   Updated: 2024/05/29 15:37:28 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	g_sigstat = IN_SHELL;

void	warn_message(void)
{
	ft_putstr_fd("Warning: Insufficient memory", 2);
	ft_putstr_fd(". Undefined behavior to be expected\n", 2);
}

void	init_shell(t_minishell *minishell)
{
	minishell->cmd_line = NULL;
	minishell->ast = NULL;
	minishell->tok_list = NULL;
}

int	main(int argc, char *argv[], char *env[])
{
	t_minishell	minishell;

	(void) argc;
	(void) argv;
	minishell.binlist = assign_binlist();
	minishell.env = setup_envlist(env);
	minishell.exit_status = 0;
	while (1)
	{
		init_signals();
		init_shell(&minishell);
		minishell.cmd_line = readline("minishell: ");
		if (check_signal(&minishell) == e_continue)
			continue ;
		check_cmdline(&minishell);
		tokenizer(&minishell.tok_list, minishell.cmd_line);
		if (parse_quotes(&minishell) == e_continue)
			continue ;
		if (check_ast(&minishell) == e_continue)
			continue ;
		execute_job(&minishell, minishell.ast);
		main_cleanup(&minishell);
	}
	return (0);
}

// int	main(int argc, char *argv[], char *env[])
// {
// 	t_minishell	minishell;

// 	(void) argc;
// 	(void) argv;
// 	//the minishell.env list should only be freed via signal or exit
// 	//because it has to stay for the whole program's run time. For
// 	//instance, I dont think this is really a signal but I free it
// 	//when use hits ctrl + d and returns NULL to get_next_line.
// 	minishell.binlist = assign_binlist();
// 	// minishell.binlist = NULL;
// 	minishell.env = setup_envlist(env);
// 	minishell.exit_status = 0;
// 	while (1)
// 	{
// 		init_signals();
// 		init_shell(&minishell);

// 		minishell.cmd_line = readline("minishell: ");
// 		if (g_sigstat == 130)
// 		{
// 			free(minishell.cmd_line);
// 			minishell.exit_status = g_sigstat;
// 			g_sigstat = IN_SHELL;
// 			continue ;
// 		}
// 		if (!minishell.cmd_line)
// 		{
// 			free(minishell.cmd_line);
// 			delete_env_list(&minishell);
// 			delete_binlist(minishell.binlist);
// 			printf("exit\n");
// 			exit(0);
// 		}
// 		else
// 			add_history(minishell.cmd_line);

// 		// printf("this is the env list\n");
// 		// print_env_list(minishell);
// 		tokenizer(&minishell.tok_list, minishell.cmd_line);

// 		// printf("---START OF LIST PRINTING---\n\n");
// 		// print_token_list(minishell.tok_list);
// 		// printf("---END OF LIST PRINTING---\n");

// 		if (!check_list_quotes(minishell.tok_list))
// 		{
// 			free(minishell.cmd_line);
// 			delete_list(minishell.tok_list);
// 			minishell.exit_status = 2;
// 			continue ;
// 		}

// 		// printf("\n\n\n");

// 		// printf("---START OF EXPANDED LIST PRINTING---\n\n");

// 		//expand_toklist(minishell.tok_list, minishell.env,
//							minishell.exit_status);

// 		// print_token_list(minishell.tok_list);
// 		// printf("---END OF EXPANDED LIST PRINTING---\n");

// 		// printf("\n\n\n");

// 		// printf("---START OF TREE PRINTING---\n\n");
// 		if (generate_ast(&minishell) == 0)
// 		{
// 			free(minishell.cmd_line);
// 			delete_list(minishell.tok_list);
// 			delete_node(minishell.ast);
// 			continue ;
// 		}
// 		// printf("\033[0;34m");
// 		// print_tree(minishell.ast, 0);
// 		// printf("\033[0m");
// 		// printf("---END OF TREE PRINTING---\n");

// 		// printf("\n\n\n");

// 		// printf("---START OF LIST CHECK POST TREE BUILD---\n\n");
// 		// print_token_list(minishell.tok_list);
// 		// printf("---END OF LIST CHECK POST TREE BUILD---\n");

// 		// printf("\n\n\n");

// 		// printf("---START OF TREE EXEC---\n\n");
// 		execute_job(&minishell, minishell.ast);
// 		// printf("---END OF TREE EXEC--\n");

// 		//must free list and tree now after every iteration of the loop
// 		free(minishell.cmd_line);
// 		delete_list(minishell.tok_list);
// 		delete_node(minishell.ast);
// 	}
// 	return (0);
// }
