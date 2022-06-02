/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_tokenizer_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 14:22:43 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/02 12:14:31 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

static void init_cl_par_ind(int *nb, t_token **tokens, t_token **tmp)
{
	*nb = 0;
	*tmp = *tokens;
}

int cl_par_ind(t_token **tokens, int ind_tok)
{
	t_token *tmp;
	int nb_op_tok;

	init_cl_par_ind(&nb_op_tok, tokens, &tmp);
	move_tok_2_ind(&tmp, ind_tok);
	while (tmp && tmp->token != TOK_EXPANDER_CL)
	{
		if (tmp->token == TOK_EXPANDER_OP && ft_strlen(tmp->value) == 1 && !ft_strncmp("(", tmp->value, 1))
			nb_op_tok++;
		tmp = tmp->next;
	}
	while (tmp && nb_op_tok)
	{
		if (tmp->token == TOK_EXPANDER_CL && ft_strlen(tmp->value) == 1 && !ft_strncmp(")", tmp->value, 1))
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

	end_par = cl_par_ind(tokens, (*tmp)->index);
	*length += (end_par - st_par);
	// printf("end par = %d\n", end_par);
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

int	piped(t_token **tokens, int start, int length)
{
	t_token	*tmp;
	int		i;

	tmp = *tokens;
	i = 0;
	move_tok_2_ind(&tmp, start);
	while (i < length && tmp)
	{
		if (tmp->token == TOK_OPERATOR && ft_strlen(tmp->value) == 1 && !ft_strncmp(tmp->value, "|", 1))
			return (1);
		tmp = tmp->next;
		i++;
	}
	return (0);
}

void divide_by_or_and(t_big_token **b_tokens, t_token **tokens, int start_tok, int length)
{
	t_token	*tmp;
	int		b_length;
	int		i;

	i = 0;
	b_length = 0;
	tmp = *tokens;
	move_tok_2_ind(&tmp, start_tok);
	while (tmp && i < length)
	{
		b_length++;
		if (tmp->token == TOK_EXPANDER_OP)
			divide_by_or_and_1(&tmp, tokens, &b_length);
		else if (tmp->token == TOK_OPERATOR && check_divider_type(tmp->value))
			divide_by_or_and_2(tmp, b_tokens, &start_tok, &b_length);
		tmp = tmp->next;
		i++;
	}
	if (!*b_tokens && !piped(tokens, start_tok, length))
		add_b_tok_sib_last(b_tokens, TOK_CLEAN, start_tok, length);
	if (!*b_tokens && piped(tokens, start_tok, length))
		add_b_tok_sib_last(b_tokens, TOK_CLEAN_PIPED, start_tok, length);
	else
		add_b_tok_sib_last(b_tokens, TOK_LAST, start_tok, b_length);
	handle_par(b_tokens, tokens);
}

void parse(t_big_token **b_tokens, t_token **tokens, int start, int length)
{
	t_big_token *tmp_b;
	t_token *tmp;

	// if (rtn)
	// return ;
	tmp = *tokens;
	index_toks(&tmp, start, length);
	divide_by_or_and(b_tokens, &tmp, start, length);
	tmp_b = *b_tokens;
	// print_b_tokens(&tmp_b, &tmp);
	if (!tmp_b)
	{
		printf("je suis nullll!\n");
		return;
	}
	if (tmp_b->type == TOK_CLEAN)
	{
		printf("\nvoici le tmp_b clean: ");
		print_s_tokens(tokens, tmp_b->ind_tok_start, tmp_b->length);
		printf("\n");
		return;
	}
	while (tmp_b)
	{
		if (tmp_b->par == 1)
		{
			// printf("je suis rentre dans la paranthese");
			printf("\n%d %d\n", tmp_b->ind_tok_start, tmp_b->length);
			// exit (0);
			printf("je me ballade\n");
			parse(&(tmp_b->child), tokens, tmp_b->ind_tok_start, tmp_b->length);
		}
		else
		{
			// printf("\n\ntmp_b->start: %d\n\n", tmp_b->ind_tok_start);
			divide_by_pipe(&tmp_b, tokens);
			// print_b_tokens(&(tmp_b->child), tokens);
			if (tmp_b->child)
			{
				if (tmp_b->child->type == TOK_CLEAN && !tmp_b->child->par)
				{
					printf("\nvoici le tmp_b clean2: ");
					print_s_tokens(tokens, tmp_b->ind_tok_start, tmp_b->length);
					printf("\n");
					if (!tmp_b->sibling) //ajout d'un if car si pas de child, on partait au lieu d'aller au sibling suivant
						return;
				}
				else if (tmp_b->child->type == TOK_CLEAN && tmp_b->child->par)
					parse(&(tmp_b->child), tokens, tmp_b->ind_tok_start, tmp_b->length);
				// else if (tmp_b->child->type != TOK_CLEAN && tmp_b->child->par)
				// 	parse(&(tmp_b->child), tokens, tmp_b->ind_tok_start, tmp_b->length);
				else if (tmp_b->child && tmp_b->child->type != TOK_CLEAN)
				{
					t_big_token *child;
					child = tmp_b->child;
					while (child)
					{
						if (child->par)
						{
					//		printf("c'est non\n");
							parse(&(child->child), tokens, child->ind_tok_start, child->length);
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
