/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_handling_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 09:57:42 by mawad             #+#    #+#             */
/*   Updated: 2024/03/03 10:00:40 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//start at i = 1 because I want to skip the dollar sign
char	*ft_get_key(char *str)
{
	char	*buffer;
	int		i;
	int		j;

	i = 1;
	while (str[i] && !ft_strchr("/", str[i]))
		i++;
	buffer = (char *)malloc(i + 1);
	if (!buffer)
		return (warn_message(), NULL);
	i = 1;
	j = 0;
	while (str[i] && !ft_strchr("/", str[i]))
		buffer[j++] = str[i++];
	buffer[j] = '\0';
	return (buffer);
}

//One thing that can cause an ambigous redirect is if
//we redirect to an environment variable with a space
//in it (and no quotes around it). It will always cause an
//ambigous redirect no matter what. For example, if we do
//[export HI="hey lol"], what happens is if we do now 
//[echo "random" >$HI], this cause an ambigous redireciton.
//But if we do [echo "random" >"$HI"], then it's fine. With
//quotes it actually does create a file with the name [hey lol].
//So something like [echo "random" >$HI] is what I am trying 
//to handle with the function below.
int	check_ambiguous(t_minishell *shell, char *str)
{
	char	*key;
	int		i;
	int		j;

	key = NULL;
	i = 0;
	if (!str)
		return (0);
	if (str[0] != '$')
		return (0);
	key = ft_get_key(str);
	while (shell->env[i] && !bool_same_key(shell->env[i], key))
		i++;
	if (!(shell->env[i]))
		return (free(key), 0);
	j = 0;
	while (shell->env[i][j])
	{
		if (is_wspace(shell->env[i][j++]))
			return (free(key), 1);
	}
	return (free(key), 0);
}
