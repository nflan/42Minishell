/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 15:45:04 by omoudni           #+#    #+#             */
/*   Updated: 2022/05/25 20:56:04 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

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

void init_tok_struct(t_token **tok_list, int rank_in_list)
{
	if (is_quoted(tok_list, rank_in_list))
		(*tok_list)->quoted = 1;
	else
		(*tok_list)->quoted = 0;
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

void print_b_tokens(t_big_token *b_token, t_token *tokens)
{
	int i;
	int j;

	while (b_token)
	{
		j = 0;
		printf("Here is a big token :\n");
		printf("this is the start of the b_token : %d\n", b_token->ind_tok_start);
		printf("this is the length of the b_token : %d\n", b_token->length);
		i = b_token->ind_tok_start;
		while (j < b_token->length)
		{
			move_tok_2_ind(&tokens, i);
			printf("%s", tokens->value);
			i++;
			j++;
		}
		printf("\n\n");
		b_token = b_token->sibling;
	}
}

int main(int argc, char *argv[])
{
	t_token *tokens;
	t_big_token *b_tokens;

	tokens = NULL;
	b_tokens = NULL;
	detect_tokens(&tokens, argv[1]);
	fill_tok_value(&tokens, argv[1]);
	index_toks(&tokens);
	t_token *tmp;
	tmp = tokens;
	// while (tmp)
	// {
	// 	printf("%d\n", tmp->token);
	// 	printf("%s\n", tmp->value);
	// 	printf("%d\n", tmp->length);
	// 	printf("%d\n", tmp->start);
	// 	printf("%d\n", tmp->quoted);
	// 	printf("%d\n", tmp->index);
	// 	printf("\n\n");
	// 	tmp = tmp->next;
	// }
	// printf("\nthere are no syntax errors: %d\n", syntax_err_handler(&tokens));
	divide_by_or_and(&b_tokens, &tokens);
	tmp = tokens;
	t_big_token *tmp2;
	tmp2 = b_tokens;
	print_b_tokens(tmp2, tmp);
	tmp = tokens;
	tmp2 = b_tokens;
	printf("%d\n", is_pipe_in_st_end(tmp2, tmp));
	divide_by_pipe(&b_tokens, &tokens);
	tmp2 = b_tokens->child;
	print_b_tokens(tmp2, tmp);
	return 0;
}
