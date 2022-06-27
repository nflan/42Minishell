/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 11:11:34 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/26 19:33:12 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void give_parent(t_big_token **b_child, t_big_token **parent)
{
	(*b_child)->parent = *parent;
}

void sub_parse_1(t_big_token **tmp_b, t_info *info, int b_start, int b_length)
{
	int	btok_info[2];

	btok_info[0] = b_start;
	btok_info[1] = b_length;
	parse(&((*tmp_b)->child), info, btok_info);
	if ((*tmp_b)->child)
		give_parent(&((*tmp_b)->child), tmp_b);
}

void sub_parse_2(t_big_token **tmp, t_big_token **tmp_b, t_info *info)
{
	t_big_token *b_child;

	b_child = *tmp;
	give_parent(&b_child, tmp_b);
	while (b_child)
	{
		if ((b_child)->par)
			sub_parse_1(&b_child, info, (b_child)->ind_tok_start, (b_child)->length);
		(b_child) = (b_child)->sibling;
	}
}

int access_file(char *file)
{
	if (access(file, F_OK) == 0)
		return (1);
	else
		return (0);
}

int no_red(t_big_token **tmp_b, t_token **tokens)
{
	(void)tmp_b;
	(void)tokens;
	return (0);
}

/*void	extract_fds(t_big_token **tmp_b, t_token **tokens)
{
	int		i;
	int		start;
	int		len;
	t_token	*tmp_s;
	char	*file;

	start = (*tmp_b)->ind_tok_start;
	len = (*tmp_b)->length;
	i = 0;
	tmp_s = *tokens;
	while (i < len)
	{
		move_tok_2_ind(&tmp_s, start + i);
		if (tmp_s->token == TOK_REDIRECTOR_RIGHT)
		{
			(*tmp_b)->red_out = 1;
			if (ft_strlen(tmp_s->value) == 1)
			{
				if (tmp_s->next->token != TOK_SEP)
					(*tmp_b)->fdout = open(tmp_s->next->value, O_RDWR | O_TRUNC | O_CREAT);
				else if (tmp_s->next->token == TOK_SEP && tmp_s->next->next)
					(*tmp_b)->fdout = open(tmp_s->next->next->value, O_RDWR | O_TRUNC | O_CREAT);
			}
			else if (ft_strlen(tmp_s->value) == 2)
			{
				if (tmp_s->next->token != TOK_SEP)
					(*tmp_b)->fdout = open(tmp_s->next->value, O_RDWR | O_APPEND | O_CREAT);
				else if (tmp_s->next->token == TOK_SEP && tmp_s->next->next)
					(*tmp_b)->fdout = open(tmp_s->next->next->value, O_RDWR | O_APPEND | O_CREAT);
			}
		}
		else if (tmp_s->token == TOK_REDIRECTOR_LEFT)
		{
			(*tmp_b)->red_in = 1;
			if (ft_strlen(tmp_s->value) == 1)
			{
				if (tmp_s->next->token != TOK_SEP)
					file = tmp_s->next->value;
				else if (tmp_s->next->token == TOK_SEP && tmp_s->next->next)
					file = tmp_s->next->next->value;
				if (access_file(file))
					(*tmp_b)->fdin = open(file, O_RDONLY);
				else
					(*tmp_b)->fdin = -1;
			}
			}
		i++;
	}
	if (i == len)
	{
		if ((*tmp_b)->fdin == -1000)
			(*tmp_b)->fdin = 0;
		if ((*tmp_b)->fdout == -1000)
			(*tmp_b)->fdout = 1;
	}
}*/

int parse(t_big_token **b_tokens, t_info *info, int btok_info[2])
{
	t_big_token *tmp_b;
//	t_big_token *b_child;

	if (divide_by_or_and(b_tokens, info, btok_info))
		return (ft_putstr_error("in parse "));
	tmp_b = *b_tokens;
	if (!tmp_b || (!tmp_b->par && tmp_b->type == TOK_CLEAN))
	{
		// handle_redirections(&tmp_b, tokens);
		return (0);
	}
	while (tmp_b)
	{
		if (tmp_b->par)
			sub_parse_1(&tmp_b, info, tmp_b->ind_tok_start, tmp_b->length);
		else if (piped(info, tmp_b->ind_tok_start, tmp_b->length))
		{
			if (divide_by_pipe(&tmp_b, info))
				return (ft_putstr_error("in parse "));
			if (tmp_b->child)
				sub_parse_2(&tmp_b->child, &tmp_b, info);
		}
		tmp_b = tmp_b->sibling;
	}
	return (0);
}
