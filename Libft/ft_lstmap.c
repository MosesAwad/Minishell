/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 20:03:21 by mawad             #+#    #+#             */
/*   Updated: 2023/08/14 20:09:06 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*new_node;

	new_list = NULL;
	if (f == NULL || del == NULL || lst == NULL)
		return (NULL);
	while (lst != NULL)
	{
		new_node = (t_list *)malloc(sizeof(t_list));
		if (new_node == NULL)
		{
			ft_lstdelone(new_node, (*del));
			return (new_list);
		}
		new_node->content = (*f)(lst->content);
		new_node->next = NULL;
		ft_lstadd_back(&new_list, new_node);
		lst = lst->next;
	}
	return (new_list);
}
