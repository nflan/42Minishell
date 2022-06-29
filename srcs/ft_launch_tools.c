#include "../include/minishell.h"

void	ft_close_pdes(int fd, int pdes)
{
	if (pdes == 0 && fd != 0)
		close(fd);
	else if (pdes == 1 && fd != 1)
		close(fd);
}

void	ft_close_cmd(t_info *info, t_big_token *b_tokens, pid_t child)
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
		waitpid(child, &child, 0);
		ft_close_pdes(info->pdes[1], 1);
		ft_close_pdes(info->pdes[0], 0);
	}
	if (WIFEXITED(child))
		info->status = WEXITSTATUS(child);
}

int	ft_lead_fd(t_info *info, t_big_token *b_tokens)
{
	if (info->nb_cmd && b_tokens->type == TOK_LEFT_PIPE)
	{
		if (pipe(info->tmp) == -1)
			return (ft_error(5, info, NULL));
		info->pdes[1] = info->tmp[1];
	}
	else if (info->nb_cmd)
		info->pdes[1] = 1;
	if (b_tokens->fdin != 0)
	{
		ft_close_pdes(info->pdes[0], 0);
		info->pdes[0] = b_tokens->fdin;
	}
	if (b_tokens->fdout != 1)
	{
		ft_close_pdes(info->pdes[1], 1);
		info->pdes[1] = b_tokens->fdout;
	}
	return (0);
}

/*int	ft_deeper_bt(t_big_token *b_tokens, t_big_token **tmp_b)
{
	int	i;

	i = 0;
	*tmp_b = b_tokens;
	if (!tmp_b)
		return (-1);
	while ((*tmp_b)->child)
	{
		*tmp_b = (*tmp_b)->child;
		i++;
	}
	return (i);
}*/

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
