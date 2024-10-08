#include "../include/minishell.h"

void ft_handle_pipe(t_ast_node *root)
{
	int fd[2];
	pid_t left_pid;
	pid_t right_pid;

	if (pipe(fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	left_pid = fork();
	if (left_pid == -1) 
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (left_pid == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		ft_execute_ast(root->left);
		exit(EXIT_SUCCESS);
	}

	right_pid = fork();
	if (right_pid == -1) 
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (right_pid == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		ft_execute_ast(root->right);
		exit(EXIT_SUCCESS);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, NULL, 0);
}
