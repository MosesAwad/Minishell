/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 01:46:58 by mawad             #+#    #+#             */
/*   Updated: 2024/02/28 23:44:04 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "Libft/libft.h"
# include "Parser/parser.h"
# include "Expander/expander.h"
# include "Execution/execution.h"
# include "Command/command.h"

typedef	struct s_binlist {
	char	**sublist;
}	t_binlist;

typedef struct s_minishell
{
	char		*cmd_line;
	t_token		*tok_list;
	t_binlist	*binlist;
	t_ASTree	*ast;
	char		**env;
	int			exit_status;
}	t_minishell;

//main.c
void		warn_message(void);
void		print_env_list(t_minishell minishell);
void		delete_env_list(t_minishell *minishell);
char		**setup_envlist(char *env[]);

//binlist_setup.c
t_binlist	*assign_binlist(void);
void		delete_binlist(t_binlist *binlist);

//Parser
//Parses/parse_setup.c
int			generate_ast(t_minishell *shell);

//Execution
//Execution/init_zone.c
int			init_command(t_command *command, int proc_count);
void		init_pipe_command(t_command *command);

//Execution/free_zone.c
void		delete_command(t_command *cmd);
void		delete_pipe_command(t_command *cmd);

//Execution/count_procs.c
int			count_procs(t_ASTree *node, t_minishell *shell);

//Execution/execution1.c
void		execute_job(t_minishell *shell);

//Execution/execution2.c
void		exec_smpl_command(t_minishell *shell, t_command *cmd);
void		execute_command(t_minishell *shell, t_command *command, int proc_count);

//Execution/file_dups.c
void		built_in_dup(t_command *cmd);
void		regular_dup(t_command *cmd);

//Execution/redir_handling.c
int			redir_handling(t_ASTree *rdrnode, t_minishell *shell, t_command *cmd);

//Execution/redir_handling_utils.c
void		memset_files_arr(t_command *cmd);
void		init_delims_and_file_arr(t_ASTree *rdrnode, t_command *cmd);
int			assign_fds(t_ASTree *rdrnode, char *str);
void		error_close_fds(t_command *cmd);

//Execution/file_handling.c
void		files_setup(t_ASTree *rdrnode, t_command *cmd);

//Execution/hdoc.c
void		hdoc(t_minishell *shell, t_command *cmd);

//Execution/execution_utils.c
int			check_in_binlist(t_minishell *shell, char *str);
void		handle_execve_error(t_minishell *shell, t_command *cmd);
int			check_built_ins(t_command *command);
int			call_built_ins(t_minishell *shell, t_command *command);
void		waiter(t_minishell *shell, t_command *cmd, int proc_count);

//Execution/file_handling.c
int		file_handling(t_ASTree *node, t_minishell *shell, t_command *cmd);

//Command
//Command/set_up_command.c
void		set_up_command(t_ASTree *rdrnode, t_minishell *shell, t_command *cmd);
void		print_cmd_args(t_command *cmd);

//Command/command_utils.c
char		*check_access(t_minishell *shell, t_command *cmd);

//Command/Cd/cd.c
int			ft_cd(t_minishell *shell, t_command *cmd);

//Command/Echo/echo.c
int			ft_echo(t_minishell *shell, t_command *cmd);

//Command/Env/env.c
int			ft_env(t_minishell *shell);

//Command/Export/export.c
int			ft_export(t_minishell *shell, t_command *cmd);

//Command/Export/export_no_flags.c
void		export_no_flags(t_minishell *minishell);

//Command/Export/export_w_flags.c
void		export_w_flags(t_minishell *shell, t_command *cmd);

//Command//Pwd/pwd.c
int			ft_pwd(t_minishell *shell);

//Command/Unset/unset.c
int			ft_unset(t_minishell *shell, t_command *cmd);

//Command/Exit/exit.c
int			ft_exit(t_minishell *shell, t_command *cmd);

#endif