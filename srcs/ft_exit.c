/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 11:28:54 by nflan             #+#    #+#             */
/*   Updated: 2022/05/20 12:31:33 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_exit(t_info *info, char *value, char **tofree)
{
	int	ret;

	ret = ft_atoi(value);
	ft_free_split(tofree);
	printf("exit\n");
	if (info->env)
		ft_free_env(info->env);
	if (info)
		ft_free_all(info);
	exit(ret);
}
