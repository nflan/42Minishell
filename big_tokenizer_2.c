/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_tokenizer_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 14:22:43 by omoudni           #+#    #+#             */
/*   Updated: 2022/05/31 18:13:50 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

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
		if (tmp->index == start)
			break;
		tmp = tmp->next;
	}
	while (tmp && i < length)
	{
		b_length++;
		if (tmp->token == TOK_EXPANDER_OP)
			divide_by_or_and_1(&tmp, tokens, &b_length);
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
//	print_b_tokens(*b_tokens, tmp);
	if(!tmp_b)	
	{
		printf("je suis nullll!\n");
		return ;
	}
	if (tmp_b && !tmp_b->par && tmp_b->type == TOK_CLEAN)
	{
		//	printf("je suis clean\n");
		return ;
	}
	print_b_tokens(tmp_b, tmp);
	while (tmp_b)
	{
		printf("je suis laaa!!\n");
		printf("%d\n", tmp_b->par);
		if (tmp_b->par == 1)
		{
			printf("je suis rentre dans la paranthese");
			parse(&(tmp_b->child), tokens, tmp_b->ind_tok_start, tmp_b->length, 0);
		}
		else
		{
			divide_by_pipe(&tmp_b, tokens);
			if (tmp_b->child)
			{
				handle_par(&tmp_b->child, tokens);
				if (tmp_b->child->type == TOK_CLEAN && tmp_b->child->par)
					parse(&(tmp_b->child), tokens, tmp_b->ind_tok_start, tmp_b->length, 0);
				if (tmp_b->child && tmp_b->child->type != TOK_CLEAN)
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
