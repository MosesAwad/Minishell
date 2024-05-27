/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_up_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 23:33:31 by mawad             #+#    #+#             */
/*   Updated: 2024/05/27 17:25:05 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//This function attempts to solve the following issue:
//Command: echo hi "lol" "wtf" $EMPTY "hey"
//Output: hi lol wtf hey (no space b/w wtf and hey)
//
//Command: echo hi "lol" "wtf" "$EMPTY" "hey"
//Output" hi lol wtf  hey (space b/w wtf and hey)
//
//So what happens is that for example, we have a command
//like $EMPTY echo hi, then my tree would have the
//NODE_CMDPATH as $EMPTY and then the args as
//echo and hi. But what I am doing is, my tree is built
//wrong, well not wrong but I cant expand at the tree yet
//so how can I know if it's empty then, well I cant. So
//how do I work around this? What I do is that I set up
//my command struct to ignore the $EMPTY and to build the
//cmd->cmd_path and cmd->cmd_args array correctly.
static int	check_empty_expand(t_minishell *shell, char *str)
{
	char	*expansion;

	if (str[0] != '$')
		return (0);
	expansion = expand_str(str, shell->env, shell->exit_status);
	if (!expansion[0])
		return (free(expansion), 1);
	return (free(expansion), 0);
}

//Upon reviewing the subject pdf, I noticed that I technically don't
//need to expand the string whilst maintaining the quotes via the
//function q_expand_str; then proceed to splitting the line (where 
//the delimeter is any whitespace) whilst mainting the quotes; and then
//strip the string of the quotes. That's unecessary, you can just straight
//up check if the expansion is empty, if it is not, then just expand the
//string and strip the quotes. No need for split_with_quotes(). 
//If you're intersted, I explain above split_with_quotes() function
//why this is no longer necessary.
static void	copy_cmd_args(t_ASTree *node, t_minishell *shell, t_command *cmd)
{
	char	*expansion;
	char	**split_expansion;
	int		i;
	int		j;

	j = 0;
	while (node)
	{
		i = 0;
		if (!check_empty_expand(shell, node->data))
		{
			expansion = q_expand_str(node->data, shell->env,
					shell->exit_status);
			split_expansion = split_with_quotes(expansion);
			while (split_expansion[i])
				cmd->cmd_args[j++] = strip_quotes(split_expansion[i++]);
			free(expansion);
			delete_2d_arr(split_expansion);
		}
		node = node->left;
	}
	cmd->cmd_args[j] = NULL;
}

int	count_cmd_args(t_ASTree *node, t_minishell *shell)
{
	char	*expansion;
	char	**split_expansion;
	int		arg_count;
	int		i;

	arg_count = 0;
	while (node)
	{
		i = 0;
		if (!check_empty_expand(shell, node->data))
		{
			expansion = q_expand_str(node->data, shell->env,
					shell->exit_status);
			split_expansion = split_with_quotes(expansion);
			while (split_expansion[i])
			{
				arg_count++;
				i++;
			}
			delete_2d_arr(split_expansion);
			free(expansion);
		}
		node = node->left;
	}
	return (arg_count);
}

void	set_up_cmd_members(t_ASTree *node, t_minishell *shell, t_command *cmd)
{
	copy_cmd_args(node, shell, cmd);
	if (cmd->cmd_args[0])
		cmd->cmd_path = ft_strdup(cmd->cmd_args[0]);
	else
		cmd->cmd_path = NULL;
}

// static void	copy_cmd_path(t_ASTree *node, t_minishell *shell,
//									t_command *cmd)
// {
// 	char	*expansion;
// 	int		len;

// 	while (node)
// 	{
// 		if (!check_empty_expand(shell, node->data))
// 		{
// 			expansion = q_expand_str(node->data, shell->env,
// 					shell->exit_status);
// 			len = ft_strlen(expansion);
// 			cmd->cmd_path = (char *)malloc(len + 1);
// 			ft_strlcpy(cmd->cmd_path, expansion, len + 1);
// 			free(expansion);
// 			break ;
// 		}
// 		node = node->left;
// 	}
// 	if (!node)
// 		cmd->cmd_path = NULL;
// }
