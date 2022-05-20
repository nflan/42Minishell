/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 12:02:17 by nflan             #+#    #+#             */
/*   Updated: 2022/05/20 12:22:27 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_env	*ft_unset(t_env *env, char *line)
{
	t_env	*tmp;
	t_env	*ptr;

	tmp = env;
	ptr = NULL;
	if (!env || !line)
		return (NULL);
	while (tmp->next && ft_strncmp(tmp->next->name, line, ft_strlen(line + 1)))
		tmp = tmp->next;
	if (!tmp->next && ft_strncmp(tmp->name, line, ft_strlen(line + 1)))
		return (NULL);
	ptr = tmp->next;
	tmp->next = ptr->next;
	ptr->next = NULL;
	ft_free_env(ptr);
	return (env);
}
