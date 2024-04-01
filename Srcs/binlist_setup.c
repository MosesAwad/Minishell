/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binlist_setup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 03:11:35 by mawad             #+#    #+#             */
/*   Updated: 2024/03/04 17:51:46 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	entry_count(char *dir_name)
{
	DIR				*dir;
	int				i;
	struct dirent	*entry;

	i = 0;
	dir = opendir(dir_name);
	if (!dir)
		return (0);
	entry = readdir(dir);
	if (entry->d_type == DT_REG)
		i++;
	while (entry != NULL)
	{
		if (entry->d_type == DT_REG)
			i++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (i);
}

static char	**get_dir_contents(char *dir_name)
{
	DIR				*dir;
	char			**buffer;
	int				i;
	int				j;
	struct dirent	*entry;

	i = entry_count(dir_name);
	if (i == 0)
		return (NULL);
	j = 0;
	dir = opendir(dir_name);
	buffer = (char **)malloc(sizeof(char *) * (i + 1));
	entry = readdir(dir);
	if (entry->d_type == DT_REG)
		buffer[j++] = ft_strdup(entry->d_name);
	while (entry != NULL)
	{
		if (entry->d_type == DT_REG)
			buffer[j++] = ft_strdup(entry->d_name);
		entry = readdir(dir);
	}
	buffer[j] = NULL;
	closedir(dir);
	return (buffer);
}

t_binlist	*assign_binlist(void)
{
	t_binlist	*full_list;

	full_list = malloc(sizeof(*full_list) * (6));
	full_list[0].sublist = get_dir_contents("/bin");
	full_list[1].sublist = get_dir_contents("/sbin");
	full_list[2].sublist = get_dir_contents("/usr/bin");
	full_list[3].sublist = get_dir_contents("/usr/sbin");
	full_list[4].sublist = get_dir_contents("/usr/local/bin");
	full_list[5].sublist = get_dir_contents("/usr/local/bdsfin");
	return (full_list);
}

void	delete_binlist(t_binlist *binlist)
{
	int	i;
	int	j;

	i = 0;
	if (!binlist)
		return ;
	while (i < 6)
	{
		j = 0;
		while (binlist[i].sublist && binlist[i].sublist[j])
			free(binlist[i].sublist[j++]);
		free(binlist[i].sublist);
		i++;
	}
	free(binlist);
}
