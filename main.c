#include "minishell.h"
#include "libft/libft.h"

char	*concat_argvs(int argc, char **argv)
{
	int		i;
	char	*ret;
	char	*tmp;
	char	*tmp2;

	i = 2;
	tmp = NULL;
	ret = ft_strdup(argv[1]);
	while (i < argc)
	{
		tmp = ft_strjoin(ret, argv[i]);
		if (i != argc - 1)
			tmp2 = ft_strjoin(tmp, " ");
		else
			tmp2 = ft_strdup(tmp);
		if (tmp)
			free(tmp);
		if (ret)
			free(ret);
		ret = tmp2;
		printf("new ret %s with new i: %d\n", ret, i);
		i++;
	}
	return (ret);
}

int	main(int argc, char *argv[])
{
	t_token		*tokens;
	t_big_token	*b_tokens;
	t_big_token	*tmp_b;
	char		*cmd;

if (argc > 1)
{
	if (argc > 2)
	{
		cmd = concat_argvs(argc, argv);
		printf("%s\n", cmd);
	}
	else
	{
		cmd = argv[1];
		printf("%s\n", cmd);
	}
	exit(0);
	tokens = NULL;
	b_tokens = NULL;
	detect_tokens(&tokens, cmd);
	fill_tok_value(&tokens, cmd);
	index_toks(&tokens, 0, len_ll_list(tokens));
	parse(&b_tokens, &tokens, 0, len_ll_list(tokens));
	tmp_b = b_tokens;
	print_all_everything(&tmp_b, &tokens);
}
	return 0;
}
