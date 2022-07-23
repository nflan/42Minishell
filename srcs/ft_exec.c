/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 13:06:50 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/23 12:45:54 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_expanding(t_info *info, t_big_token *b)
{
	if (!b->par)
	{
		if (ft_expand_args(b, info))
			return (1);
		if (ft_add_wildcards(b))
			return (ft_putstr_error("Wildcards error\n"));
		if (ft_noquote_args(b))
			return (1);
	}
	return (0);
}

int	ft_exec_simple(t_info *info, t_big_token *b_tokens)
{
	if (ft_wash_btoken(info, b_tokens))
		return (2147483647);
	if (b_tokens->sc == -1)
	{
		if (ft_open_fd(b_tokens, info))
			return (1);
		if (ft_expanding(info, b_tokens))
			return (1);
		ft_launch_cmd(info, b_tokens);
		b_tokens->sc = info->status;
		ft_close_fd(b_tokens);
	}
	return (0);
}

int	exec_the_bulk(t_info *info, int sib_child, t_big_token *b)
{
	info->nb_cmd = 0;
	if (sib_child >= 1 && sib_child <= 3)
		ft_exec_simple(info, b);
	else if (sib_child == 4)
		ft_init_pipex(info, b);
	if (b && b->parent)
		give_parent_sc(&(b), &(b->parent));
	return (0);
}
