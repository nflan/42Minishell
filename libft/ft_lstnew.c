/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 11:35:19 by nflan             #+#    #+#             */
/*   Updated: 2022/01/18 15:40:52 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_pile	*ft_lstnew(int num)
{
	t_pile	*list;

	list = NULL;
	list = malloc(sizeof(t_pile));
	if (list)
	{
		list->num = num;
		list->next = NULL;
	}
	return (list);
}
