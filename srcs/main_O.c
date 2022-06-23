#include "../include/minishell.h"

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

int main_agent_O(t_info *info)
{
	info->tokens = NULL;
	info->new_tokens = NULL;
	info->parse = NULL;
	if (detect_tokens(&info->tokens, info->rdline))
		return (ft_putstr_error("in main_agent_O\nDetect_tokens error\n"));
	if (fill_tok_value(&info->tokens, info->rdline))
		return (ft_putstr_error("in main_agent_O\nFill_tok_value error\n"));
	index_toks(&info->tokens);
	if (syntax_err_handler(&info->tokens))
	{
		printf("Error number: %d\n", syntax_err_handler(&info->tokens));
		return (ft_putstr_error("Syntax error\n"));
	}
	if (parse(&info->parse, &info->tokens, 0, len_ll_list(info->tokens)))
		return (ft_putstr_error("in main_agent_O\nParse error\n"));
//	print_tab(tmp_b->cmd_args);
	if (info->nb_cmd == 10)
		print_all_everything(&info->parse, &info->tokens);
	return (0);
}
