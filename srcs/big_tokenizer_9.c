/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_tokenizer_9.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 14:45:15 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/25 23:48:35 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_piped(t_big_token **tmp_b, t_info *info)
{
	int		i;
	t_token	*tmp_s;

	i = 0;
	tmp_s = info->tokens;
	(*tmp_b)->cmd_args_num = 1;
	(*tmp_b)->cmd_args = ft_calloc(2, sizeof(char *));
	if (!(*tmp_b)->cmd_args)
		return (ft_putstr_error("Malloc error\n"));
	while (i < (*tmp_b)->length)
	{
		move_tok_2_ind(&tmp_s, (*tmp_b)->ind_tok_start + i);
		((*tmp_b)->cmd_args)[0] = ft_strjoin_free(((*tmp_b)->cmd_args)[0],
				tmp_s->value, 1);
		if (!(*tmp_b)->cmd_args[0])
			return (ft_putstr_error("Malloc error\n"));
		i++;
	}
	return (0);
}
