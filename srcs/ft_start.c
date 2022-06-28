#include "../include/minishell.h"

int	ft_keep_history(char *str)
{
	int	i;

	i = 0;
	if (str)
		while (*str && *str == '\n')
			str++;
	if (str)
		while (*str++)
			i++;
	return (i);
}

int	ft_init_info(t_info *info)
{
	int	err;

	err = main_agent_O(info);
	if (err)
	{
		info->status = err;
		return (ft_free_all(info, NULL), 1);
	}
	return (0);
}

char	*ft_rdline_word(t_info *info)
{
	char		*word;
	char		*tmp;

	word = NULL;
	tmp = ft_get_env_value(info, "HOME");
	word = getcwd(word, 0);
	if (!word)
		return (NULL);
	if (tmp && !strncmp(tmp, word, ft_strlen(tmp)))
	{
		word = ft_substr_free(word, ft_strlen(tmp), ft_strlen(word)
				- ft_strlen(tmp));
		word = ft_strjoin_free("minishell:~", word, 2);
	}
	else
		word = ft_strjoin_free("minishell:", word, 2);
	word = ft_strjoin_free(word, "$ ", 1);
	return (word);
}

int	ft_init_first(t_info *info, char **envp)
{
	info->nb_cmd = 0;
	info->status = 0;
	info->rdline = NULL;
	info->parse = NULL;
	info->old_tokens = NULL;
	info->tokens = NULL;
	info->pid = NULL;
	init_tok_type_tab(&(info->tok_type_tab));
	if (ft_init_env(info, envp))
		return (ft_putstr_error("Error create env\n"));
	info->home = ft_strdup(ft_get_env_value(info, "HOME"));
	signal(SIGINT, &ft_signal);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}
