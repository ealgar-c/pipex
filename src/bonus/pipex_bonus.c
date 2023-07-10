/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ealgar-c <ealgar-c@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 13:37:23 by ealgar-c          #+#    #+#             */
/*   Updated: 2023/07/10 14:48:03 by ealgar-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/pipex_bonus.h"

void	parent(int *fd)
{
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	waitpid(-1, NULL, 0);
}

void	do_childs(char *cmd, char **envp)
{
	pid_t	pid;
	int		fd[2];

	if (pipe(fd) == -1)
		error(2);
	pid = fork();
	if (pid == -1)
		error(3);
	else if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		execute_process(cmd, envp);
	}
	else
		parent(fd);
}

int	open_doc(char *file, char mode)
{
	int	fd;

	if (mode == 'w')
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	else if (mode == 'r')
		fd = open(file, O_RDONLY, 0777);
	else
		fd = 0;
	if (fd < 0)
		error(1);
	return (fd);
}

void	here_doc(char *limit, int ac)
{
	pid_t	here_doc;
	int		fd[2];
	char	*str;

	if (ac != 6)
		error(0);
	if (pipe(fd) == -1)
		error(2);
	here_doc = fork();
	if (here_doc == -1)
		error(3);
	else if (here_doc == 0)
	{
		close(fd[0]);
		str = get_next_line(0);
		while (ft_strcmp(str, limit) != 10)
		{
			ft_putstr_fd(str, fd[1]);
			str = get_next_line(0);
		}
		exit(EXIT_SUCCESS);
	}
	parent(fd);
}

int	main(int ac, char **av, char **envp)
{
	int	i;
	int	inf_fd;
	int	outf_fd;

	if (ac < 5)
		error(0);
	if (ft_strncmp(av[1], "here_doc", 8) == 0)
	{
		i = 2;
		outf_fd = open_doc(av[ac - 1], 'w');
		here_doc(av[2], ac);
	}
	else
	{
		i = 1;
		inf_fd = open_doc(av[1], 'r');
		outf_fd = open_doc(av[ac - 1], 'w');
		dup2(inf_fd, STDIN_FILENO);
	}
	while (i < ac - 2)
		do_childs(av[++i], envp);
	dup2(outf_fd, STDOUT_FILENO);
	execute_process(av[ac - 2], envp);
	return (0);
}
