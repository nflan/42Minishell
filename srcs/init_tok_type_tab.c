/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tok_type_tab.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 20:22:37 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/27 21:04:43 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	init_tok_type_tab_bis(t_tok_type tok_type_tab[128], int i)
{
	while (++i < 124)
		tok_type_tab[i] = TOK_WORD;
	tok_type_tab[i] = TOK_OPERATOR; // i = 124
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
	(*tok_type_tab)[i] = TOK_SEP; // i = 32
	(*tok_type_tab)[++i] = TOK_WORD; // i = 33
	(*tok_type_tab)[++i] = TOK_D_QUOTER; //i = 34
	while (++i < 38)
		(*tok_type_tab)[i] = TOK_WORD;
	(*tok_type_tab)[i] = TOK_OPERATOR; // i = 38
	(*tok_type_tab)[++i] = TOK_S_QUOTER; //i = 39
	(*tok_type_tab)[++i] = TOK_EXPANDER_OP; //i = 40
	(*tok_type_tab)[++i] = TOK_EXPANDER_CL; //i = 41
	while (++i < 60)
		(*tok_type_tab)[i] = TOK_WORD;
	(*tok_type_tab)[i] = TOK_REDIRECTOR_LEFT; // i = 38
	(*tok_type_tab)[++i] = TOK_WORD; // i = 33
	(*tok_type_tab)[++i] = TOK_REDIRECTOR_RIGHT; // i = 38
	init_tok_type_tab_bis(*tok_type_tab, i);
}
