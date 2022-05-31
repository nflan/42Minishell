#include "minishell.h"
#include "libft/libft.h"

void print_b_tokens(t_big_token *b_token, t_token *tokens_s, int start, int length)
{
	int i;
	int j;
	t_big_token	*tmp_b;
	t_token	*tokens;

	// printf ("this is the glabal length of toks: %d\n", len_ll_list(tokens));
	tmp_b = b_token;
	tokens = tokens_s;
	while (tmp_b)
	{
		j = 0;
		printf("Here is a big token :\n");
		printf("this is the start of the b_token : %d\n", tmp_b->ind_tok_start);
		printf("this is the length of the b_token : %d\n", tmp_b->length);
		printf("this is the par of the b_token : %d\n", tmp_b->par);
		i = tmp_b->ind_tok_start;
		printf("\n\ni : %d\n\n", i);
		printf("\n\ntmp->b->len: %d\n\n", tmp_b->length);
		while (j < tmp_b->length)
		{
//			printf("\n\nj: %d\n\n", j);
			move_tok_2_ind(&tokens, i);
//			printf("\n\nindex: %d\n\n", tokens->index);
			printf("%s", tokens->value);
			i++;
			j++;
		}
		printf("\n\n");
		tmp_b = tmp_b->sibling;
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
//	index_toks(&tokens, 0, len_ll_list(tokens));
	t_token *tmp;
	tmp = tokens;
	printf("len tokens: %d\n", len_ll_list(tmp));
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


	// divide_by_or_and(&b_tokens, &tokens, 0, len_ll_list(tokens));
	parse(&b_tokens, &tokens, 0,len_ll_list(tokens), 0);
 exit (1);

	t_big_token *tmp2;
	tmp2 = b_tokens;
	print_b_tokens(tmp2, tmp);
	exit (1);
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
	exit (0);
	//	tmp2 = b_tokens->child;
	printf("\n\n\n");
	//print_b_tokens(tmp2, tmp);
	return 0;
}
