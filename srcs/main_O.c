/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_O.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 20:08:35 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/22 20:08:38 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	t_big_token	*tmp_b;

	info->tokens = NULL;
	info->new_tokens = NULL;
	info->parse = NULL;
	detect_tokens(&info->tokens, info->rdline);
	fill_tok_value(&info->tokens, info->rdline);
	index_toks(&info->tokens, 0, len_ll_list(info->tokens));
	if (syntax_err_handler(&info->tokens))
		printf("erreur syntaxique\n");

	// print_s_tokens(&(info->tokens), 0, len_ll_list(info->tokens));
	// printf("\n----------------------\n");
	// dol_expand(&(info->tokens), info);
	// print_s_tokens(&(info->tokens), 0, len_ll_list(info->tokens));
	// printf("\n----------------------\n");
	// expanded_toks(&(info->tokens), &(info->new_tokens));
	// index_toks(&info->new_tokens, 0, len_ll_list(info->new_tokens));
	// print_s_tokens(&(info->new_tokens), 0, len_ll_list(info->new_tokens));
	// printf("\n----------------------\n");
	// exit(0);

//	if (syntax_err_handler(&info->tokens))
//	{
//		printf("Error number: %d\n", syntax_err_handler(&info->tokens));
//		free_all_tokens(&info->tokens);
//		ft_putstr_fd("Parsing error\n", 2);
//		return (1);
//	}
	parse(&info->parse, &info->tokens, 0, len_ll_list(info->tokens));
	exit (0);
	tmp_b = info->parse;
//	print_tab(tmp_b->cmd_args);
	if (info->nb_cmd == 10)
		print_all_everything(&tmp_b, &info->tokens);
	return (0);
}
