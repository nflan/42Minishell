#include "minishell.h"
#include "libft/libft.h"

char *concat_argvs(int argc, char **argv)
{
	int i;
	char *ret;
	char *tmp;
	char *tmp2;

	i = 2;
	tmp = NULL;
	tmp2 = NULL;
	ret = ft_strdup(argv[1]);
	while (i < argc)
	{
		tmp2 = ft_strjoin(ret, " ");
		tmp = ft_strjoin(tmp2, argv[i]);
		free(tmp2);
		if (ret)
			free(ret);
		ret = tmp;
		i++;
	}
	return (ret);
}

void	free_all_tokens(t_token **tokens)
{
	t_token *tmp;
	t_token *tmp2;

	tmp = *tokens;
	while (tmp)
	{
		if (tmp->value)
			free(tmp->value);
		if (tmp->next)
		{
			tmp2 = tmp->next;
			free(tmp);
			tmp = tmp2;
		}
		else
		{
			free(tmp);
			tmp = NULL;
		}
	}
}

int main(int argc, char *argv[])
{
	t_token *tokens;
	t_big_token *b_tokens;
	t_big_token *tmp_b;
	char *cmd;

	if (argc > 1)
	{
		cmd = argv[1];
		tokens = NULL;
		b_tokens = NULL;
		detect_tokens(&tokens, cmd);
		fill_tok_value(&tokens, cmd);
		if (syntax_err_handler(&tokens))
		{
			free_all_tokens(&tokens);
			printf("Parsing Error\n");
			exit(0);
		}
		index_toks(&tokens, 0, len_ll_list(tokens));
		parse(&b_tokens, &tokens, 0, len_ll_list(tokens));
		tmp_b = b_tokens;
		print_all_everything(&tmp_b, &tokens);
	}
	return 0;
}
