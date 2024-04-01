/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 01:46:58 by mawad             #+#    #+#             */
/*   Updated: 2024/03/04 17:49:32 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "Libft/libft.h"
# include "Parser/parser.h"
# include "Expander/expander.h"
# include "Execution/execution.h"
# include "ft_dprintf/ft_dprintf.h"

extern int	g_sigstat;

enum e_loop
{
	e_break,
	e_continue,
	e_normal
};

enum e_sig
{
	IN_SHELL,
	IN_CHILD,
	IN_HDOC,
	OUT_HDOC
};

typedef struct s_binlist
{
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

//main_utils.c
void		main_cleanup(t_minishell *shell);
int			check_signal(t_minishell *shell);
void		check_cmdline(t_minishell *shell);
int			parse_quotes(t_minishell *shell);
int			check_ast(t_minishell *shell);

//main_env_list_utils.c
char		*append_to_path(char *path);
char		**setup_envlist(char *env[]);
void		print_env_list(t_minishell minishell);
void		delete_env_list(t_minishell *minishell);

//signals.c
void		init_child_signals(void);
void		init_signals(void);

//binlist_setup.c
t_binlist	*assign_binlist(void);
void		delete_binlist(t_binlist *binlist);

//general_utils.c
void		delete_2d_arr(char **arr);

//general_utils2.c
char		**split_with_quotes(char *str);

//Parser
//Parses/parse_setup.c
int			generate_ast(t_minishell *shell);

//Execution
//Execution/1-execute_job.c
void		execute_job(t_minishell *shell, t_ASTree *tree);

//Execution/2-execute_pipeline.c
void		execute_pipeline(t_minishell *shell, t_command *cmd,
				int proc_count);

//Execution/3-execute_command.c
void		execute_command(t_minishell *shell, t_command *command,
				int proc_count);

//Execution/4-execute_simple_command.c
int			exec_smpl_command(t_minishell *shell, t_command *cmd);

//Execution/execution_error_handling.c
int			no_permissions(t_minishell *shell, t_command *cmd);

//Execution/execution_error_handling2.c
void		handle_execve_error(t_minishell *shell, t_command *cmd);
int			check_directory(t_minishell *shell, t_command *cmd);

//Execution/init_zone.c
int			init_command(t_command *command, int proc_count);
void		init_pipe_command(t_command *command);

//Execution/free_zone.c
void		delete_command(t_command *cmd);
void		delete_pipe_command(t_command *cmd);

//Execution/file_dups.c
void		built_in_dup(t_command *cmd);
void		regular_dup(t_command *cmd);

//Execution/redir_handling.c
int			redir_handling(t_ASTree *rdrnode, t_minishell *shell,
				t_command *cmd);

//Execution/redir_handling_utils.c
void		memset_files_arr(t_command *cmd);
void		init_delims_and_file_arr(t_ASTree *rdrnode, t_command *cmd);
int			assign_fds(t_ASTree *rdrnode, char *str);
void		error_close_fds(t_command *cmd);

//Execution/file_handling.c
int			file_handling(t_ASTree *node, t_minishell *shell, t_command *cmd);

//Execution/files_setup.c
void		files_setup(t_ASTree *rdrnode, t_command *cmd);

//Execution/files_utils.c
void		set_infile(t_command *cmd);
void		set_outfile(t_command *cmd);
void		set_appendfile(t_command *cmd);
void		set_mode(t_ASTree *rdrnode, t_command *cmd);
void		copy_file_data(t_command *cmd, int ind, int mode);

//Execution/file_handling_utils.c
char		*ft_get_key(char *str);
int			check_ambiguous(t_minishell *shell, char *str);

//Execution/hdoc.c
void		hdoc(t_minishell *shell, t_command *cmd);

//Execution/execution_utils.c
int			check_in_binlist(t_minishell *shell, char *str);
void		handle_execve_error(t_minishell *shell, t_command *cmd);
int			check_built_ins(t_command *command);
int			call_built_ins(t_minishell *shell, t_command *command);
void		waiter(t_minishell *shell, t_command *cmd, int proc_count,
				int stat_flag);

//Execution/execution_utils2.c
void		ft_cleanup(t_minishell *shell, t_command *cmd);
void		do_pipe(t_minishell *shell, t_command *cmd, int *fd);
int			count_procs(t_ASTree *node);

//Execution/file_handling.c
int			file_handling(t_ASTree *node, t_minishell *shell, t_command *cmd);

//Command
//Command/set_up_command.c
int			count_cmd_args(t_ASTree *node, t_minishell *shell);
void		set_up_cmd_members(t_ASTree *node, t_minishell *shell,
				t_command *cmd);

//Command/command_utils.c
char		*check_access(t_minishell *shell, t_command *cmd);
void		set_up_command(t_ASTree *node, t_minishell *shell, t_command *cmd);
void		print_cmd_args(t_command *cmd);

//Command/Cd/cd.c
int			ft_cd(t_minishell *shell, t_command *cmd);

//Command/Cd/cd_utils.c
int			count_cd_args(t_command *cmd);
int			bool_key_match(char *s1, char *s2);
char		*copy_new_environ(char *env_var, char *new_val);
char		**update_env_list(char **env_list, char *var, char *new_val);

//Command/Echo/echo.c
int			ft_echo(t_minishell *shell, t_command *cmd);

//Command/Env/env.c
int			ft_env(t_minishell *shell);

//Command/Export/export.c
int			ft_export(t_minishell *shell, t_command *cmd);

//Command/Export/export_no_flags.c
void		export_no_flags(t_minishell *minishell);

//Export/export_w_flags_utils.c
int			check_export_syntax(char *str);
int			bool_same_key(char *s1, char *s2);
char		*update_value(char *s1, char *s2);
int			check_same_key(char **env_list, char *str);

//Export/export_w_flags_utils2.c
int			check_export_syntax(char *str);

//Export/export_w_flags.c
int			bool_no_update(char *s1, char *s2);
int			check_no_update(char **env_list, char *str);
char		**add_to_loc_list(char **env_list, char *str);
void		export_w_flags(t_minishell *shell, t_command *cmd);

//Command//Pwd/pwd.c
int			ft_pwd(t_minishell *shell);

//Command/Unset/unset.c
int			ft_unset(t_minishell *shell, t_command *cmd);

//Command/Exit/exit.c
int			ft_exit(t_minishell *shell, t_command *cmd);

//Command/Exit/exit_utils.c
int			count_exit_args(t_command *cmd);
int			bool_numeric(char *str);
void		full_free(t_minishell *shell, t_command *cmd);

#endif