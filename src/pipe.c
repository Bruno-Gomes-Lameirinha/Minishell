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
		ft_free_ast(root->head);
		exit(update_status_error(-1));
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
		ft_free_ast(root->head);
		exit(update_status_error(-1));
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(left_pid, NULL, 0);
	pid_last_exit_status(right_pid);
}
