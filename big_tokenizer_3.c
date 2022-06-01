/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_tokenizer_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 17:57:08 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/01 14:55:57 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

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
		// printf("\nlwngthhhhhhhhhh: %d\n", tmp_b->length);
		while (i < tmp_b->length)
		{
			length_piped++;
			move_tok_2_ind(&tmp_s, j);
			// printf("\n\n\nalertttt:  %d\n\n\n", j);
			if (tmp_s->token == TOK_EXPANDER_OP)
			{
				st_par = tmp_s->index;
				end_par = cl_par_ind(&tmp_s, tmp_s->token, tmp_s->index, tmp_s->value);
				length_piped += (end_par - st_par);
				j += (end_par - st_par);
				i += (end_par - st_par);
			}
			else if (tmp_s->token == TOK_OPERATOR && ft_strlen(tmp_s->value) == 1 && !ft_strncmp(tmp_s->value, "|", 1))
			{
			// printf("I added a child!\n");
				add_b_tok_sib_last(&((tmp_b)->child), TOK_LEFT_PIPE, start, length_piped - 1);
				start = tmp_s->index + 1;
				length_piped = 0;
			}
			j++;
			i++;
		}
		if (!((tmp_b)->child))
		{
			// printf("I added a weird child\n");
			add_b_tok_sib_last(&((tmp_b)->child), TOK_CLEAN, start, length_piped);
		}
		else
		{
			// printf("I added a child!\n");
			add_b_tok_sib_last(&((tmp_b)->child), TOK_PIPE_LAST, start, length_piped);
		}
		handle_par(&(tmp_b->child), tokens);
//		tmp_b = tmp_b->sibling;
//	}
}

/*void divide_by_pipe(t_big_token **b_tokens, t_token **tokens)
{
	t_token		*tmp_s;
	t_big_token	*tmp_b;
	int			length_piped;
	int			start;
	int			st_par;
	int			end_par;
	int			i;
	int			j;

	tmp_s = *tokens;
	tmp_b = *b_tokens;
	start = tmp_b->ind_tok_start;
	length_piped = 0;

 while (tmp_b)
 {
	// if par == 1 you do something else et tu te casses! :)
	i = 0;
	j = tmp_b->ind_tok_start;
	length_piped = 0;
	while (i < tmp_b->length)
	{
		length_piped++;
		move_tok_2_ind(&tmp_s, j);
		//
		//********************* here I gotta add the if par = 1 , you do again the divide by and and or*******
		// if (tmp_s->token == TOK_EXPANDER_OP)
		// 			{
		// 				st_par = tmp_s->index;
		// 				end_par = cl_par_ind(&tmp_s, tmp_s->token, tmp_s->index, tmp_s->value);
		// 				length_piped += (end_par - st_par);
		// //				move_tok_2_ind(&tmp_s, end_par);
		// 			}
		if (!tmp_b->par && tmp_s->token == TOK_OPERATOR && ft_strlen(tmp_s->value) == 1 && !ft_strncmp(tmp_s->value, "|", 1))
		{
			add_b_tok_sib_last(&((tmp_b)->child), TOK_LEFT_PIPE, tmp_b->ind_tok_start, length_piped - 1);
			start = tmp_s->index + 1;
			length_piped = 0;
		}
		j++;
		i++;
	}
	if (!((*b_tokens)->child))
		add_b_tok_sib_last(&((tmp_b)->child), TOK_CLEAN, start, length_piped);
	else
		add_b_tok_sib_last(&((tmp_b)->child), TOK_PIPE_LAST, start, length_piped);
	handle_par(&((tmp_b)->child), tokens);
	tmp_b = tmp_b->sibling;
}
}*/
/*void divide_by_pipe(t_big_token **b_tokens, t_token **tokens)
  {
  t_token		*tmp_s;
  t_big_token	*tmp_b;
  int			length_piped;
  int			start;
  int			st_par;
  int			end_par;
  int			i;
  int			j;

  tmp_s = *tokens;
  tmp_b = *b_tokens;
  start = tmp_b->ind_tok_start;
  length_piped = 0;

  i = 0;
  j = tmp_b->ind_tok_start;
  length_piped = 0;
  while (i < tmp_b->length)
  {
  length_piped++;
  move_tok_2_ind(&tmp_s, j);
  if (tmp_s->token == TOK_OPERATOR && ft_strlen(tmp_s->value) == 1 && !ft_strncmp(tmp_s->value, "|", 1))
  {
  add_b_tok_sib_last(&((tmp_b)->child), TOK_LEFT_PIPE, tmp_b->ind_tok_start, length_piped - 1);
  start = tmp_s->index + 1;
  length_piped = 0;
  }
  j++;
  i++;
  }
  if (!((*b_tokens)->child))
  add_b_tok_sib_last(&((tmp_b)->child), TOK_CLEAN, start, length_piped);
  else
  add_b_tok_sib_last(&((tmp_b)->child), TOK_PIPE_LAST, start, length_piped);
  handle_par(&((tmp_b)->child), tokens);
  }
  */
