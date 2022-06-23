/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_launch_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 10:29:00 by nflan             #+#    #+#             */
/*   Updated: 2022/06/23 17:56:50 by nflan            ###   ########.fr       */
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

/*t_cmd	*ft_convert_bt_cmd(t_info *info, t_big_token *b_tokens)
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
	return (tmp);
}*/

int	ft_check_builtins(t_big_token *b_tokens)
{
	int	len;

	if (!b_tokens->cmd_args || !b_tokens->cmd_args[0])
		return (2);
	len = ft_strlen(b_tokens->cmd_args[0]) + 1;
	if (!ft_strncmp(b_tokens->cmd_args[0], "unset", len))
		return (0);
	else if (!ft_strncmp(b_tokens->cmd_args[0], "export", len))
		return (0);
	else if (!ft_strncmp(b_tokens->cmd_args[0], "cd", len))
		return (0);
	else if (!ft_strncmp(b_tokens->cmd_args[0], "exit", len) || !ft_strncmp(b_tokens->cmd_args[0], "exit ", 5))
		return (0);
	else if (!ft_strncmp(b_tokens->cmd_args[0], "pwd", len))
		return (0);
	else if (!ft_strncmp(b_tokens->cmd_args[0], "env", len))
		return (0);
	else if (!ft_strncmp(b_tokens->cmd_args[0], "echo", len))
		return (0);
	return (1);
}

int	ft_builtins(t_info *info, t_big_token *b_tokens)
{
	int	len;

	if (!b_tokens->cmd_args || !b_tokens->cmd_args[0])
		return (2);
	len = ft_strlen(b_tokens->cmd_args[0]) + 1;
	if (!ft_strncmp(b_tokens->cmd_args[0], "unset", len))
	{
		if (!ft_unset(info, b_tokens))
			return (1);
		else
			return (0);
	}
	else if (!ft_strncmp(b_tokens->cmd_args[0], "export", len))
		return (ft_export(info, b_tokens));
	else if (!ft_strncmp(b_tokens->cmd_args[0], "cd", len))
		return (ft_cd(info, b_tokens));
	else if (!ft_strncmp(b_tokens->cmd_args[0], "exit", len) || !ft_strncmp(b_tokens->cmd_args[0], "exit ", 5))
		return (ft_exit(info, b_tokens));
	else if (!ft_strncmp(b_tokens->cmd_args[0], "pwd", len))
		return (ft_pwd());
	else if (!ft_strncmp(b_tokens->cmd_args[0], "env", len))
		return (ft_env(info));
	else if (!ft_strncmp(b_tokens->cmd_args[0], "echo", len))
		return (ft_echo(info, b_tokens));
	return (1);
}

int	ft_exit_cmd(t_info *info, char *str, int err)
{
	info->status = err;
	if (err == 1 && str)
		ft_putstr_frror(str, ": command not found\n", 0);
	if (info)
		ft_free_all(info, info->env);
	rl_clear_history();
	exit (info->status);
}

void	ft_close_cmd(t_info *info, t_big_token *b_tokens, pid_t child)
{
	if (!info->nb_cmd) 
	{
//		printf("je ferme premier pipex -> value b_token: ");
		close(info->pdes[1]);
	}
	else if (b_tokens->type == TOK_LEFT_PIPE)
	{
//		printf("type = %d\n", b_tokens->type);
//		printf("je ferme pipe to pipe -> value b_token: ");
//		if (info->pdes[0] != 0)
		close(info->pdes[0]);
		info->pdes[0] = info->tmp[0];
//		if (info->pdes[1] != 1 && info->pdes[1] != 2)
		close(info->pdes[1]);
	}
	else
	{
//		printf("je ferme le dernier pipe -> value b_token: ");
		waitpid(child, &child, 0);
	//	if (info->pdes[0] != 0)
		close(info->pdes[1]);
		close(info->pdes[0]);
	}
	if (WIFEXITED(child))
		info->status = WEXITSTATUS(child);
}

int	ft_lead_fd(t_info *info, t_big_token *b_tokens)
{
	if (info->nb_cmd && b_tokens->type == TOK_LEFT_PIPE)
	{
//		printf("j'ai fait un pipe tmp\n");
		if (pipe(info->tmp) == -1)
			return (ft_error(5, info, NULL));
		info->pdes[1] = info->tmp[1];
	}
	if (b_tokens->fdin != 0)
	{
//		printf("je suis utile pour rien !\n");
		info->pdes[0] = b_tokens->fdin;
	}
	if (b_tokens->fdout != 1)
	{
//		printf("je suis utile pour rien !\n");
		info->pdes[1] = b_tokens->fdout;
	}
	return (0);
}

int	ft_fork_par(t_info *info, t_big_token *b_tokens)
{
	pid_t	pid;

	pid = -1;
//	printf("fork_par\n");
	pid = fork();
	ft_manage_sig(0);
	if ((int) pid == -1)
		return (ft_error(2, info, NULL));
	else if ((int) pid == 0)
	{
		ft_manage_sig(1);
		rec_exec(info, &(b_tokens)->child, 0);
		ft_exit_cmd(info, NULL, 0);
	}
	waitpid(pid, &pid, 0);
	ft_manage_sig(2);
	if (WIFEXITED(pid))
		info->status = WEXITSTATUS(pid);
	return (info->status);
}

int	ft_launch_cmd_pipex(t_info *info, t_big_token *b_tokens, int pid)
{
	pid = -1;
	if (ft_lead_fd(info, b_tokens))
		return (ft_putstr_error("FD problem\n"));
	b_tokens->envp = ft_env_to_tab(info->env);
	pid = fork();
	ft_manage_sig(0);
	if ((int) pid == -1)
		return (ft_error(2, info, NULL));
	else if ((int) pid == 0)
	{
		ft_manage_sig(1);
		if (ft_pipex(info, b_tokens))
			return (ft_free_cmd(b_tokens), 1);
		ft_exit_cmd(info, NULL, 0);
	}
	ft_manage_sig(2);
	ft_close_cmd(info, b_tokens, pid);
	return (info->status);
}

int	ft_do_solo(t_info *info, t_big_token *b_tokens)
{
	pid_t	pid;

	pid = -1;
	pid = fork();
	ft_manage_sig(0);
	if ((int) pid == -1)
		return (ft_error(2, info, NULL));
	else if ((int) pid == 0)
	{
		ft_manage_sig(1);
		dup2(b_tokens->fdin, STDIN_FILENO);
		dup2(b_tokens->fdout, STDOUT_FILENO);
//		printf("b_tokens->fdout[b_tokens->rd_inouthd[1]] = %d\n", b_tokens->fdout[b_tokens->rd_inouthd[1 - 1]]);
		if (ft_command(info, b_tokens))
			ft_exit_cmd(info, b_tokens->cmd_args[0], 1);
		else
			if (execve(b_tokens->cmd_args[0], b_tokens->cmd_args, b_tokens->envp) == -1)
				exit (ft_error(4, info, b_tokens));
	}
	waitpid(pid, &pid, 0);
	ft_manage_sig(2);
	if (WIFEXITED(pid))
		info->status = WEXITSTATUS(pid);
	return (info->status);
}

int	ft_launch_cmd(t_info *info, t_big_token *b_tokens)
{
	if (b_tokens->par == 1)
		return (ft_fork_par(info, b_tokens));
	b_tokens->envp = ft_env_to_tab(info->env);
	if (!ft_check_builtins(b_tokens))
		info->status = ft_builtins(info, b_tokens);
	else if (ft_check_builtins(b_tokens) == 1)
		ft_do_solo(info, b_tokens);
	return (info->status);
}
