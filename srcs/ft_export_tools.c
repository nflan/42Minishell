/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 20:17:19 by nflan             #+#    #+#             */
/*   Updated: 2022/07/24 20:17:20 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_export_solo(t_env *env, t_big_token *b_tok)
{
	if (env)
	{
		while (env)
		{
			ft_putstr_fd("declare -x ", b_tok->fdout);
			ft_putstr_fd(env->name, b_tok->fdout);
			if (env->value)
			{
				ft_putstr_fd("=\"", b_tok->fdout);
				ft_putstr_fd(env->value, b_tok->fdout);
				ft_putstr_fd("\"", b_tok->fdout);
			}
			ft_putstr_fd("\n", b_tok->fdout);
			env = env->next;
		}
	}
}
