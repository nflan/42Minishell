/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_O.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 20:08:35 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/22 01:32:56 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main_agent_o(t_info *info)
{
	int	btok_info[2];

	if (detect_tokens(info))
		return (1);
	if (fill_tok_value(&info->tokens, info->rdline))
		return (1);
	index_toks(&info->tokens);
	if (syntax_err_handler(&info->tokens))
	{
//		printf("Error number: %d\n", syntax_err_handler(&info->tokens));
		return (ft_putstr_error("Syntax error\n"));
	}
	btok_info[0] = 0;
	btok_info[1] = len_ll_list(info->tokens);
	if (parse(&info->parse, info, btok_info))
		return (ft_putstr_error("in main_agent_O\nParse error\n"));
	return (0);
}
