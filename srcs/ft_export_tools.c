/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 20:17:19 by nflan             #+#    #+#             */
/*   Updated: 2022/07/25 19:24:31 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_export_concat(t_env *env, char *line, int i)
{
	env->value = ft_strjoin_free(env->value, line + i + 1, 1);
	if (!env->value)
		return (1);
	return (0);
}

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
