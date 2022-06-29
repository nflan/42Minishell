#include "../include/minishell.h"

int	ft_exec_simple(t_info *info, t_big_token *b_tokens)
{
	t_big_token	*tmp_b;

	tmp_b = b_tokens;
	if (ft_wash_btoken(info, tmp_b))
		return (2147483647);
	if (tmp_b->sc == -1)
	{
		if (ft_add_wildcards(info, b_tokens))
			return (ft_putstr_error("Wildcards error\n"));
		ft_launch_cmd(info, tmp_b);
		tmp_b->sc = info->status;
	}
	ft_close_fd(b_tokens);
	return (0);
}

int	exec_the_bulk(t_info *info, int sib_child, t_big_token *b)
{
	info->nb_cmd = 0;
	if (!ft_open_fd(b))
	{
		if (!b->par)
		{
			if (ft_check_expand(info->tokens, b->ind_tok_start, b->length))
			{
				dol_expand(&info->tokens, info, b);
				expanded_toks(&info->tokens, b->ind_tok_start, b->length);
				index_toks(&info->tokens);
				if (ft_noquote_args(b))
					return (1);
			}
		}
		if (sib_child >= 1 && sib_child <= 3)
			ft_exec_simple(info, b);
		else if (sib_child == 4)
			ft_init_pipex(info, b);
	}
	ft_close_fd(b);
	if (b && b->parent)
		give_parent_sc(&(b), &(b->parent));
	return (0);
}
