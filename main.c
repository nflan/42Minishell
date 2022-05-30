#include "minishell.h"
#include "libft/libft.h"

void print_b_tokens(t_big_token *b_token, t_token *tokens)
{
	int i;
	int j;

	// printf ("this is the glabal length of toks: %d\n", len_ll_list(tokens));

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


	// divide_by_or_and(&b_tokens, &tokens, 0, len_ll_list(tokens));
	parse(&b_tokens, &tokens, 0,len_ll_list(tokens));
		exit (1);

	t_big_token *tmp2;
	tmp2 = b_tokens;
	print_b_tokens(tmp2, tmp);
	exit (1);
	tmp = tokens;
	tmp2 = b_tokens;
//	printf("%d\n", is_pipe_in_st_end(tmp2, tmp));
	divide_by_pipe(&b_tokens, &tokens);
	tmp2 = b_tokens->child;
	printf("\n\n\n");
	print_b_tokens(tmp2, tmp);
	return 0;
}
