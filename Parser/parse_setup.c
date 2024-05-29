/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_setup.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 18:11:59 by moses             #+#    #+#             */
/*   Updated: 2024/05/28 16:10:58 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

	// <command line>	   		  ::= 	 <job> ';' <command line>
	// 					  		  |		 <job> ';'
	// 				  		  	  | 	 <job> '&' <command line>
	// 					  		  |		 <job> '&'
	// 							  |	     <job>

	// <job>			  	  	::=		<command> '|' <job>
	// 					  	 	 |		<command>

	// <command>		  	  	::=		<redirection list1> <simple command>
	//							|		<redirection list2>

	// <simple command>			::=		<pathname><redirection list><token list>

	// <token list>				::=		<token> <redirection list> <token list>
	// 							|		(EMPTY)

	// <redirection list1>		::=     <redirection> <redirection list>
	//							|   	(EMPTY)

	// <redirection list2>   	::=     <redirection> <redirection list>

	//	<redirection>			::=		'<' <filename>
	//							|   	'>' <filename>
	//					   	    |   	'<<' <filename>
	//					        |   	'>>' <filename>

#include "../minishell.h"

int	base_case(t_tokenType tok_type, t_token **tok_node, char **data)
{
	int	i;

	i = 0;
	if (*tok_node == NULL)
		return (0);
	if ((*tok_node)->type != tok_type)
		return (0);
	if (data)
	{
		if (!(*tok_node)->data)
			return (0);
		while ((*tok_node)->data[i])
			i++;
		*data = (char *)malloc(1 * (i + 1));
		if (!(*data))
			return (warn_message(), 0);
		ft_strlcpy(*data, (*tok_node)->data, i + 1);
	}
	(*tok_node) = (*tok_node)->next;
	return (1);
}

static void	ast_parse_messages(t_minishell *shell, t_token *head)
{
	t_token	*next;

	next = (shell->tok_list)->next;
	if (head->type == CHAR_PIPE)
	{
		ft_dprintf(2, "syntax error near unexpected token '|'\n");
		shell->exit_status = 2;
		return ;
	}
	if (next)
		ft_dprintf(2, "syntax error near unexpected token '%s'\n", next->data);
	else
		ft_dprintf(2, "syntax error near unexpected token 'newline'\n");
	shell->exit_status = 2;
}

//Must save the token_list because we need it back to free it from its
//head. The cmdline() traverses it and will loop it all the way to NULL.
//So to bring it back, we save it.
int	generate_ast(t_minishell *shell)
{
	t_token	*save;

	save = shell->tok_list;
	shell->ast = cmdline(&(shell->tok_list));
	if ((shell->tok_list) != NULL)
	{
		ast_parse_messages(shell, save);
		return (shell->tok_list = save, 0);
	}
	return (shell->tok_list = save, 1);
}
