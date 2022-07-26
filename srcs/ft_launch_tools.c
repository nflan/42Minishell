/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_launch_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 13:13:52 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/26 15:35:20 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_close_pdes(int fd, int pdes)
{
	if (pdes == 0 && fd != 0)
		close(fd);
	else if (pdes == 1 && fd != 1)
		close(fd);
}

void	ft_close_cmd(t_info *info, t_big_token *b_tokens)
{
	if (!info->nb_cmd)
		ft_close_pdes(info->pdes[1], 1);
	else if (b_tokens->type == TOK_LEFT_PIPE)
	{
		ft_close_pdes(info->pdes[0], 0);
		info->pdes[0] = info->tmp[0];
		ft_close_pdes(info->pdes[1], 1);
	}
	else
	{
		ft_close_pdes(info->pdes[1], 1);
		ft_close_pdes(info->pdes[0], 0);
	}
}

int	ft_lead_fd(t_info *info, t_big_token *b_tokens)
{
	if (info->nb_cmd && b_tokens->type == TOK_LEFT_PIPE)
	{
		if (pipe(info->tmp) == -1)
			exit (ft_mal_err(info, info->env, "Pipe error\n"));
		info->pdes[1] = info->tmp[1];
	}
	if (b_tokens->fdin < 0)
		return (1);
	return (0);
}

int	ft_wash_btoken(t_info *info, t_big_token *b_tokens)
{
	t_token	*tokens;
	int		i;

	i = b_tokens->ind_tok_start;
	tokens = info->tokens;
	while (tokens && i--)
		tokens = tokens->next;
	if (!tokens)
		return (1);
	if (tokens->token == TOK_SEP)
	{
		tokens = tokens->next;
		b_tokens->ind_tok_start++;
		b_tokens->length--;
		if (b_tokens->ind_tok_start == len_ll_list(info->tokens))
			return (1);
	}
	return (0);
}
