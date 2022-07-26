/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fd_tools_bis.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 19:23:32 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/24 19:25:21 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	create_del_cond(t_tok_type tok)
{
	if (tok == TOK_WORD || tok == TOK_WORD_S_QUOTED
		|| tok == TOK_WORD_D_QUOTED || tok == TOK_WORD_NULL_S
		|| tok == TOK_WORD_NULL_D || tok == TOK_D_QUOTER
		|| tok == TOK_S_QUOTER || tok == TOK_QUOTER)
		return (1);
	return (0);
}
