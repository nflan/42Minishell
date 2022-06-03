#include "minishell.h"
#include "libft/libft.h"

void	print_all_everything(t_big_token **b_tokens, t_token **tokens)
{
	t_big_token	*tmp_b;
	int	i;

	tmp_b = *b_tokens;
	i = 1;
	while (tmp_b)
	{
		printf("\nWelcome in the parent: %d\n", i);
		printf("\n");
		print_s_tokens(tokens, tmp_b->ind_tok_start, tmp_b->length);
		printf("\n");
		if (tmp_b->child)
			print_all_child(&(tmp_b->child), tokens, i);
		else
			printf("\nje n'ai pas de child moi!!\n");
		tmp_b = tmp_b->sibling;
		i++;
		printf("---------------");
	}
}

void	print_all_child(t_big_token **b_tokens, t_token **tokens, int i)
{
	t_big_token	*tmp_b;

	int	j;
	j = 1;
	tmp_b = *b_tokens;
	while (tmp_b)
	{
		printf("\nWelcome to the bunch of childs of order: %d of the parent: %d\n", j, i);
		print_b_tokens(&tmp_b, tokens);
		printf("\n");
		tmp_b = tmp_b->child;
		j++;
	}
}


void	print_b_tokens(t_big_token **b_tokens, t_token **tokens)
{
	t_big_token	*tmp_b;
	t_token		*tmp_s;
	int			i;

	tmp_b = *b_tokens;
	tmp_s = *tokens;
	i = 1;
	while (tmp_b)
	{
		printf("   --->>    ");
		print_s_tokens(&tmp_s, tmp_b->ind_tok_start, tmp_b->length);
		printf("\n");
		printf("It's type is: %d\n", tmp_b->type);
		printf("It's start is: %d\n", tmp_b->ind_tok_start);
		printf("It's length is: %d\n", tmp_b->length);
		printf("It's par_pam is: %d\n", tmp_b->par);
		tmp_b = tmp_b->sibling;
		i++;
	}
}

void	print_s_tokens(t_token **tokens, int start, int length)
{
	int i;
	t_big_token *tmp_b;
	t_token *tmp_s;

	tmp_s = *tokens;
	i = 0;
	move_tok_2_ind(&tmp_s, start);
	while (i < length && tmp_s)
	{
		printf("%s", tmp_s->value);
		tmp_s = tmp_s->next;
		i++;
	}
}
void    ft_print_test(t_big_token *b_tok, t_token *tokens, int i)
{
    while (--i > 0)
        printf("--> ");
    if (b_tok)
    {
        printf(">");
        print_s_tokens(&tokens, b_tok->ind_tok_start, b_tok->length);
        printf("<\n");
    }
}

void    ft_print_b_tok(t_big_token *b_tok, t_token *tokens, int i)
{
    i++;
    if (b_tok->child)
    {
        ft_print_b_tok(b_tok->child, tokens, i);
        ft_print_test(b_tok, tokens, i);
        printf("\n");
    }
    if (b_tok->sibling)
        ft_print_b_tok(b_tok->sibling, tokens, i);
    if (!b_tok->child)
        ft_print_test(b_tok, tokens, i);
}
int main(int argc, char *argv[])
{
	t_token *tokens;
	t_big_token *b_tokens;
	t_token *tmp;
	t_big_token	*tmp_b;

	tokens = NULL;
	b_tokens = NULL;
	detect_tokens(&tokens, argv[1]);
	fill_tok_value(&tokens, argv[1]);
	index_toks(&tokens, 0, len_ll_list(tokens));
	// divide_by_or_and(&b_tokens, &tokens, 0, len_ll_list(tokens));
	// printf("\n\n\ntest\n\n\n");
		// printf("before the dividing \n");
	// print_b_tokens(&b_tokens, &tokens);
	// divide_by_or_and(&(b_tokens->child), &tokens, b_tokens->ind_tok_start, b_tokens->length);
	// exit (0);
	parse(&b_tokens, &tokens, 0, len_ll_list(tokens));
	 tmp_b = b_tokens;
    ft_print_b_tok(tmp_b, tokens, 0);
	// tmp_b = b_tokens;
	// print_all_everything(&tmp_b, &tokens);
	exit(1);























	// printf("len tokens: %d\n", len_ll_list(tmp));
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
	// exit (0);
	// printf("\nthere are no syntax errors: %d\n", syntax_err_handler(&tokens));



	t_big_token *tmp2;
	tmp2 = b_tokens;
	// print_b_tokens(tmp2, tmp);
	exit(1);
	tmp = tokens;
	tmp2 = b_tokens;
	//	print_b_tokens(tmp2, tmp);
	//	divide_by_pipe(&tmp2, &tokens);
	/*
	   while (tmp2)
	   {
	   if (tmp2->child)
	   {
	   t_big_token	*tmp3;
	   tmp3 = tmp2->child;
	   print_b_tokens(tmp3, tmp);
	   }
	   tmp2 = tmp2->sibling;
	   }
	   */
	exit(0);
	//	tmp2 = b_tokens->child;
	printf("\n\n\n");
	// print_b_tokens(tmp2, tmp);
	return 0;
}
