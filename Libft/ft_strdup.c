/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 20:31:38 by mawad             #+#    #+#             */
/*   Updated: 2023/07/06 18:02:00 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	int		slen;
	char	*buffer;
	int		i;

	slen = ft_strlen(s);
	buffer = (char *) malloc((slen + 1) * sizeof(*buffer));
	i = 0;
	if (buffer == NULL)
		return (NULL);
	while (s[i])
	{
		buffer[i] = s[i];
		i++;
	}
	buffer[i] = '\0';
	return (buffer);
}
