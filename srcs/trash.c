
// int check_for_operators(t_token **tokens, int index_start, )
// {
// 	int i;
// 	t_token *tmp;
// 	int len;

// 	tmp = *tokens;
// 	len = len_ll_list(tokens);
// 	if (index_start >= len)
// 		return (-1);
// 	while (tmp)
// 	{
// 		if (tmp->index == index_start)
// 			break;
// 		tmp = tmp->next;
// 	}
// 	while (tmp)
// 	{
// 		if (tmp->token == TOK_OPERATOR)
// 			return (tmp->index);
// 		tmp = tmp->next;
// 	}
// 	return (-2);
// }



// int	is_there_oth_op(t_token *tokens)
// {
// 	t_token *tmp;

// 	tmp = tokens;
// 	while (tmp)
// 	{
// 		if (check_divider_type(tmp->token) > 0)
// 			return (tmp->index);
// 		tmp = tmp->next;
// 	}
// 	return (-1);
// }



// void	browse_tokens(t_token **tokens)
// {
// 	t_token	*tmp;
// 	int		jump;
// 	int		par;
// 	int		and_for_none;

// 	par = 0;
// 	tmp = *tokens;
// 	and_for_none = 0;
// 	while (tmp)
// 	{
// 		if (tmp->value == TOK_EXPANDER_OP)
// 		{
// 			jump = cl_par_ind(tokens, tmp->index);
// 			if ((jump + 1) >= len_ll_list(tokens))
// 			{
// 				move_tok_2_ind(&tmp, jump);
// 				tmp = tmp->next;
// 				//create wahed kayn entre parentheses
// 			}
// 			else
// 			{
// 				//create the big token with arenthesis (it'll be a clear token) avec valeur de start et de end (that would be the last token of your tokens)
// 			}
// 		}
// 		else if (check_divider_type(tmp->value) > 0)
// 		{
// 			if (is_there_oth_op(tokens) > -1)
// 			{
// 				//create chnou binathoum
// 				//n9ez lmn b3d ma tsala segment
// 			}
// 			else
// 			{
// 				//create a big token ghadi htal lkher
// 			}
// 		}
// 		else
// 			//ma3reftch
// 	}

// }

// void divide_b_toks(t_token **tokens)
// {
// 	int index_start;

// 	index_start = 0;
// 	while (1)
// 	{
// 		if (check_for_operators(tokens) > 0)
// 	}
// }
