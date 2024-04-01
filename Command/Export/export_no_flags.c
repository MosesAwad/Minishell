/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_no_flags.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 19:01:24 by mawad             #+#    #+#             */
/*   Updated: 2024/03/05 12:11:16 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	**copy_envlist(char *env[])
{
	int		i;
	char	**buffer;

	i = 0;
	while (env[i])
		i++;
	buffer = (char **)malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (env[i])
	{
		buffer[i] = ft_strdup(env[i]);
		i++;
	}
	buffer[i] = NULL;
	return (buffer);
}

void	alphabetic_order(char **elist, int (*in_order)())
{
	int		size;
	int		i;
	char	*temp;

	i = 0;
	size = 0;
	while (elist[size])
		size++;
	while (size > 1)
	{
		i = 0;
		while (i < size - 1)
		{
			if ((*in_order)(elist[i], elist[i + 1]) == 0)
			{
				temp = elist[i];
				elist[i] = elist[i + 1];
				elist[i + 1] = temp;
			}
			i++;
		}
		size--;
	}
}

int	in_order(char *str1, char *str2)
{
	int	j;

	j = 1;
	while (!ft_strncmp(str1, str2, j))
		j++;
	if (str1[j - 1] > str2[j - 1])
		return (0);
	return (1);
}

void	add_prefix(char *env[])
{
	int	i;
	int	j;

	i = 0;
	alphabetic_order(env, in_order);
	while (env[i])
	{
		j = 0;
		ft_putstr_fd("delcare -x ", STDOUT_FILENO);
		while (env[i][j] != '=' && env[i][j])
			ft_putchar_fd(env[i][j++], STDOUT_FILENO);
		if (env[i][j])
		{
			ft_putchar_fd(env[i][j++], STDOUT_FILENO);
			ft_putchar_fd('\"', STDOUT_FILENO);
			while (env[i][j])
				ft_putchar_fd(env[i][j++], STDOUT_FILENO);
			ft_putstr_fd("\"", STDOUT_FILENO);
		}
		ft_dprintf(1, "\n");
		i++;
	}
}

//we copy the list here because we do not want to
//make alerations to minishell elist struct. We just
//rearrange it for the purposes of this function and
//print the sorted list with the declare -x prefix and
//tehe quotes before and after the value in the key-value
//pairs and that's it. Then we free this copy_elist and we
//get on with the program.
void	export_no_flags(t_minishell *minishell)
{
	char	**elist_copy;
	int		i;

	i = 0;
	elist_copy = copy_envlist(minishell->env);
	add_prefix(elist_copy);
	while (elist_copy[i])
		free(elist_copy[i++]);
	free(elist_copy);
}

/*
int main(int argc, char *argv[], char *env[])
{
	(void)argc;
	(void)argv;

	printf("----BEFORE REARRANGING----\n");
	for (int i = 0; env[i] != NULL; i++) {
		printf("%s\n", env[i]);
	}

	printf("----AFTER REARRANGING----\n");
	add_prefix(env);

	return (0);
}*/
