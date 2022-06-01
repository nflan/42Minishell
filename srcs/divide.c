/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   divide.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 17:57:08 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/01 15:26:53 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"

static void handle_par_1(t_token **tmp_s, t_big_token *tmp_b, int *adv_steps, int *to_reduce)
{
	move_tok_2_ind(tmp_s, tmp_b->ind_tok_start + 1);
	(*adv_steps)++;
	(*to_reduce)++;
}

static void handle_par_2(t_token **tmp_s, t_big_token *tmp_b, int *to_reduce, t_token *tokens)
{
	*tmp_s = tokens;
	move_tok_2_ind(tmp_s, tmp_b->ind_tok_start + tmp_b->length - 2);
	(*to_reduce)++;
}

static void handle_par_3(t_big_token **tmp_b, int to_reduce, int adv_steps)
{
	(*tmp_b)->par = 1;
	(*tmp_b)->length -= (2 + to_reduce);
	(*tmp_b)->ind_tok_start += (1 + adv_steps);
}

static void init_params(int *adv_steps, int *to_reduce)
{
	(*adv_steps) = 0;
	(*to_reduce) = 0;
}

void handle_par(t_big_token **b_tokens, t_token **tokens)
{
	t_big_token *tmp_b;
	t_token		*tmp_s;
	int			params[2];

	tmp_b = *b_tokens;
	tmp_s = *tokens;
	while (tmp_b)
	{
	init_params(&(params[0]), &(params[1]));
		tmp_s = *tokens;
		printf("tok_start (handle part): %d\n", tmp_b->ind_tok_start);
		move_tok_2_ind(&tmp_s, tmp_b->ind_tok_start);
		if (tmp_s->token == TOK_SEP && tmp_b->length > 2)
			handle_par_1(&tmp_s, tmp_b, &(params[0]), &(params[1]));
		if (tmp_s->token == TOK_EXPANDER_OP && tmp_b->length > 2)
		{
			move_tok_2_ind(&tmp_s, tmp_b->ind_tok_start + tmp_b->length - 1);
			if (tmp_s->token == TOK_SEP)
				handle_par_2(&tmp_s, tmp_b, &(params[1]), *tokens);
			if (tmp_s->token == TOK_EXPANDER_CL)
			{
				printf("token expander\n");
				handle_par_3(&tmp_b, params[1], params[0]);
			}
		printf("\n%d %d\n", params[0], params[1]);
		}
		else
			tmp_b->par = 0;
		tmp_b = tmp_b->sibling;
	}
}

t_big_token *ft_create_btoken(t_big_tok_type type, int ind_tok_start, int length)
{
	t_big_token *tok;

	tok = (t_big_token *)malloc(sizeof(t_big_token));
	if (!tok)
		return (NULL);
	tok->type = type;
	tok->ind_tok_start = ind_tok_start;
	tok->length = length;
	// tok->more_info =
	tok->child = NULL;
	tok->sibling = NULL;
	return (tok);
}

void add_b_tok_sib_last(t_big_token **b_tok_list, t_big_tok_type type, int start, int length)
{
	t_big_token *tmp;
	t_big_token *bef_last;
	int rank_in_list;

	if (!*b_tok_list)
	{
		*b_tok_list = ft_create_btoken(type, start, length);
		rank_in_list = 0;
	}
	else
	{
		tmp = *b_tok_list;
		while (tmp->sibling)
			tmp = tmp->sibling;
		bef_last = tmp;
		bef_last->sibling = ft_create_btoken(type, start, length);
		rank_in_list = 1;
	}
}

int check_divider_type(char *tmp_value)
{
	if (!ft_strncmp(tmp_value, "&&", 2))
		return (1);
	if (!ft_strncmp(tmp_value, "||", 2))
		return (2);
	return (0);
}

int is_cl_2_op(char *value_tok_op, char *value_tok_cl)
{
	if (!ft_strncmp(value_tok_op, "(", 1) && !ft_strncmp(value_tok_cl, ")", 1))
		return (1);
	if (!ft_strncmp(value_tok_op, "{", 1) && !ft_strncmp(value_tok_cl, "}", 1))
		return (2);
	return (0);
}

void move_tok_2_ind(t_token **tokens, int ind)
{
	while (*tokens)
	{
		if ((*tokens)->index == ind)
			return;
		*tokens = (*tokens)->next;
	}
}


static void init_cl_par_ind(int *nb, t_token *tokens, t_token **tmp)
{
	*nb = 0;
	*tmp = tokens;
}

int cl_par_ind(t_token **tokens, t_tok_type tok, int ind_tok, char *value)
{
	t_token *tmp;
	int nb_op_tok;

	init_cl_par_ind(&nb_op_tok, *tokens, &tmp);
	while (tmp)
	{
		if (tmp->index == ind_tok)
			break;
		tmp = tmp->next;
	}
	while (tmp && tmp->token != TOK_EXPANDER_CL)
	{
		if (tmp->token == TOK_EXPANDER_OP && !ft_strncmp(value, tmp->value, ft_strlen(value)))
			nb_op_tok++;
		tmp = tmp->next;
	}
	while (tmp && nb_op_tok)
	{
		if (tmp->token == TOK_EXPANDER_CL && is_cl_2_op(value, tmp->value))
			nb_op_tok--;
		if (!nb_op_tok)
			return (tmp->index);
		tmp = tmp->next;
	}
	return (-1);
}

static void divide_by_or_and_1(t_token **tmp, t_token **tokens, int *length)
{
	int st_par;
	int end_par;

	st_par = (*tmp)->index;

	printf("op par = %d\n", (*tmp)->index);
	end_par = cl_par_ind(tokens, (*tmp)->token, (*tmp)->index, (*tmp)->value);
	*length += (end_par - st_par);
	printf("end par = %d\n", end_par);
	move_tok_2_ind(tmp, end_par);
}

static void divide_by_or_and_2(t_token *tmp, t_big_token **b_tokens, int *start, int *length)
{
	if (!ft_strncmp(tmp->value, "&&", 2))
		add_b_tok_sib_last(b_tokens, TOK_LEFT_AND, (*start), (*length) - 1);
	else if (!ft_strncmp(tmp->value, "||", 2))
		add_b_tok_sib_last(b_tokens, TOK_LEFT_OR, (*start), (*length) - 1);
	(*start) = tmp->index + 1;
	(*length) = 0;
}

void divide_by_or_and(t_big_token **b_tokens, t_token **tokens, int start_tok, int length)
{
	printf("\nstart tok: %d, length: %d\n" , start_tok, length);
	t_token *tmp;
	int b_length;
	int start;
	int	i;

	i = 0;
	start = 0;
	b_length = 0;
	tmp = *tokens;
	while (tmp)
	{
		if (tmp->index == start_tok)
			break;
		tmp = tmp->next;
	}
	while (tmp && i < length)
	{
		b_length++;
		if (tmp->token == TOK_EXPANDER_OP)
		{
			printf("\n\nthis is the source of the prolem: %d the value is %s\n\n", tmp->index, tmp->value);
			divide_by_or_and_1(&tmp, tokens, &b_length);
		}
		else if (tmp->token == TOK_OPERATOR && check_divider_type(tmp->value))
			divide_by_or_and_2(tmp, b_tokens, &start, &b_length);
//		if (tmp && tmp->index < length)
		tmp = tmp->next;
		i++;
	//		break;
	}
	if (!*b_tokens)
		add_b_tok_sib_last(b_tokens, TOK_CLEAN, 0, len_ll_list(*tokens));
	else
		add_b_tok_sib_last(b_tokens, TOK_LAST, start, b_length);
	handle_par(b_tokens, tokens);
}

void parse(t_big_token **b_tokens, t_token **tokens, int start, int length, int rtn)
{
	t_big_token *tmp_b;
	t_token *tmp;

	if (rtn)
		return ;
	tmp = *tokens;
	index_toks(&tmp, start, length);
	divide_by_or_and(b_tokens, &tmp, start, length);
	tmp_b = *b_tokens;
	if(!tmp_b)
	{
		printf("je suis nullll!\n");
		return ;
	}
	while (tmp_b)
	{
		if (tmp_b->par == 1)
		{
			printf("je suis rentre dans la paranthese");
			printf("\n%d %d\n", tmp_b->ind_tok_start, tmp_b->length);
			// exit (0);
			parse(&(tmp_b->child), tokens, tmp_b->ind_tok_start, tmp_b->length, 0);
		}
		else
		{
			printf("\n\ntmp_b->start: %d\n\n", tmp_b->ind_tok_start);
			exit (0);
			divide_by_pipe(&tmp_b, tokens);
			handle_par(&(tmp_b->child), tokens);
			if (tmp_b->child)
			{
				if (tmp_b->child->type == TOK_CLEAN && !tmp_b->child->par)
					return ;
				if (tmp_b->child->type == TOK_CLEAN && tmp_b->child->par)
					parse(&(tmp_b->child), tokens, tmp_b->ind_tok_start, tmp_b->length, 0);
				else if (tmp_b->child && tmp_b->child->type != TOK_CLEAN)
				{
					t_big_token *child;
					child = tmp_b->child;
					while (child)
					{
						if (child->par)
						{
							printf("c'est non\n");
							parse(&(child->child), tokens, child->ind_tok_start, child->length, 0);
						}
						child = child->sibling;
					}
				}
				//		printf("je suis clean et pipe\n");
				//		parse(&(tmp_b->child->child), tokens, tmp_b->child->ind_tok_start, tmp_b->child->length, 0);
			}
		}
		tmp_b = tmp_b->sibling;
	}
}

void divide_by_pipe(t_big_token **b_tokens, t_token **tokens)
{
	t_token *tmp_s;
	t_big_token *tmp_b;
	int length_piped;
	int start;
	int st_par;
	int end_par;
	int i;
	int j;

	tmp_s = *tokens;
	tmp_b = *b_tokens;
	length_piped = 0;

//	while (tmp_b)
//	{
	start = tmp_b->ind_tok_start;
		i = 0;
		j = tmp_b->ind_tok_start;
		length_piped = 0;
		printf("\nlwngthhhhhhhhhh: %d\n", tmp_b->length);
		while (i < tmp_b->length)
		{
			length_piped++;
			move_tok_2_ind(&tmp_s, j);
			printf("\n\n\nalertttt:  %d\n\n\n", j);
			if (tmp_s && tmp_s->token == TOK_EXPANDER_OP)
			{
				st_par = tmp_s->index;
				end_par = cl_par_ind(&tmp_s, tmp_s->token, tmp_s->index, tmp_s->value);
				length_piped += (end_par - st_par);
			}
			else if (tmp_s && tmp_s->token == TOK_OPERATOR && ft_strlen(tmp_s->value) == 1 && !ft_strncmp(tmp_s->value, "|", 1))
			{
			printf("I added a child!\n");
				add_b_tok_sib_last(&((tmp_b)->child), TOK_LEFT_PIPE, start, length_piped - 1);
				start = tmp_s->index + 1;
				length_piped = 0;
			}
			j++;
			i++;
		}
		if (!((tmp_b)->child))
		{
			printf("I added a weird child\n");
			add_b_tok_sib_last(&((tmp_b)->child), TOK_CLEAN, start, length_piped);
		}
		else
		{
			printf("I added a child!\n");
			add_b_tok_sib_last(&((tmp_b)->child), TOK_PIPE_LAST, start, length_piped);
		}
//		tmp_b = tmp_b->sibling;
//	}
}
