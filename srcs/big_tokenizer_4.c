/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_tokenizer_4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 14:45:15 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/15 18:46:34 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

int check_if_piped(t_big_token **tmp_b, int ind, t_token **tokens, int len)
{
	t_token *tmp;
	int i;

	(void) tmp_b;
	tmp = *tokens;
	i = 0;
	move_tok_2_ind(&tmp, ind);
	while (tmp && i < len)
	{
		if (tmp->token == TOK_OPERATOR && ft_strlen(tmp->value) == 1 && !ft_strncmp("|", tmp->value, 1))
			return (1);
		tmp = tmp->next;
		i++;
	}
	return (0);
}

int *count_red_inout(t_big_token **tmp_b, int ind, t_token **tokens, int len)
{
	t_token *tmp;
	int i;

	tmp = *tokens;
	i = 0;
	move_tok_2_ind(&tmp, ind);
	while (tmp && i < len)
	{
		if (tmp->token == TOK_REDIRECTOR_LEFT && ft_strlen(tmp->value) == 1)
			(((*tmp_b)->rd_inouthd)[0])++;
		else if (tmp->token == TOK_REDIRECTOR_LEFT && ft_strlen(tmp->value) == 2)
			(((*tmp_b)->rd_inouthd)[2])++;
		else if (tmp->token == TOK_REDIRECTOR_RIGHT)
			(((*tmp_b)->rd_inouthd)[1])++;
		tmp = tmp->next;
		i++;
	}
	if (((*tmp_b)->rd_inouthd)[0])
	{
		(*tmp_b)->red_in = ft_calloc(sizeof(int), ((*tmp_b)->rd_inouthd)[0]);
		(*tmp_b)->infile = ft_calloc(sizeof(char *), ((*tmp_b)->rd_inouthd)[0]);
		(*tmp_b)->err_in = ft_calloc(sizeof(int), ((*tmp_b)->rd_inouthd)[0]);
		(*tmp_b)->fdin = ft_calloc(sizeof(int), ((*tmp_b)->rd_inouthd)[0]);
	}
	if (((*tmp_b)->rd_inouthd)[1])
	{
		(*tmp_b)->red_out = ft_calloc(sizeof(int), ((*tmp_b)->rd_inouthd)[1]);
		(*tmp_b)->outfile = ft_calloc(sizeof(char *), ((*tmp_b)->rd_inouthd)[1]);
		(*tmp_b)->err_out = ft_calloc(sizeof(int), ((*tmp_b)->rd_inouthd)[1]);
		(*tmp_b)->fdout = ft_calloc(sizeof(int), ((*tmp_b)->rd_inouthd)[1]);
	}
	if (((*tmp_b)->rd_inouthd)[2])
		(*tmp_b)->delimitator = ft_calloc(sizeof(char *), ((*tmp_b)->rd_inouthd)[2]);
	return (0);
}

void	count_cmd_args(t_big_token **tmp_b, int ind, t_token **tokens, int len)
{
	t_token *tmp;
	int i;
	int j;
	int count;
	int red;

	tmp = *tokens;
	i = 0;
	j = 0;
	count = 0;
	red = 0;
	move_tok_2_ind(&tmp, ind);
	while (tmp && i < len)
	{
		if (tmp->token == TOK_REDIRECTOR_LEFT || tmp->token == TOK_REDIRECTOR_RIGHT)
		{
			red = 1;
			j++;
		}
		if (tmp->token == TOK_WORD)
		{
			if (!red)
				count++;
			else if (red && j == 3)
			{
				count++;
				j = 0;
				red = 0;
			}
			else
				j++;
		}
		tmp = tmp->next;
		i++;
	}
	if (count)
	{
		(*tmp_b)->cmd_args = ft_calloc((count), sizeof(char *));
		(*tmp_b)->cmd_args_num = count;
	}
}

void rd_inout_type(char *str, t_big_token **tmp_b, int *type_red, int (*inouthd)[3])
{
	if (ft_strlen(str) == 1 && !ft_strncmp(str, "<", 1))
	{
	//	if (!((*tmp_b)->red_in))
	//		printf("Yesss!\n");
		(*tmp_b)->red_in[((*tmp_b)->rd_inouthd)[0] - (*inouthd)[0]] = 1;
		(*type_red) = 1;
	}
	if (ft_strlen(str) == 2 && !ft_strncmp(str, "<<", 2))
	{
		(*type_red) = 2;
	}
	if (ft_strlen(str) == 1 && !ft_strncmp(str, ">", 1))
	{
		(*tmp_b)->red_out[((*tmp_b)->rd_inouthd)[1] - (*inouthd)[1]] = 1;
		(*type_red) = 3;
	}
	if (ft_strlen(str) == 2 && !ft_strncmp(str, ">>", 2))
	{
		(*tmp_b)->red_out[((*tmp_b)->rd_inouthd)[1] - (*inouthd)[1]] = 2;
		(*type_red) = 4;
	}
}

void handle_red_files(t_big_token **tmp_b, char *tmp_value, int (*inouthd)[3], int type_red)
{
	if (type_red == 1)
	{
		(*tmp_b)->infile[((*tmp_b)->rd_inouthd)[0] - (*inouthd)[0]] = ft_strdup(tmp_value);
		((*inouthd)[0])--;
	}
	if (type_red == 2)
	{
		(*tmp_b)->delimitator[((*tmp_b)->rd_inouthd)[2] - (*inouthd)[2]] = ft_strdup(tmp_value);
		((*inouthd)[2])--;
	}
	if (type_red == 3 || type_red == 4)
	{
//		printf("got here to fill the outfile with tmp->value: %s!\n", tmp_value);
//		printf("genius that I thought I was. Here are rd_inouthd[1]: %d and (*inouthd)[1]: %d\n", ((*tmp_b)->rd_inouthd)[1], (*inouthd)[1]);
		(*tmp_b)->outfile[((*tmp_b)->rd_inouthd)[1] - (*inouthd)[1]] = ft_strdup(tmp_value);
		((*inouthd)[1])--;
	}
}

void handle_par_dir(t_token **tmp_s, t_big_token **tmp_b, t_token **tokens, int ind_word)
{
	t_token *tmp;
	int i;
	int j;
	int cl_ind;
//	int to_where_check;
	int type_red;
	int len;
	int inouthd[3];
	int to_reduce;
	int to_start;

	(void)tmp_s;
	(void)ind_word;
	tmp = *tokens;
	i = 0;
	j = 0;
	type_red = 0;
	len = 0;
	to_reduce = 0;
	to_start = 0;
	move_tok_2_ind(&tmp, (*tmp_b)->ind_tok_start);
	if (tmp->token == TOK_SEP)
	{
		move_tok_2_ind(&tmp, (*tmp_b)->ind_tok_start + 1);
		to_reduce++;
		to_start++;
	}
	if (tmp->token == TOK_EXPANDER_OP)
	{
		to_reduce++;
		to_start++;
		cl_ind = cl_par_ind(&tmp, tmp->index);
		if (cl_ind == -1)
		{
		//	printf("There is an error here!\n");
			return;
		}
		else
		{
			to_reduce++;
			move_tok_2_ind(&tmp, cl_ind + 1);
			len = (*tmp_b)->length + (*tmp_b)->ind_tok_start - 1 - cl_ind;
			if (check_if_piped(tmp_b, cl_ind + 1, tokens, len))
			{
	//			printf("I'm piped darling!\n");
				return;
			}
			to_reduce += len;
			count_red_inout(tmp_b, cl_ind + 1, tokens, len);
			inouthd[0] = ((*tmp_b)->rd_inouthd)[0];
			inouthd[1] = ((*tmp_b)->rd_inouthd)[1];
			inouthd[2] = ((*tmp_b)->rd_inouthd)[2];
		}
	}
	type_red = 0;
	while (tmp && j < len)
	{
		if ((tmp->token == TOK_REDIRECTOR_LEFT || tmp->token == TOK_REDIRECTOR_RIGHT) && i % 2)
		{
	//		printf("There is a problem in redirections!\n");
			return;
		}
		if ((tmp->token == TOK_REDIRECTOR_LEFT || tmp->token == TOK_REDIRECTOR_RIGHT) && !(i % 2))
		{
	//		printf("I entered here with : %s\n", tmp->value);
			rd_inout_type(tmp->value, tmp_b, &type_red, &inouthd);
	//		printf("typered: %d\n", type_red);
			i++;
		}
		else if (tmp->token == TOK_WORD && (!(i % 2) || !type_red))
		{
	//		printf("tmp_tok: %s, %d, %d\n", tmp->value, i, type_red);
	//		printf("problem here putain!\n");
			return;
		}
		if (tmp->token == TOK_WORD && (i % 2))
		{
			handle_red_files(tmp_b, tmp->value, &inouthd, type_red);
			i++;
		}
		tmp = tmp->next;
		j++;
	}
	(*tmp_b)->ind_tok_start += to_start;
	(*tmp_b)->length -= to_reduce;
	(*tmp_b)->par = 1;
}

void handle_dir(t_big_token **tmp_b, t_token **tokens)
{
	t_token *tmp;
	int i;
	int j;
//	int to_where_check;
	int type_red;
	int len;
	int inouthd[3];
	int save_word;
	int	cmd_args_num;

//	printf("I'm here houbbiiiii!!!\n");
	tmp = *tokens;
	i = 0;
	j = 0;
	len = len_ll_list(*tokens);
	type_red = 0;
	save_word = 0;
	move_tok_2_ind(&tmp, (*tmp_b)->ind_tok_start);
	count_red_inout(tmp_b, (*tmp_b)->ind_tok_start, tokens, (*tmp_b)->length);
	count_cmd_args(tmp_b, (*tmp_b)->ind_tok_start, tokens, (*tmp_b)->length);
	inouthd[0] = ((*tmp_b)->rd_inouthd)[0];
	inouthd[1] = ((*tmp_b)->rd_inouthd)[1];
	inouthd[2] = ((*tmp_b)->rd_inouthd)[2];
	cmd_args_num = (*tmp_b)->cmd_args_num;
//	printf("choufi hada: %d\n", cmd_args_num);
//	printf("tmpb_len: %d et tmpb_start: %d\n", (*tmp_b)->length, (*tmp_b)->ind_tok_start);
//	printf("------------\n");
//	print_b_tokens(tmp_b, tokens, 1, 1);
//	printf("------------\n");
	type_red = 0;
	while (tmp && j < len)
	{
		if ((tmp->token == TOK_REDIRECTOR_LEFT || tmp->token == TOK_REDIRECTOR_RIGHT) && !(i % 2))
		{
		//	printf("I entered in rd\n");
			rd_inout_type(tmp->value, tmp_b, &type_red, &inouthd);
			i++;
			save_word = 1;
		}
		else if (tmp->token == TOK_WORD && !save_word)
		{
		//	printf("I entered arg\n");
			(*tmp_b)->cmd_args[(*tmp_b)->cmd_args_num - cmd_args_num] = ft_strdup(tmp->value);
			cmd_args_num--;
			save_word = 0;
		}
		else if (tmp->token == TOK_WORD && (i % 2) && save_word)
		{
		//	printf("I entered red files\n");
			handle_red_files(tmp_b, tmp->value, &inouthd, type_red);
			i++;
		}
		tmp = tmp->next;
		j++;
	}
	(*tmp_b)->par = 0;
}

void handle_par(t_big_token **b_tokens, t_token **tokens)
{
	t_big_token *tmp_b;
	t_token *tmp_s;
	int params[2];
	int st_par;
	int end_par;

	tmp_b = *b_tokens;
	tmp_s = *tokens;
	while (tmp_b)
	{
		init_params(&(params[0]), &(params[1]));
		tmp_s = *tokens;
		move_tok_2_ind(&tmp_s, tmp_b->ind_tok_start);
		if (tmp_s->token == TOK_SEP && tmp_b->length > 2)
		{
			handle_par_1(&tmp_s, tmp_b, &(params[0]), &(params[1]));
		}
		if (tmp_s->token == TOK_EXPANDER_OP && tmp_b->length > 2)
		{
			st_par = tmp_s->index;
			move_tok_2_ind(&tmp_s, tmp_b->ind_tok_start + tmp_b->length - 1);
			if (tmp_s->token == TOK_SEP)
				handle_par_2(&tmp_s, tmp_b, &(params[1]), *tokens);
			if (tmp_s->token == TOK_EXPANDER_CL)
			{
				end_par = tmp_s->index;
				if (end_par == cl_par_ind(tokens, st_par))
					handle_par_3(&tmp_b, params[1], params[0]);
			}
			else if (tmp_s->token == TOK_WORD)
			{
	//			printf("I'm here habibiiw!\n");
	//			printf("I have this word: %s\n", tmp_s->value);
				handle_par_dir(&tmp_s, &tmp_b, tokens, st_par);
			}
		}
		else
		{
			handle_dir(&tmp_b, tokens);
			// tmp_b->par = 0;
		}
		tmp_b = tmp_b->sibling;
	}
}

/*void handle_par(t_big_token **b_tokens, t_token **tokens)
{
	t_big_token *tmp_b;
	t_token		*tmp_s;
	int			params[2];
	int			st_par;
	int			end_par;

	tmp_b = *b_tokens;
	tmp_s = *tokens;
	while (tmp_b)
	{
	init_params(&(params[0]), &(params[1]));
		tmp_s = *tokens;
		move_tok_2_ind(&tmp_s, tmp_b->ind_tok_start);
		if (tmp_s->token == TOK_SEP && tmp_b->length > 2)
		{
			handle_par_1(&tmp_s, tmp_b, &(params[0]), &(params[1]));
		}
		if (tmp_s->token == TOK_EXPANDER_OP && tmp_b->length > 2)
		{
			st_par = tmp_s->index;
			move_tok_2_ind(&tmp_s, tmp_b->ind_tok_start + tmp_b->length - 1);
			if (tmp_s->token == TOK_SEP)
				handle_par_2(&tmp_s, tmp_b, &(params[1]), *tokens);
			if (tmp_s->token == TOK_EXPANDER_CL)
			{
				end_par = tmp_s->index;
				if (end_par ==  cl_par_ind(tokens, st_par))
					handle_par_3(&tmp_b, params[1], params[0]);
			}
		}
		else
			tmp_b->par = 0;
		tmp_b = tmp_b->sibling;
	}
}*/
