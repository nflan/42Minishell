/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 14:30:49 by omoudni           #+#    #+#             */
/*   Updated: 2022/05/19 19:20:49 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

char *ft_strncpy(char *str, int n)
{
	int i;
	char *ret;

	i = 0;
	if (!str)
		return (NULL);
	ret = malloc(n * sizeof(char));
	if (!ret)
		return (NULL);
	while (i < n)
	{
		ret[i] = str[i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}

t_token *ft_create_token(t_tok_type tok_type, int length, int i)
{
	t_token *tok;

	tok = (t_token *)malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->token = tok_type;
	tok->length = length;
	tok->start = i;
	tok->next = NULL;
	return (tok);
}

void add_tok_last(t_token **tok_list, t_tok_type tok_type, int length, int i)
{
	t_token *tmp;
	t_token *bef_last;
	int rank_in_list;

	if (!*tok_list)
	{
		*tok_list = ft_create_token(tok_type, length, i);
		(*tok_list)->prev = NULL;
		rank_in_list = 0;
	}
	else
	{
		tmp = *tok_list;
		while (tmp->next)
			tmp = tmp->next;
		bef_last = tmp;
		bef_last->next = ft_create_token(tok_type, length, i);
		bef_last = bef_last->next;
		bef_last->prev = tmp;
		rank_in_list = 1;
	}
	init_tok_struct(tok_list, rank_in_list);
}

int is_quoted(t_token **tok_list, int rank_in_list)
{
	t_token *tmp;
	int res;

	res = 1;
	tmp = *tok_list;
	if (!rank_in_list)
		return (0);
	else
	{
		while (tmp->prev)
		{
			tmp = tmp->prev;
			if (tmp->token == TOK_QUOTER)
				res *= -1;
		}
		if (res < 0)
			return (1);
		return (0);
	}
}

void init_tok_struct(t_token **tok_list, int rank_in_list)
{
	if (is_quoted(tok_list, rank_in_list))
		(*tok_list)->quoted = 1;
	else
		(*tok_list)->quoted = 0;
}

int len_ll_list(t_token *tok_list)
{
	int len;

	len = 0;
	if (!tok_list)
		return (len);
	while (tok_list)
	{
		len++;
		tok_list = tok_list->next;
	}
	return (len);
}

unsigned int get_real_tok_type(char c, t_token **tok_list)
{
	int len;

	len = len_ll_list(*tok_list);
	if (len == 0 || len == 1)
		return (get_tok_type[get_char_class[c]]);
	else
	{
		if (is_quoted(tok_list, 1))
			return (TOK_WORD);
		else
			return (get_tok_type[get_char_class[c]]);
	}
	return (-1);
}

void detect_tokens(t_token **tok_list, char *str)
{
	int i;
	unsigned int tok_type;
	int length;
	int start;

	i = 0;
	if (!str)
		return;
	while (str[i])
	{
		length = 1;
		start = i;
		tok_type = get_real_tok_type(str[i], tok_list);
		i++;
		while (str[i] && get_real_tok_type(str[i], tok_list) == tok_type)
		{
			length++;
			i++;
		}
		add_tok_last(tok_list, tok_type, length, start);
	}
}

void fill_tok_value(t_token **tok, char *str)
{
	t_token *tmp;

	if (!*tok || !str)
		return;
	tmp = *tok;
	while (tmp)
	{
		tmp->value = ft_strncpy(&(str[tmp->start]), tmp->length);
		tmp = tmp->next;
	}
}

int count_op_tok(t_token **tokens)
{
	t_token *tmp;
	int ret;

	ret = 0;
	while (tmp)
	{
		if (tmp->token == TOK_EXPANDER_OP)
			ret++;
		tmp = tmp->next;
	}
	return (ret);
}

int count_cl_tok(t_token **tokens)
{
	t_token *tmp;
	int ret;

	ret = 0;
	while (tmp)
	{
		if (tmp->token == TOK_EXPANDER_CL)
			ret++;
		tmp = tmp->next;
	}
	return (ret);
}

int check_count_errors(t_token **tokens)
{
	t_token *tmp;

	tmp = *tokens;
	while (tmp)
	{
		if (tmp->token == TOK_OPERATOR && ft_strlen(tmp->value))
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int r_quotes_impair(t_token **tokens)
{
	t_token *tmp;
	int q_num;

	q_num = 0;
	tmp = *tokens;
	while (tmp)
	{
		if (tmp->token == TOK_QUOTER)
			q_num++;
		tmp = tmp->next;
	}
	if (q_num % 2)
		return (1);
	return (0);
}

int ll_len(t_token **tokens)
{
	int len;
	t_token *tmp;

	len = 0 tmp = *tokens;
	while (tmp)
	{
		len++;
		tmp = tmp->next;
	}
	return (len);
}

int is_last_op(t_token **tokens)
{
	t_token *tmp;

	tmp = *tokens;
	while (tmp->next)
	{
		tmp = tmp->next;
	}
	if (tmp->token == TOK_OPERATOR)
		return (1);
	return (0);
}

int syntax_err_handler(t_token **tokens)
{
	int nb_optok;
	int nb_cltok;
	int err_num[3];

	err_num[0] = 0;
	err_num[1] = 0;
	err_num[2] = 0;
	if (!*tokens)
		return (-1);
	nb_optok = count_op_tok(tokens);
	nb_cltok = count_cl_tok(tokens);
	if (nb_optok != nb_cltok)
		return (1);
	if ((*tokens)->token == TOK_OPERATOR)
		return (2);
	if (check_count_errors(tokens))
		return (3);
	if (r_quotes_impair(tokens))
		return (4);
	if (is_last_op(tokens))
		return (5);
	return (0);
}

void index_toks(t_token **tokens)
{
	int i;
	t_token *tmp;

	i = 0;
	tmp = *tokens;
	while (tmp)
	{
		tmp->index = i;
		i++;
		tmp = tmp->next;
	}
}


int	check_for_operators(t_token **tokens, int index_start)
{
	int	i;
	t_token	*tmp;
	int	len;
	
	tmp = *tokens;
	len_ll = ll_len(tokens); 
	if (index_start >= ll_len)
		rreturn (-1);
	while (tmp)
	{
		tmp = tmp->next;

	}
	while (tmp)
	{
		if (tmp->token == TOK_OPERATOR)
			return (tmp->index);	
		tmp = tmp->next;
	}
}

void	divide_b_toks(t_token **tokens)
{
	
}

int main(int argc, char *argv[])
{
	t_token *tokens;

	tokens = NULL;
	detect_tokens(&tokens, argv[1]);
	fill_tok_value(&tokens, argv[1]);
	t_token *tmp;
	tmp = tokens;
	while (tmp)
	{
		printf("%d\n", tmp->token);
		printf("%s\n", tmp->value);
		printf("%d\n", tmp->length);
		printf("%d\n", tmp->start);
		printf("%d\n", tmp->quoted);
		printf("\n\n");
		tmp = tmp->next;
	}
	return 0;
}
