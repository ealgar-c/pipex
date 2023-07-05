/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ealgar-c <ealgar-c@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 15:34:06 by ealgar-c          #+#    #+#             */
/*   Updated: 2023/07/05 13:15:23 by ealgar-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

char	*get_useful_path(char *cmd, char **envp)
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

void	execute_process(char *cmd, char **envp, int out_fd)
{
	char	*path;
	char	**cmd_splitted;
	int		i;

	i = 0;
	cmd_splitted = ft_split(cmd, ' ');
	path = get_useful_path(cmd_splitted[0], envp);
	if (path == NULL)
	{
		dup2(STDOUT_FILENO, out_fd);
		ft_printf("Error: Command %s not found\n", cmd_splitted[0]);
		while (cmd_splitted[i])
			free(cmd_splitted[i++]);
		free(cmd_splitted);
		exit(EXIT_FAILURE);
	}
	execve(path, cmd_splitted, envp);
}

void	child_process(char **argv, int *pipe_fd, char **envp)
{
	int		infile_fd;

	infile_fd = open(argv[1], O_RDONLY, 0777);
	if (infile_fd == -1)
	{
		ft_printf("Error en el fichero de entrada\n");
		exit(EXIT_FAILURE);
	}
	dup2(infile_fd, STDIN_FILENO);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[0]);
	execute_process(argv[2], envp, pipe_fd[1]);
}

void	parent_process(char **argv, int *pipe_fd, char **envp)
{
	int		outfile_fd;

	outfile_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	dup2(outfile_fd, STDOUT_FILENO);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[1]);
	execute_process(argv[3], envp, outfile_fd);
}

int	main(int argc, char **argv, char **envp)
{
	pid_t	ppid;
	int		pipe_fd[2];

	if (argc != 5)
	{
		ft_printf("Numero de argumentos incorrecto");
		exit(EXIT_FAILURE);
	}
	if (pipe(pipe_fd) == -1)
	{
		ft_printf("Error en pipe");
		exit(EXIT_FAILURE);
	}
	ppid = fork();
	if (ppid == -1)
	{
		ft_printf("Error en fork");
		exit(EXIT_FAILURE);
	}
	if (ppid == 0)
		child_process(argv, pipe_fd, envp);
	wait(0);
	parent_process(argv, pipe_fd, envp);
	return (0);
}
