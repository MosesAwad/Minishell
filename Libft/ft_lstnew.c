/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 19:49:34 by mawad             #+#    #+#             */
/*   Updated: 2023/07/11 19:50:44 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*buffer;

	buffer = (t_list *)malloc(sizeof(t_list));
	if (buffer == NULL)
		return (NULL);
	buffer->content = content;
	buffer->next = NULL;
	return (buffer);
}
