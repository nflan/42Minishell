/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_O.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 20:08:35 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/25 23:25:57 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern int	g_sc;

int	main_agent_o(t_info *info)
{
	int	err;
	int	btok_info[2];

	err = 0;
	if (detect_tokens(info))
		return (1);
	if (fill_tok_value(&info->tokens, info->rdline))
		return (1);
	index_toks(&info->tokens);
	if (syntax_err_handler(&info->tokens))
		return (ft_putstr_error("minishell: syntax error\n"), 2);
	btok_info[0] = 0;
	btok_info[1] = len_ll_list(info->tokens);
	err = parse(&info->parse, info, btok_info);
	return (err);
}
