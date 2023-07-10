/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ealgar-c <ealgar-c@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 14:35:47 by ealgar-c          #+#    #+#             */
/*   Updated: 2023/07/10 15:08:01 by ealgar-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/pipex_bonus.h"

static char	*get_useful_path(char *cmd, char **envp)
{
	int		i;
	char	*path;
	char	**all_paths;

	i = 0;
	while (ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	all_paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (all_paths[i])
	{
		path = ft_strjoin(ft_strjoin(all_paths[i], "/"), cmd);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	i = 0;
	while (all_paths[i])
		free(all_paths[i++]);
	free(all_paths);
	return (NULL);
}

void	execute_process(char *cmd, char **envp)
{
	char	*path;
	char	**cmd_splitted;
	int		i;

	i = 0;
	cmd_splitted = ft_split(cmd, ' ');
	path = get_useful_path(cmd_splitted[0], envp);
	if (path == NULL)
	{
		while (cmd_splitted[i])
			free(cmd_splitted[i++]);
		free(cmd_splitted);
		exit(EXIT_FAILURE);
	}
	execve(path, cmd_splitted, envp);
}

void	error(int errnb)
{
	if (errnb == 0)
	{
		ft_printf("Numero de argumentos incorrecto\n");
		ft_printf("Llama a la función de la forma:");
		ft_printf(" ./pipex_bonus file 1 cmd1 cmd2 ... cmdn file2\n");
	}
	else if (errnb == 1)
		ft_printf("Error de apertura de archivos\n");
	else if (errnb == 2)
		ft_printf("Error en un pipe\n");
	else if (errnb == 3)
		ft_printf("Error en un fork\n");
	exit(EXIT_FAILURE);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}
