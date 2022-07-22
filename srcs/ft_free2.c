/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 17:46:15 by nflan             #+#    #+#             */
/*   Updated: 2022/07/21 21:02:06 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_free_tokens(t_token *tokens)
{
	if (!tokens)
		return ;
	if (tokens->next)
		ft_free_tokens(tokens->next);
	if (tokens->value)
	{
		free(tokens->value);
		tokens->value = NULL;
	}
	free(tokens);
	tokens = NULL;
}

void	ft_free_env(t_env *env)
{
	t_env	*tmp;

	tmp = NULL;
	if (!env)
		return ;
	while (env)
	{
		tmp = env;
		env = tmp->next;
		if (tmp->name)
			free(tmp->name);
		tmp->name = NULL;
		if (tmp->value)
			free(tmp->value);
		tmp->value = NULL;
		free(tmp);
		tmp = NULL;
	}
}
