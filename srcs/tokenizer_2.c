/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 13:47:48 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/23 19:51:12 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_tok_type(t_tok_type type, char c, t_info *i, int test)
{
	if (test == 1)
	{
		if (type != TOK_EXPANDER_OP && type != TOK_EXPANDER_CL
			&& type != TOK_S_QUOTER && type != TOK_D_QUOTER
			&& (get_real_tok_type(c, &(i->tokens), i->tok_type_tab) == type))
			return (1);
	}
	return (0);
}

int	checko(t_tok_type tok_type)
{
	if (tok_type == TOK_OPERATOR)
		return (1);
	return (0);
}
// int detect_tokens(t_token **tok_list, char *str)
// {
// 	int i;
// 	unsigned int tok_type;
// 	int length;
// 	int start;

// 	i = 0;
// 	if (!str)
// 		return (ft_putstr_error("Command error in detect_tokens "));
// 	while (str[i])
// 	{
// 		printf("i: %d\n", i);
// 		length = 1;
// 		start = i;
// 		tok_type = get_real_tok_type(str[i], tok_list);
// 		i++;
// 		while (str[i] && check_tok_type(tok_type) && (get_real_tok_type(str[i], tok_list) == tok_type))
// 		{
// 			printf("I'm filling the bulk with this i: %d\n", i);
// 			if (tok_type == TOK_OPERATOR && ft_strncmp(&(str[i]), &(str[i - 1]), 1))
// 				break ;
// 			length++;
// 			i++;
// 		}
// 		if (tok_type == TOK_WORD_NULL_S || tok_type == TOK_WORD_NULL_D)
// 		{
// 			printf("I'm here\n");
// 			if (tok_type == TOK_WORD_NULL_S)
// 			{
// 			if (add_tok_last(tok_list, TOK_WORD_S_QUOTED, 0, start))
// 				return (ft_putstr_error("detect_tokens "));
// 			if (add_tok_last(tok_list, TOK_S_QUOTER, length, start))
// 				return (ft_putstr_error("detect_tokens "));
// 			}
// 			else
// 			{
// 			if (add_tok_last(tok_list, TOK_WORD_D_QUOTED, 0, start))
// 				return (ft_putstr_error("detect_tokens "));
// 			if (add_tok_last(tok_list, TOK_D_QUOTER, length, start))
// 				return (ft_putstr_error("detect_tokens "));
// 			}
// 		}
// 		else
// 		{
// 		if (add_tok_last(tok_list, tok_type, length, start))
// 			return (ft_putstr_error("detect_tokens "));
// 		}
// 	}
// 	return (0);
// }

int	detect_tokens(t_info *in)
{
	unsigned int	t;
	int				i;
	int				length;
	int				start;

	i = 0;
	if (!in->rdline)
		return (ft_putstr_error("Command error in detect_tokens\n"));
	while (in->rdline[i])
	{
		length = 1;
		start = i;
		t = get_real_tok_type(in->rdline[i], &(in->tokens), in->tok_type_tab);
		// printf("t: %d\n", t);
		i++;
		while (in->rdline[i] && check_tok_type(t, in->rdline[i], in, 1))
		{
			if (checko(t) && strncmp(&(in->rdline[i]), &(in->rdline[i - 1]), 1))
				break ;
			length++;
			i++;
		}
		// if (t == 2)
		// 	printf("start: %d, length: %d\n", start, length);
		if (add_tok(&(in->tokens), start, length, t))
			return (ft_putstr_error("Error while detecting tokens"));
	}
//	print_s_tokens(&in->tokens, 0, len_ll_list(in->tokens));
	return (0);
}

void	index_toks(t_token **tokens)
{
	t_token	*tmp;
	int		i;

	i = 0;
	tmp = *tokens;
	if (!tmp)
		return ;
	while (tmp)
	{
		tmp->index = i;
		tmp = tmp->next;
		i++;
	}
}
