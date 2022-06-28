#include <stdio.h>

typedef enum s_tok_type
{
	TOK_IDK,
	TOK_SEP,
	TOK_D_QUOTER,
	TOK_QUOTER,
	TOK_S_QUOTER,
	TOK_EXPANDER_OP,
	TOK_EXPANDER_CL,
	TOK_OPERATOR,
	TOK_PATH,
	TOK_REDIRECTOR_LEFT,
	TOK_REDIRECTOR_RIGHT,
	TOK_WORD,
	TOK_WORD_NULL_S,
	TOK_WORD_NULL_D,
	TOK_WORD_S_QUOTED,
	TOK_WORD_D_QUOTED,
	TOK_EXPANDER,
} 			t_tok_type;

void	print_tok_type(t_tok_type token)
{

	if(token == TOK_SEP)
		printf("TOK_SEP\n");
	if(token == TOK_D_QUOTER)
		printf("TOK_D_QUOTER\n");
	if (token == TOK_S_QUOTER)
		printf("TOK_S_QUOTER\n");
	if (token == TOK_EXPANDER_OP)
		printf("TOK_EXPANDER_OP\n");
	if (token == TOK_EXPANDER_CL)
		printf("TOK_EXPANDER_CL\n");
	if (token == TOK_OPERATOR)
		printf("TOK_OPERATOR\n");
	if (token == TOK_REDIRECTOR_LEFT)
		printf("TOK_REDIRECTOR_LEFT\n");
	if (token == TOK_REDIRECTOR_RIGHT)
		printf("TOK_REDIRECTOR_RIGHT\n");
	if (token == TOK_WORD)
		printf("TOK_WORD\n");
}

void	init_tok_type_tab(t_tok_type (*tok_type_tab)[127])
{
	int	i;

	i = -1;
	while (++i < 9)
		(*tok_type_tab)[i] = TOK_WORD;
	(*tok_type_tab)[i] = TOK_SEP;
	while (++i < 32)
		(*tok_type_tab)[i] = TOK_WORD;
	(*tok_type_tab)[i] = TOK_SEP; // i = 32
	(*tok_type_tab)[++i] = TOK_WORD; // i = 33
	(*tok_type_tab)[++i] = TOK_D_QUOTER; //i = 34
	while (++i < 38)
		(*tok_type_tab)[i] = TOK_WORD;
	(*tok_type_tab)[i] = TOK_OPERATOR; // i = 38
	(*tok_type_tab)[++i] = TOK_S_QUOTER; //i = 39
	(*tok_type_tab)[++i] = TOK_EXPANDER_OP; //i = 40
	(*tok_type_tab)[++i] = TOK_EXPANDER_CL; //i = 41
	while (++i < 60)
		(*tok_type_tab)[i] = TOK_WORD;
	(*tok_type_tab)[i] = TOK_REDIRECTOR_LEFT; // i = 38
	(*tok_type_tab)[++i] = TOK_WORD; // i = 33
	(*tok_type_tab)[++i] = TOK_REDIRECTOR_RIGHT; // i = 38
	while (++i < 124)
		(*tok_type_tab)[i] = TOK_WORD;
	(*tok_type_tab)[i] = TOK_OPERATOR;  // i = 124
	while (++i < 128)
		(*tok_type_tab)[i] = TOK_WORD;
}

int	main(int argc, char **argv)
{
	t_tok_type	tab[127];
	int			i;

	i = 0;
	init_tok_type_tab(&tab);
	
	while (i < 128)
	{
		printf("%d ", i); 
		print_tok_type(tab[i]);
		i++;
	}
	return (0);
}
