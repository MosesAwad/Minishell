/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 13:24:52 by mawad             #+#    #+#             */
/*   Updated: 2023/07/14 14:49:57 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	start_finder(char const *s1, char const *set)
{
	int	i;
	int	j;

	i = 0;
	while (s1[i] != '\0')
	{
		j = 0;
		while (set[j] != '\0')
		{
			if (s1[i] == set[j])
				break ;
			j++;
		}
		if (set[j] == '\0')
			return (i);
		i++;
	}
	if (i == 0)
		return (0);
	else
		return (i - 1);
}

static int	end_finder(char const *s1, char const *set)
{
	int	i;
	int	j;

	i = ft_strlen(s1) - 1;
	while (i >= 0)
	{
		j = 0;
		while (set[j] != '\0')
		{
			if (s1[i] == set[j])
				break ;
			j++;
		}
		if (set[j] == '\0')
			return (i);
		i--;
	}
	return (i + 1);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start_ind;
	int		end_ind;
	char	*buffer;

	if (!s1 || !set)
		return (NULL);
	start_ind = start_finder(s1, set);
	end_ind = end_finder(s1, set);
	if ((start_ind > end_ind) || (*s1 == '\0'))
	{
		buffer = ft_strdup("");
		return (buffer);
	}
	buffer = (char *)malloc(((end_ind - start_ind + 1) + 1) * sizeof(*buffer));
	if (buffer == NULL)
		return (NULL);
	ft_strlcpy(buffer, s1 + start_ind, (end_ind - start_ind + 1) + 1);
	return (buffer);
}
