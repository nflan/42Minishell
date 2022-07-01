/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_tokenizer_4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 14:45:15 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/01 03:22:01 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

// itctlt[0] = i, itctlt[1] = type_red, itctlt[2] = cl_ind,
// itctlt[3] = to_reduce, itctlt[4] = len, itctlt[5] = to_start 
// int	handle_par_dir(t_big_token **tmp_b, t_info *info)
// {
// 	t_token	*tmp;
// 	int		itctlt[6];

// 	tmp = info->tokens;
// 	itctlt[0] = 0;
// 	itctlt[1] = 0;
// 	itctlt[3] = 0; 
// 	itctlt[5] = 0;
// 	handle_piped(tmp_b, info);
// 	move_tok_2_ind(&tmp, (*tmp_b)->ind_tok_start);
// 	if (tmp->token == TOK_SEP)
// 	{
// 		move_tok_2_ind(&tmp, (*tmp_b)->ind_tok_start + 1);
// 		itctlt[3]++;
// 		itctlt[5]++;
// 	}
// 	if (tmp->token == TOK_EXPANDER_OP)
// 	{
// 		itctlt[3]++;
// 		itctlt[5]++;
// 		itctlt[2] = cl_par_ind(&tmp, tmp->index);
// 		if (itctlt[2] == -1)
// 			return (1);
// 		else
// 		{
// 			itctlt[3]++;
// 			move_tok_2_ind(&tmp, itctlt[2] + 1);
// 			itctlt[4] = (*tmp_b)->length + (*tmp_b)->ind_tok_start - 1 - itctlt[2];
// 			if (check_if_piped(tmp_b, itctlt[2] + 1, info, itctlt[4]))
// 				return (0);
// 			itctlt[3] += itctlt[4];
// 		}
// 	}
// 	while (tmp && itctlt[4]--)
// 	{
// 		if ((tmp->token == TOK_REDIRECTOR_LEFT || tmp->token == TOK_REDIRECTOR_RIGHT) && itctlt[0] % 2)
// 			return (1);
// 		if ((tmp->token == TOK_REDIRECTOR_LEFT || tmp->token == TOK_REDIRECTOR_RIGHT) && !(itctlt[0] % 2))
// 		{
// 			rd_inout_type(tmp->value, &itctlt[1]);
// 			itctlt[0]++;
// 		}
// 		else if (tmp->token == TOK_WORD && (!(itctlt[0] % 2) || !itctlt[1]))
// 			return (1);
// 		if (tmp->token != TOK_SEP && (itctlt[0] % 2))
// 		{
// 			if (itctlt[1] == 1 || itctlt[1] == 2)
// 			{
// 				if (ft_fdnew(*(tmp_b), &(*tmp_b)->fd_in, &tmp, itctlt))
// 					return (ft_putstr_error("in handle par dir "));
// 			}
// 			else
// 				if (ft_fdnew(*(tmp_b), &(*tmp_b)->fd_out, &tmp, itctlt))
// 					return (ft_putstr_error("in handle par dir "));
// 			itctlt[0]++;
// 		}
// 		else
// 		{
// 			tmp = tmp->next;
// 			itctlt[4]--;
// 		}
// 	}
// 	(*tmp_b)->ind_tok_start += itctlt[5];
// 	(*tmp_b)->length -= itctlt[3];
// 	(*tmp_b)->par = 1;
// 	return (0);
// }
// // itscl[0] = i, itscl[1] = type_red, itscl[2] = save_word, itscl[3] = cmd_args_nums
// // itscl[4] = b_length
// int	handle_dir(t_big_token **tmp_b, t_info *info)
// {
// 	t_token	*tmp;
// 	int		itscl[5];

// 	tmp = info->tokens;
// 	itscl[0] = 0;
// 	itscl[1] = 0;
// 	itscl[2] = 0;
// 	itscl[4] = (*tmp_b)->length;
// 	move_tok_2_ind(&tmp, (*tmp_b)->ind_tok_start);
// 	count_cmd_args(tmp_b, (*tmp_b)->ind_tok_start, (&info->tokens), (*tmp_b)->length);
// 	itscl[3] = (*tmp_b)->cmd_args_num;
// 	while (tmp && itscl[4])
// 	{
// 		if ((tmp->token == TOK_REDIRECTOR_LEFT || tmp->token == TOK_REDIRECTOR_RIGHT) && !(itscl[0] % 2))
// 		{
// 			rd_inout_type(tmp->value, &itscl[1]);
// 			itscl[0]++;
// 			itscl[2] = 1;
// 			tmp = tmp->next;
// 			itscl[4]--;
// 		}
// 		else if (tmp->token != TOK_SEP && !itscl[2])
// 		{
// 			while (tmp && itscl[4] && tmp->token != TOK_SEP)
// 			{
// 				(*tmp_b)->cmd_args[(*tmp_b)->cmd_args_num - itscl[3]] = ft_strjoin_free((*tmp_b)->cmd_args[(*tmp_b)->cmd_args_num - itscl[3]], tmp->value, 1);
// 				if (!(*tmp_b)->cmd_args[(*tmp_b)->cmd_args_num - itscl[3]])
// 					return (ft_putstr_error("Malloc error in ft_strdup in handle dir "));
// 				tmp = tmp->next;
// 				itscl[4]--;
// 			}
// 			itscl[3]--;
// 		}
// 		else if ((tmp->token != TOK_SEP && (itscl[0] % 2) && itscl[2]))
// 		{
// 			if (itscl[1] == 1 || itscl[1] == 2)
// 			{
// 				if (ft_fdnew(*tmp_b, &(*tmp_b)->fd_in, &tmp, itscl))
// 					return (ft_putstr_error("in handle dir "));
// 			}
// 			else
// 				if (ft_fdnew(*tmp_b, &(*tmp_b)->fd_out, &tmp, itscl))
// 					return (ft_putstr_error("in handle dir "));
// 			itscl[2] = 0;
// 			itscl[0]++;
// 		}
// 		else
// 		{
// 			tmp = tmp->next;
// 			itscl[4]--;
// 		}
// 	}
// 	(*tmp_b)->par = 0;
// 	return (0);
// }

// int	handle_par(t_big_token **b_tokens, t_info *info)
// {
// 	t_big_token	*tmp_b;
// 	t_token		*tmp_s;
// 	int			params[2];
// 	int			st_par;
// 	int			end_par;

// 	tmp_b = *b_tokens;
// 	tmp_s = info->tokens;
// 	if (is_pipe_in_st_end(tmp_b, tmp_s) || is_red_st_par(tmp_b, tmp_s))
// 		return (ft_putstr_error("Syntax error 2\n"), 2);
// 	while (tmp_b)
// 	{
// 		init_params(&(params[0]), &(params[1]));
// 		tmp_s = info->tokens;
// 		move_tok_2_ind(&tmp_s, tmp_b->ind_tok_start);
// 		if (tmp_s->token == TOK_SEP && tmp_b->length > 2)
// 			handle_par_1(&tmp_s, tmp_b, &(params[0]), &(params[1]));
// 		if (tmp_s->token == TOK_EXPANDER_OP && tmp_b->length > 2)
// 		{
// 			st_par = tmp_s->index;
// 			move_tok_2_ind(&tmp_s, tmp_b->ind_tok_start + tmp_b->length - 1);
// 			if (tmp_s->token == TOK_SEP)
// 				handle_par_2(&tmp_s, tmp_b, &(params[1]), info->tokens);
// 			if (tmp_s->token == TOK_EXPANDER_CL)
// 			{
// 				end_par = tmp_s->index;
// 				if (end_par == cl_par_ind(&(info->tokens), st_par))
// 					handle_par_3(&tmp_b, params[1], params[0], info);
// 			}
// 			else if (tmp_s->token == TOK_WORD)
// 			{
// 				//			printf("I'm here habibiiw!\n");
// 				//			printf("I have this word: %s\n", tmp_s->value);
// 				if (handle_par_dir(&tmp_b, info))
// 					return (ft_putstr_error("in handle par "));
// 			}
// 		}
// 		else if (!piped(info, tmp_b->ind_tok_start, tmp_b->length))
// 		{
// 			if (handle_dir(&tmp_b, info))
// 				return (ft_putstr_error("in handle par "));
// 		}
// 		else
// 			if (handle_piped(&tmp_b, info))
// 				return (ft_putstr_error("in handle par "));
// 		tmp_b = tmp_b->sibling;
// 	}
// 	return (0);
// }

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
