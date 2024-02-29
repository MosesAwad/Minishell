/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moses <moses@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 16:28:13 by moses             #+#    #+#             */
/*   Updated: 2024/02/08 18:16:16 by moses            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

typedef enum e_tokenType
{
	CHAR_HDOC = -3,
	CHAR_APND = -2,
	CHAR_GEN = -1,
	CHAR_PIPE = '|',
	CHAR_SQUOTE = '\'',
	CHAR_DQUOTE = '\"',
	CHAR_RDIN = '<',
	CHAR_RDOUT = '>',
	CHAR_SPC = ' ',
	CHAR_TAB = '\t',
	CHAR_NL = '\n'
}	t_tokenType;

enum e_state
{
	IN_SQUOTE,
	IN_DQUOTE,
	STATE_GENERAL
};

typedef struct s_token
{
	char			*data;
	int				type;
	struct s_token	*next;
}	t_token;

//check_toklist_quotes.c
int		check_list_quotes(t_token *tok_list);

//get_next_line.c
char	*get_next_line(int fd);

//token_utils.c
int		is_wspace(char c);
void	add_to_list(t_token **lst, t_token *new);
void	print_token_list(t_token *list);
void	add_node_data(t_token *tok_node, t_tokenType type);
void	delete_list(t_token *token_list);

//quote_handler.c
void	single_quote_handle(t_token *tok_node, char *str, int *j);
void	double_quote_handler(t_token *tok_node, char *str, int *j);

//general_case_handler.c
int		set_state(char quote);
int		general_case_handler(t_token *tok_node, char *str, int j);

//token.c
void	tokenizer(t_token **tok_list, char *str);

#endif