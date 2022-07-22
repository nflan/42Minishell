/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tok_type_tab.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 20:22:37 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/22 22:03:29 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	init_tok_type_tab_bis(t_tok_type tok_type_tab[128], int i)
{
	while (++i < 124)
		tok_type_tab[i] = TOK_WORD;
	tok_type_tab[i] = TOK_OPERATOR;
	while (++i < 128)
		tok_type_tab[i] = TOK_WORD;
}

void	init_tok_type_tab(t_tok_type (*tok_type_tab)[128])
{
	int	i;

	i = -1;
	while (++i < 9)
		(*tok_type_tab)[i] = TOK_WORD;
	(*tok_type_tab)[i] = TOK_SEP;
	while (++i < 32)
		(*tok_type_tab)[i] = TOK_WORD;
	(*tok_type_tab)[i] = TOK_SEP;
	(*tok_type_tab)[++i] = TOK_WORD;
	(*tok_type_tab)[++i] = TOK_D_QUOTER;
	while (++i < 38)
		(*tok_type_tab)[i] = TOK_WORD;
	(*tok_type_tab)[i] = TOK_OPERATOR;
	(*tok_type_tab)[++i] = TOK_S_QUOTER;
	(*tok_type_tab)[++i] = TOK_EXPANDER_OP;
	(*tok_type_tab)[++i] = TOK_EXPANDER_CL;
	while (++i < 60)
		(*tok_type_tab)[i] = TOK_WORD;
	(*tok_type_tab)[i] = TOK_REDIRECTOR_LEFT;
	(*tok_type_tab)[++i] = TOK_WORD;
	(*tok_type_tab)[++i] = TOK_REDIRECTOR_RIGHT;
	init_tok_type_tab_bis(*tok_type_tab, i);
}
