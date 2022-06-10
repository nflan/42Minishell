#include "../include/minishell.h"

int	depth_b_token(t_big_token **b_token)
{
	t_big_token	*tmp_b;
	int			depth;

	tmp_b = *b_token;
	depth = 0;
	while (tmp_b)
	{
		depth++;
		tmp_b = tmp_b->child;
	}
	return (depth);
}
