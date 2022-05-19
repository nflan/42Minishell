/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 16:39:38 by nflan             #+#    #+#             */
/*   Updated: 2022/05/19 16:42:01 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_env(t_env *env)
{
	t_env	*print;

	print = env;
	if (print)
	{
		while (print)
		{
			printf("%s=%s\n", print->name, print->value);
			print = print->next;
		}
	}
}
