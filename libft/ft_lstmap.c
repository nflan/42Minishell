/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 17:01:16 by nflan             #+#    #+#             */
/*   Updated: 2021/11/29 17:46:50 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*tmp;
	t_list	*tmp2;

	tmp = NULL;
	if (lst)
	{
		while (lst)
		{
			tmp2 = ft_lstnew((*f)(lst->content));
			if (!tmp2)
			{
				ft_lstclear(&tmp, del);
				return (NULL);
			}
			ft_lstadd_back(&tmp, tmp2);
			lst = lst->next;
		}
	}
	return (tmp);
}
