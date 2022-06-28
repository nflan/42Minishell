/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_tokenizer_4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 14:45:15 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/28 18:27:21 by omoudni          ###   ########.fr       */
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

static void handle_par_3(t_big_token **tmp_b, int to_reduce, int adv_steps, t_info *info)
{
	int		i;
	t_token	*tmp_s;

	i = 0;
	tmp_s = info->tokens;
	(*tmp_b)->par = 1;
	(*tmp_b)->length -= (2 + to_reduce);
	(*tmp_b)->ind_tok_start += (1 + adv_steps);
	(*tmp_b)->cmd_args_num = 1;
	(*tmp_b)->cmd_args = ft_calloc(2, sizeof(char *));
	while (i < (*tmp_b)->length)
	{
		move_tok_2_ind(&tmp_s, (*tmp_b)->ind_tok_start + i);
		((*tmp_b)->cmd_args)[0] = ft_strjoin_free(((*tmp_b)->cmd_args)[0], tmp_s->value, 1);
		i++;
	}
}

int	handle_piped(t_big_token **tmp_b, t_info *info)
{
	int		i;
	t_token	*tmp_s;

	i = 0;
	tmp_s = info->tokens;
	(*tmp_b)->cmd_args_num = 1;
	(*tmp_b)->cmd_args = ft_calloc(2, sizeof(char *));
	if (!(*tmp_b)->cmd_args)
		return (ft_putstr_error("Malloc error in ft_calloc in handle piped "));
	while (i < (*tmp_b)->length)
	{
		move_tok_2_ind(&tmp_s, (*tmp_b)->ind_tok_start + i);
		((*tmp_b)->cmd_args)[0] = ft_strjoin_free(((*tmp_b)->cmd_args)[0], tmp_s->value, 1);
		if (!(*tmp_b)->cmd_args[0])
			return (ft_putstr_error("Malloc error in ft_strjoin_free in handle piped "));
		i++;
	}
	return (0);
}

static void init_params(int *adv_steps, int *to_reduce)
{
	(*adv_steps) = 0;
	(*to_reduce) = 0;
}

int check_if_piped(t_big_token **tmp_b, int ind, t_info *info, int len)
{
	t_token *tmp;
	int i;

	(void) tmp_b;
	tmp = info->tokens;
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

void	ft_fdadd_back(t_fd **alst, t_fd *new)
{
	t_fd	*tmp;

	tmp = NULL;
	tmp = *alst;
	if (alst && new)
	{
		if (*alst == NULL)
			*alst = new;
		else
		{
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new;
		}
	}
}

int	ft_create_tmp(t_fd *fd, int hd)
{
	int	i;

	i = 0;
	if (hd)
	{
		fd->file = ft_strdup("/tmp/.tmp_hd_");
		if (!fd->file)
			return (1);
		while (--hd)
			i++;
		fd->file = ft_strjoin_free(fd->file, ft_itoa(i), 4);
		if (!fd->file)
			return (1);
	}
	return (0);
}

int	ft_fill_fdnew(t_fd *fd, t_token *tmp, int red, int *hd)
{
	if (red == 1 || red == 2)
		fd->red = red - 1;
	else
		fd->red = red - 3;
	if (red == 2)
	{
		*hd += 1;
		fd->delimitator = ft_strdup(tmp->value);
		if (ft_create_tmp(fd, *hd))
			return (1);
		fd->fd = open(fd->file, O_RDWR | O_CREAT | O_TRUNC, 0644);
		if (tmp->quoted)
			red = 5; 
		ft_here(fd, red);
	}
	else
		fd->file = ft_strdup(tmp->value);
	if (!fd->file)
		return (1);
	return (0);
}

int	ft_fdnew(t_big_token *b_tokens, t_fd **fd, t_token *tmp, int red)
{
	t_fd	*new;

	new = ft_calloc(sizeof(t_fd), 1);
	if (!new)
		return (ft_putstr_error("Malloc error in ft_fdnew "));
	new->info = b_tokens->info;
	if (ft_fill_fdnew(new, tmp, red, &(b_tokens)->nb_hd))
		return (ft_putstr_error("Malloc error in ft_fdnew "));
	ft_fdadd_back(fd, new);
	return (0);
}

void	count_cmd_args(t_big_token **tmp_b, int ind, t_token **tokens, int len)
{
	t_token *tmp;
	int count;
	int red;

	tmp = *tokens;
	count = 0;
	red = 0;
	move_tok_2_ind(&tmp, ind);
	while (tmp && len)
	{
		if (tmp->token == TOK_REDIRECTOR_LEFT || tmp->token == TOK_REDIRECTOR_RIGHT)
		{
			red = 1;
			tmp = tmp->next;
			len--;
		}
		else if (tmp->token == TOK_WORD || tmp->token == TOK_WORD_D_QUOTED || tmp->token == TOK_WORD_S_QUOTED) // ajout des mots quoted comme args
		{
			if (!red)
			{
				while (len && tmp->token != TOK_SEP)
				{
					tmp = tmp->next;
					len--;
				}
				count++;
			}
			else
			{
				tmp = tmp->next;
				len--;
				red = 0;
			}
		}
	}
	(*tmp_b)->cmd_args = ft_calloc(sizeof(char *), count + 1);
	(*tmp_b)->cmd_args_num = count;
}

void rd_inout_type(char *str, int *type_red)
{
	if (ft_strlen(str) == 1 && !ft_strncmp(str, "<", 1))
		(*type_red) = 1;
	if (ft_strlen(str) == 2 && !ft_strncmp(str, "<<", 2))
		(*type_red) = 2;
	if (ft_strlen(str) == 1 && !ft_strncmp(str, ">", 1))
		(*type_red) = 3;
	if (ft_strlen(str) == 2 && !ft_strncmp(str, ">>", 2))
		(*type_red) = 4;
}

/*void handle_red_files(t_big_token **tmp_b, char *tmp_value, int (*inouthd)[3], int type_red)
{
	if (type_red == 1)
	{
//		(*tmp_b)->infile[((*tmp_b)->rd_inouthd)[0] - (*inouthd)[0]] = ft_strdup(tmp_value);
		((*inouthd)[0])--;
	}
	if (type_red == 2)
	{
//		(*tmp_b)->delimitator[((*tmp_b)->rd_inouthd)[2] - (*inouthd)[2]] = ft_strdup(tmp_value);
		((*inouthd)[2])--;
	}
	if (type_red == 3 || type_red == 4)
	{
//		printf("got here to fill the outfile with tmp->value: %s!\n", tmp_value);
//		printf("genius that I thought I was. Here are rd_inouthd[1]: %d and (*inouthd)[1]: %d\n", ((*tmp_b)->rd_inouthd)[1], (*inouthd)[1]);
//		(*tmp_b)->outfile[((*tmp_b)->rd_inouthd)[1] - (*inouthd)[1]] = ft_strdup(tmp_value);
		((*inouthd)[1])--;
	}
}*/

int	handle_par_dir(t_token **tmp_s, t_big_token **tmp_b, t_info *info, int ind_word)
{
	t_token *tmp;
	int i;
	int j;
	int cl_ind;
	int type_red;
	int len;
	int to_reduce;
	int to_start;

	(void)tmp_s;
	(void)ind_word;
	tmp = info->tokens;
	i = 0;
	j = 0;
	type_red = 0;
	len = 0;
	to_reduce = 0;
	to_start = 0;
	handle_piped(tmp_b, info);
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
			return (1);
		}
		else
		{
			to_reduce++;
			move_tok_2_ind(&tmp, cl_ind + 1);
			len = (*tmp_b)->length + (*tmp_b)->ind_tok_start - 1 - cl_ind;
			if (check_if_piped(tmp_b, cl_ind + 1, info, len))
			{
	//			printf("I'm piped darling!\n");
				return (0);
			}
			to_reduce += len;
		}
	}
	type_red = 0;
	while (tmp && j < len)
	{
		if ((tmp->token == TOK_REDIRECTOR_LEFT || tmp->token == TOK_REDIRECTOR_RIGHT) && i % 2)
		{
	//		printf("There is a problem in redirections!\n");
			return (1);
		}
		if ((tmp->token == TOK_REDIRECTOR_LEFT || tmp->token == TOK_REDIRECTOR_RIGHT) && !(i % 2))
		{
	//		printf("I entered here with : %s\n", tmp->value);
			rd_inout_type(tmp->value, &type_red);
	//		printf("typered: %d\n", type_red);
			i++;
		}
		else if (tmp->token == TOK_WORD && (!(i % 2) || !type_red))
		{
	//		printf("tmp_tok: %s, %d, %d\n", tmp->value, i, type_red);
	//		printf("problem here putain!\n");
			return (1);
		}
		if (tmp->token == TOK_WORD && (i % 2))
		{
			if (type_red == 1 || type_red == 2)
			{
				if (ft_fdnew(*(tmp_b), &(*tmp_b)->fd_in, tmp, type_red))
					return (ft_putstr_error("in handle par dir "));
			}
			else
				if (ft_fdnew(*(tmp_b), &(*tmp_b)->fd_out, tmp, type_red))
					return (ft_putstr_error("in handle par dir "));
			i++;
		}
		tmp = tmp->next;
		j++;
	}
	(*tmp_b)->ind_tok_start += to_start;
	(*tmp_b)->length -= to_reduce;
	(*tmp_b)->par = 1;
	return (0);
}

int	handle_dir(t_big_token **tmp_b, t_info *info)
{
	t_token *tmp;
	int i;
	int j;
	int type_red;
	int len;
	int save_word;
	int	cmd_args_num;

	tmp = info->tokens;
	i = 0;
	j = 0;
	len = (*tmp_b)->length;
	type_red = 0;
	save_word = 0;
	move_tok_2_ind(&tmp, (*tmp_b)->ind_tok_start);
	count_cmd_args(tmp_b, (*tmp_b)->ind_tok_start, (&info->tokens), (*tmp_b)->length);
	cmd_args_num = (*tmp_b)->cmd_args_num;
	type_red = 0;
	while (tmp && j < len)
	{
		if ((tmp->token == TOK_REDIRECTOR_LEFT || tmp->token == TOK_REDIRECTOR_RIGHT) && !(i % 2))
		{
			rd_inout_type(tmp->value, &type_red);
			i++;
			save_word = 1;
		}
		else if ((tmp->token == TOK_WORD || tmp->token == TOK_WORD_D_QUOTED || tmp->token == TOK_WORD_S_QUOTED) && !save_word)
		{
				while (j < len && tmp->token != TOK_SEP && tmp)
				{
				(*tmp_b)->cmd_args[(*tmp_b)->cmd_args_num - cmd_args_num] = ft_strjoin_free((*tmp_b)->cmd_args[(*tmp_b)->cmd_args_num - cmd_args_num], tmp->value, 1);
			if (!(*tmp_b)->cmd_args[(*tmp_b)->cmd_args_num - cmd_args_num])
				return (ft_putstr_error("Malloc error in ft_strdup in handle dir "));
				j++;
				tmp = tmp->next;
				}
				//function that fills the arg with everything before reaching the SEP)
			cmd_args_num--;
		}
		else if ((tmp->token == TOK_WORD || tmp->token == TOK_WORD_D_QUOTED || tmp->token == TOK_WORD_S_QUOTED) && (i % 2) && save_word)
		{
			if (type_red == 1 || type_red == 2)
			{
				if (ft_fdnew(*tmp_b, &(*tmp_b)->fd_in, tmp, type_red))
					return (ft_putstr_error("in handle dir "));
			}
			else
				if (ft_fdnew(*tmp_b, &(*tmp_b)->fd_out, tmp, type_red))
					return (ft_putstr_error("in handle dir "));
			save_word = 0;
			i++;
		}
		tmp = tmp->next;
		j++;
	}
	(*tmp_b)->par = 0;
	return (0);
}

int	handle_par(t_big_token **b_tokens, t_info *info)
{
	t_big_token *tmp_b;
	t_token *tmp_s;
	int params[2];
	int st_par;
	int end_par;

	tmp_b = *b_tokens;
	tmp_s = info->tokens;
	if (is_pipe_in_st_end(tmp_b, tmp_s) || is_red_st_par(tmp_b, tmp_s))
		return (ft_putstr_error("Syntax error 2\n"), 2);
	while (tmp_b)
	{
		init_params(&(params[0]), &(params[1]));
		tmp_s = info->tokens;
		move_tok_2_ind(&tmp_s, tmp_b->ind_tok_start);
		if (tmp_s->token == TOK_SEP && tmp_b->length > 2)
			handle_par_1(&tmp_s, tmp_b, &(params[0]), &(params[1]));
		if (tmp_s->token == TOK_EXPANDER_OP && tmp_b->length > 2)
		{
			st_par = tmp_s->index;
			move_tok_2_ind(&tmp_s, tmp_b->ind_tok_start + tmp_b->length - 1);
			if (tmp_s->token == TOK_SEP)
				handle_par_2(&tmp_s, tmp_b, &(params[1]), info->tokens);
			if (tmp_s->token == TOK_EXPANDER_CL)
			{
				end_par = tmp_s->index;
				if (end_par == cl_par_ind(&(info->tokens), st_par))
					handle_par_3(&tmp_b, params[1], params[0], info);
			}
			else if (tmp_s->token == TOK_WORD)
			{
	//			printf("I'm here habibiiw!\n");
	//			printf("I have this word: %s\n", tmp_s->value);
				if (handle_par_dir(&tmp_s, &tmp_b, info, st_par))
					return (ft_putstr_error("in handle par "));
			}
		}
		else if (!piped(info, tmp_b->ind_tok_start, tmp_b->length))
		{
			if (handle_dir(&tmp_b, info))
				return (ft_putstr_error("in handle par "));
		}
		else
			if (handle_piped(&tmp_b, info))
				return (ft_putstr_error("in handle par "));
		tmp_b = tmp_b->sibling;
	}
	return (0);
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
