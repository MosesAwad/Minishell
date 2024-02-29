/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 14:59:06 by mawad             #+#    #+#             */
/*   Updated: 2023/07/07 15:12:16 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*buffer;
	size_t			len;
	unsigned int	i;

	if (!s || !f)
		return (NULL);
	i = 0;
	len = ft_strlen(s);
	buffer = (char *)malloc((len + 1) * sizeof(*buffer));
	if (buffer == NULL)
		return (NULL);
	while (s[i])
	{
		buffer[i] = (*f)(i, s[i]);
		i++;
	}
	buffer[i] = '\0';
	return (buffer);
}
