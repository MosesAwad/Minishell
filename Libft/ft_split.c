/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 13:34:07 by mawad             #+#    #+#             */
/*   Updated: 2023/08/14 20:08:07 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static int	is_sep(char c, char sep)
{
	if (c == sep)
		return (1);
	else
		return (0);
}

static int	word_counter(const char *s, char sep)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] && is_sep(s[i], sep))
			i++;
		if (s[i] != '\0')
			count++;
		while (s[i] && !is_sep(s[i], sep))
			i++;
	}
	return (count);
}

int	free_bird(char **arr, int words)
{
	if (words && arr[words - 1] == NULL)
	{
		while (words > 0)
		{
			free(arr[words - 1]);
			arr[words - 1] = NULL;
			words--;
		}
		return (1);
	}
	return (0);
}

static void	array_filler(char **arr, const char *s, char sep, int *words)
{
	int	i;
	int	j;

	i = 0;
	while (s[i])
	{
		if (is_sep(s[i], sep) && s[i])
			i++;
		else
		{
			j = 0;
			while (s[i + j] != '\0' && !is_sep(s[i + j], sep))
				j++;
			arr[(*words)++] = malloc(sizeof(char) * (j + 1));
			if (free_bird(arr, *words) == 1)
				return ;
			ft_strlcpy(arr[(*words) - 1], s + i, j + 1);
			i = i + j;
		}
	}
}

char	**ft_split(char const *s, char c)
{
	int		words;
	char	**buffer;

	if (!s)
		return (NULL);
	words = word_counter(s, c);
	buffer = (char **)malloc((words + 1) * sizeof(char *));
	if (buffer == NULL)
		return (NULL);
	words = 0;
	array_filler(buffer, s, c, &words);
	if (free_bird(buffer, words) == 1)
	{
		free(buffer);
		return (NULL);
	}
	buffer[words] = NULL;
	return (buffer);
}

// #include<stdio.h>
// int main()
// {
// 	char **buffer;

// 	buffer = ft_split("", 'a');
// 	int i = 0;
// 	while (buffer[i])
// 	{
// 		printf("%s\n", buffer[i]);
// 		// free(buffer[i]);
// 		i++;
// 	}
// 	// free(buffer);
// 	return (0);
// }

// #include<stdio.h>
// int main()
// {
// 	int count = word_counter("chinimala", ' ');
//  	char **buffer = ft_split("chinimala", ' ');
// 	int i = 0;
// 		printf("%d\n", count);
// 	while (i < count)
// 	{
// 		printf("%s", buffer[i]);
// 		free(buffer[i++]);
// 	}
// 	free (buffer);
// 	return (0);
// }