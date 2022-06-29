#include "../include/minishell.h"

char	*ft_noquote_line(char *line)
{
	char	*new;
	int		i;
	int		j;

	i = -1;
	j = 0;
	new = NULL;
	if (!line)
		return (NULL);
	if (ft_strlen(line) == ft_strlen_nq(line))
		return (line);
	new = ft_calloc(sizeof(char), ft_strlen_nq(line) + 1);
	if (!new)
		return (NULL);
	while (line[++i])
	{
		if (line[i] == '\'')
			while (line[++i] != '\'')
				new[j++] = line[i];
		if (line[i] == '\"')
			while (line[++i] != '\"')
				new[j++] = line[i];
		if (line[i] != '\'' && line[i] != '\"')
			new[j++] = line[i];
	}
	free(line);
	return (new);
}

int	ft_expand_line(char **str, int *i, t_info *info)
{
	char	*tmp[4];
	int		length;

	length = *i;
	tmp[0] = ft_substr(*str, 0, *i - 1);
	if (!tmp[0])
		return (1);
	if (str[0][length])
		while (str[0][length] && str[0][length] != '\"' && str[0][length] != '\'')
			length++;
	tmp[3] = ft_substr(*str, *i, length - *i);
	if (!tmp[4])
		return (1);
	tmp[1] = ft_expanded_value(info, tmp[3]);
	if (!tmp[1])
		return (1);
	tmp[2] = ft_substr(*str, length, ft_strlen(*str));
	if (!tmp[2])
		return (1);
	free(*str);
	*str = ft_strjoiiin_free(tmp[0], tmp[1], tmp[2], 4);
	*i += length - 1;
	return (0);
}

char	*ft_expand_l(char *str, t_info *info)
{
	int		i;
	int		t;

	i = 0;
	t = 0;
	while (str[i])
	{
		ft_type(str[i], &t);
		if (str[i] == '$' && t < 2)
		{
			i++;
			if (ft_expand_line(&str, &i, info))
				return (NULL);
			if ((size_t)i > ft_strlen(str))
				break ;
		}
		else
			i++;
	}
	return (str);
}
