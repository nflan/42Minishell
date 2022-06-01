#include "minishell.h"
#include "libft/libft.h"

// void	print_tree(t_big_token **b_tokens, t_token **tokens)
// {
// 	t_big_token	*tmp_b_1;
// 	t_big_token	*tmp_b_2;

// 	tmp_b_1 = *b_tokens;
// 	tmp_b_2 = *b_tokens;
// 	while (tmp_b_1)
// 	{

// 	}
// }

void	print_all_everything(t_big_token **b_tokens, t_token **tokens)
{
	t_big_token	*tmp_b;
	int	i;

	tmp_b = *b_tokens;
	i = 1;
	while (tmp_b)
	{
		printf("\nWelcome in the first parent: %d\n", i);
		printf("\n");
		print_s_tokens(tokens, tmp_b->ind_tok_start, tmp_b->length);
		printf("\n");
		if (tmp_b->child)
			print_all_child(&(tmp_b->child), tokens);
		else
			printf("\nje n'ai pas de child moi!!\n");
		tmp_b = tmp_b->sibling;
		i++;
	}
}

void	print_all_child(t_big_token **b_tokens, t_token **tokens)
{
	t_big_token	*tmp_b;

	tmp_b = *b_tokens;
	while (tmp_b)
	{
		print_b_tokens(&tmp_b, tokens);
		printf("\n");
		tmp_b = tmp_b->child;
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
	printf("--");
	// printf("Here is big token : %d\n", i);
	print_s_tokens(&tmp_s, tmp_b->ind_tok_start, tmp_b->length);
	// printf("(voici son type: %d)", tmp_b->type);
	// printf("( start: %d len : %d + ())", tmp_b->ind_tok_start, tmp_b->length);
	// print_s_tokens(tokens, tmp_b->ind_tok_start, tmp_b->length);
	if (tmp_b->par)
	{
	 	printf("(voici son type de tok : %d ())", tmp_b->type);
	 	// printf("( start: %d len : %d + ())", tmp_b->ind_tok_start, tmp_b->length);
	}
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
	// printf("\n");
	while (i < length && tmp_s)
	{
		printf("%s", tmp_s->value);
		tmp_s = tmp_s->next;
		i++;
	}
	// printf("\n");
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
	// index_toks(&tokens, 0, len_ll_list(tokens));
	//divide_by_or_and(&b_tokens, &tokens, 0, len_ll_list(tokens));
	// tmp = tokens;
	// tmp_b = b_tokens;
	// print_b_tokens(&tmp_b,&tmp);
	// tmp = tokens;
	// tmp_b = b_tokens;
	// print_b_tokens(&tmp_b,&tmp);
	parse(&b_tokens, &tokens, 0, len_ll_list(tokens));
	print_b_tokens(&b_tokens, &tokens);
	tmp_b = b_tokens;
	// while (tmp_b)
	// {
	// printf("this is the depth of the b_token: %d\n", depth_b_token(&tmp_b));
	// tmp_b = tmp_b->sibling;
	// }
	print_all_everything(&tmp_b, &tokens);
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
