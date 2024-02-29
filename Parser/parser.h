/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moses <moses@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 17:50:12 by moses             #+#    #+#             */
/*   Updated: 2024/02/16 04:21:59 by moses            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "../Tokenization/tokenizer.h"

typedef enum e_nodeType
{
	NODE_PIPE = 1,
	NODE_CMDPATH = 2,
	NODE_ARG = 3,
	NODE_RDIN = 4,
	NODE_RDOUT = 5,
	NODE_HDOC = 6,
	NODE_APND = 7,
	NODE_RDRGEN = 8
}	t_nodeType;

typedef struct s_ASTree
{
	int				type;
	char			*data;
	struct s_ASTree	*left;
	struct s_ASTree	*right;
}	t_ASTree;

//ASTree_utils.c
void		print_tree(t_ASTree *node, int space);
void		set_node_type(t_ASTree *node, t_nodeType nodeType);
void		set_node_data(t_ASTree *node, char *nodeData);
void		attach_branch(t_ASTree *node, t_ASTree *left_n, t_ASTree *right_n);
t_ASTree	*copy_node(t_ASTree *src);
void		delete_node(t_ASTree *node);

//parse_setup.c
int			base_case(t_tokenType tok_type, t_token **tok_node, char **data);

//1-cmdline_level.c
t_ASTree	*cmdline(t_token **token_list);
t_ASTree	*cmdline5(t_token **token_list);

//2-job_level.c
t_ASTree	*job(t_token **token_list);
t_ASTree	*job1(t_token **token_list);
t_ASTree	*job2(t_token **token_list);

//3-1-command_level.c
t_ASTree	*command(t_token **token_list);
t_ASTree	*command1(t_token **token_list);
t_ASTree	*command2(t_token **token_list);

//3-2-command_level_utils.c
t_ASTree	*case_checker1(t_ASTree *rdrlist1, t_ASTree *rdrlist2, t_ASTree *smplcmdnode);
t_ASTree	*case_checker2(t_ASTree *rdrlist2, t_ASTree *smplcmdnode);
t_ASTree	*assign_rdrlist2(t_ASTree *smplcmdnode);

//4-redir_list_level.c
t_ASTree	*redirlist(t_token **token_list);
t_ASTree	*redirlist1(t_token	**token_list);
t_ASTree	*redirlist2(t_token	**token_list);
t_ASTree	*redirlist3(t_token	**token_list);
t_ASTree	*redirlist4(t_token	**token_list);

//5-1-simple_command_level.c
t_ASTree	*smpl_command(t_token **token_list);
t_ASTree	*smpl_command1(t_token	**token_list);

//5-2-simple_command_level_utils.c
t_ASTree	*get_right_node(t_ASTree *node);
t_ASTree	*push_rdlst_up(t_ASTree *node);
void		append_to_rdrlist(t_ASTree *rdrlistnode, t_ASTree *argnode);

//6-arg_list_level.c
t_ASTree	*arg_list(t_token **token_list);
t_ASTree	*arg_list1(t_token **token_list);
t_ASTree	*arg_list2(t_token **token_list);

#endif