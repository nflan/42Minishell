/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_launch_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 10:29:00 by nflan             #+#    #+#             */
/*   Updated: 2022/06/10 10:47:40 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_deeper_bt(t_big_token *b_tokens, t_big_token **tmp_b)
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
}

int	ft_wash_btoken(t_info *info, t_big_token *b_tokens)
{
	t_token	*tokens;
	int	i;

	i = b_tokens->ind_tok_start;
	tokens = info->tokens;
//	printf("WASH: Start = %d && length = %d\n", b_tokens->ind_tok_start, b_tokens->length);
//	printf("value = ");
//	print_s_tokens(&info->tokens, b_tokens->ind_tok_start, b_tokens->length);
//	printf("\n");
	while (tokens && i--)
	{
	//	printf("tokens->start = %d\n", tokens->start);
	//	printf("tokens->value = %s\n", tokens->value);
		tokens = tokens->next;
	}
	if (!tokens)
		return (1);
	if (tokens->token == TOK_SEP)
	{
		tokens = tokens->next;
		b_tokens->ind_tok_start++;
		b_tokens->length--;
		if (b_tokens->ind_tok_start == len_ll_list(info->tokens))
		{
			printf("Que des separateurs dans le bat7 (' ')\n");
			return (1);
		}
	}
	return (0);
}

t_cmd	*ft_convert_bt_cmd(t_info *info, t_big_token *b_tokens)
{
	t_cmd	*tmp;
	t_token	*tokens;
	int	i;

	i = b_tokens->ind_tok_start;
	tokens = info->tokens;
	while (tokens && i--)
		tokens = tokens->next;
	i = b_tokens->length;
	if (!tokens)
		return (NULL);
	tmp = ft_calloc(sizeof(t_cmd), 1);
	if (!tmp)
		return (NULL);
	while (tokens && i--)
	{
		tmp->cmd = ft_strjoin_free(tmp->cmd, tokens->value, 1);
		tokens = tokens->next;
	}
	if (!tmp->cmd)
		return (free(tmp), NULL);
	tmp->cmd_p = ft_split(tmp->cmd, ' ');
	if (!tmp->cmd_p)
		return (free(tmp->cmd), free(tmp), NULL);
	tmp->envp = ft_env_to_tab(info->env);
	tmp->fdin = 0;
	tmp->fdout = 1;
//	tmp->child = -1;
	return (tmp);
}

int	ft_builtins_no_fork(t_info *info, t_cmd *cmd)
{
	int	len;

	len = ft_strlen(cmd->cmd_p[0]) + 1;
	if (!ft_strncmp(cmd->cmd_p[0], "unset", len))
	{
		if (!ft_unset(info, cmd))
			return (1);
		else
			return (0);
	}
	else if (!ft_strncmp(cmd->cmd_p[0], "export", len))
		return (ft_export(info, cmd));
	else if (!ft_strncmp(cmd->cmd_p[0], "cd", len))
		return (ft_cd(info, cmd));
	else if (!ft_strncmp(cmd->cmd_p[0], "exit", len) || !ft_strncmp(cmd->cmd, "exit ", 5))
		ft_exit(info, cmd->cmd_p[1], cmd->cmd_p);
	return (2);
}

int	ft_builtins(t_info *info, t_cmd *cmd)
{
	int	len;

	len = ft_strlen(cmd->cmd_p[0]) + 1;
	if (!ft_strncmp(cmd->cmd_p[0], "pwd", len))
		return (ft_pwd());
	else if (!ft_strncmp(cmd->cmd_p[0], "env", len))
		return (ft_env(info));
	else if (!ft_strncmp(cmd->cmd_p[0], "echo", len))
		return (ft_echo(info, cmd));
	return (2);
}

int	ft_exit_cmd(t_info *info, t_cmd *cmd)
{
	if (cmd)
		ft_free_cmd(cmd);
	if (info)
		ft_free_all(info, info->env);
	exit (info->status);
}

void	ft_close_cmd(t_info *info, t_big_token *b_tokens, pid_t child)
{
	if (!info->nb_cmd && b_tokens->type == TOK_LEFT_PIPE)
		close(info->pdes[1]);
	else if (info->nb_cmd && b_tokens->type == TOK_LEFT_PIPE)
	{
//		if (info->pdes[0] != 0)
		close(info->pdes[0]);
		info->pdes[0] = info->tmp[0];
	//	if (info->pdes[1] != 1 && info->pdes[1] != 2)
		close(info->pdes[1]);
	}
	else
	{
		waitpid(child, &child, 0);
		if (info->pdes[0] != 0)
			close(info->pdes[0]);
	}
	if (WIFEXITED(child))
		info->status = WEXITSTATUS(child);
}

int	ft_lead_fd(t_info *info, t_big_token *b_tokens, t_cmd *cmd)
{
	if (info->nb_cmd && b_tokens->type == TOK_LEFT_PIPE)
	{
		if (pipe(info->tmp) == -1)
			return (ft_error(5, info, cmd));
		info->pdes[1] = info->tmp[1];
	}
	if (cmd->fdin != 0)
		info->pdes[0] = cmd->fdin;
	if (cmd->fdout != 1)
		info->pdes[1] = cmd->fdout;
	return (0);
}

int	ft_launch_cmd(t_info *info, t_big_token *b_tokens)
{
	t_cmd	*cmd;
	pid_t	child;
	static int	i = 0;

	child = -1;
	cmd = ft_convert_bt_cmd(info, b_tokens);
	if (!cmd)
		return (1);
//	printf("tour %d :\npdes[0] = %d && pdes[1] = %d\ntmp[0] = %d && tmp[1] = %d\n", i, info->pdes[0], info->pdes[1], info->tmp[0], info->tmp[1]);
	//printf("tour %d :\npdes[0] = %d && pdes[1] = %d\n", i, info->pdes[0], info->pdes[1]);
	if (ft_lead_fd(info, b_tokens, cmd))
		return (ft_putstr_error("FD problem\n"));;
	info->status = ft_builtins_no_fork(info, cmd);
	if (info->status == 2)
	{
		child = fork();
		if ((int) child == -1)
			return (ft_error(2, info, NULL));
		else if ((int) child == 0)
		{
//	printf("cmd->cmd = %s\n", cmd->cmd);
			if (ft_pipex(info, cmd, b_tokens))
				return (ft_free_cmd(cmd), 1);
			ft_exit_cmd(info, cmd);
		}
	}
	ft_close_cmd(info, b_tokens, child);
	i++;
	if (b_tokens->type != TOK_LEFT_PIPE)
		i = 0;
	return (ft_free_cmd(cmd), info->status);
}

int	ft_launch_sibling(t_info *info, t_big_token *b_tokens)
{
	t_big_token	*tmp_b;

	tmp_b = b_tokens;
	while (tmp_b)
	{
	//		printf("value b_token\n");
	//		print_s_tokens(&info->tokens, tmp_b->ind_tok_start, tmp_b->length);
	//		printf("\n");
		if (ft_wash_btoken(info, tmp_b))
			return (2147483647);
		ft_launch_cmd(info, tmp_b);
		if (tmp_b->sibling)
			tmp_b = tmp_b->sibling;
		else
			break;
		info->nb_cmd++;
	}
//		printf("allo\n");
	return (0);
}

int	ft_find_cmd(t_info *info)
{
	t_big_token	*b_tokens;
	t_big_token	*tmp_b;

	b_tokens = info->parse;
	tmp_b = NULL;
	if (!b_tokens)
	{
		printf("pas de b_tokens dans ft_find_cmd\n");
		return (1);
	}
/*	t_token	*tokens;
	tokens = info->tokens;
	printf("valeur des tokens au debut :"); 
	while (tokens)
	{
		printf(" (%d)'%s'", tokens->index, tokens->value);
		tokens = tokens->next;
	}
	printf("\n");*/
	while (b_tokens)
	{
		if (pipe(info->pdes) == -1)
			return (ft_error(5, info, NULL));
		if (ft_deeper_bt(b_tokens, &tmp_b) == -1)
		{
			printf("pb de deeper_bt\n");
			return (1);
		}
//		printf("tmp_b->type = %d\n", tmp_b->type);
//		printf("value\n");
//		print_s_tokens(&info->tokens, tmp_b->ind_tok_start, tmp_b->length);
//		printf("\n");
//		printf("type = %d\n", b_tokens->type);
		if ((!b_tokens->child && !b_tokens->parent))
		{
			if (ft_wash_btoken(info, b_tokens))
				return (2147483647);
			ft_launch_cmd(info, b_tokens);
		}
		else if (tmp_b->parent)
		{
			while (tmp_b->parent)
			{
				if (tmp_b->sibling)
					if (ft_launch_sibling(info, tmp_b) == 2147483647)
						return (2147483647);
				if (info->nb_cmd)
				{
					while (info->nb_cmd)
					{
						info->nb_cmd--;
						printf("je wait une commande\n");
						wait(NULL);
					}
				}
				tmp_b = tmp_b->parent;
			}
		}
		if ((b_tokens->type == TOK_LEFT_AND && info->status != 0)
				|| (b_tokens->type == TOK_LEFT_OR && !info->status))
			break ;
		b_tokens = b_tokens->sibling;
	}
	return (0);
}
