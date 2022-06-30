/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_tokenizer_6.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 23:40:41 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/29 11:44:58 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_pipe(t_token *tmp_s)
{
	if (tmp_s && tmp_s->token == TOK_OPERATOR && ft_strlen(tmp_s->value) == 1
		&& !ft_strncmp(tmp_s->value, "|", 1))
		return (1);
	return (0);
}
