/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 12:36:49 by mawad             #+#    #+#             */
/*   Updated: 2023/07/16 19:39:56 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*makebuf(char const *s, unsigned int start, size_t len, size_t slen)
{
	char	*buffer;
	size_t	remainder;

	remainder = 0;
	while (s[start + remainder] != '\0')
		remainder++;
	if (start + len > slen)
	{
		buffer = malloc((remainder + 1) * sizeof(*buffer));
		if (!buffer)
			return (NULL);
	}
	else
	{
		buffer = malloc((len + 1) * sizeof(*buffer));
		if (!buffer)
			return (NULL);
	}
	return (buffer);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*buffer;
	size_t	i;
	size_t	slen;

	if (!s)
		return (NULL);
	i = 0;
	slen = ft_strlen(s);
	if (start > slen)
		return (ft_strdup(""));
	buffer = makebuf(s, start, len, slen);
	if (!buffer)
		return (NULL);
	while (*(s + start) && i < len)
	{
		buffer[i++] = *((s++) + start);
	}
	buffer[i] = '\0';
	return (buffer);
}
