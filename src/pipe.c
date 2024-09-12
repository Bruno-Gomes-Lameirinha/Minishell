#include "../include/minishell.h"

void	init_pipe(t_pipex *pipex)
{
	pipex->parent_env = malloc(sizeof(char*) * 3); 
	if (pipex->parent_env == NULL)
	{
		perror("Failed to allocate memory");
		exit(EXIT_FAILURE);
	}
	char *shell_var = getenv("SHELL");
	if (shell_var == NULL)
		shell_var = "/bin/bash";
	pipex->parent_env[0] = ft_strjoin("SHELL=", shell_var);
	char *path_var = getenv("PATH");
	if (path_var == NULL)
		path_var = "/usr/local/bin:/usr/bin:/bin";
	pipex->parent_env[1] = ft_strjoin("PATH=", path_var);
	pipex->parent_env[2] = NULL; // Finalizando a lista com NULL
	pipex->parent_argv = malloc(sizeof(char*) * 2);
	if (pipex->parent_argv == NULL)
	{
		perror("Failed to allocate memory");
		exit(EXIT_FAILURE);
	}
	pipex->parent_argv[0] = strdup("./minishell"); // Nome do shell ou comando executado
	pipex->parent_argv[1] = NULL;

	// Caminho para os executáveis
	pipex->path = ft_split(path_var, ':'); // Divide o PATH em múltiplos diretórios
	if (pipex->path == NULL)
	{
		perror("Failed to allocate memory for path");
		exit(EXIT_FAILURE);
	}
}

// void ft_execute_command_ast_pipe(t_ast_node *command_node, t_pipex *pipex)
// {
// 	t_ast_node *current;
// 	char	*executable;
// 	int		n_args;
// 	int i;

// 	i = 0;
// 	n_args = 0;
// 	current = command_node;
// 	while(current)
// 	{
// 		n_args++;
// 		current = current->right;
// 	}
// 	pipex->argv_childs = malloc(sizeof(char *) * (n_args + 1));
// 	if (!pipex->argv_childs)
// 	{
// 		perror("malloc");
// 		exit(EXIT_FAILURE);
// 	}
// 	current = command_node;
// 	while (current)
// 	{
// 		pipex->argv_childs[i] = ft_strdup(current->value);
// 		i++;
// 		current = current->right;
// 	}
// 	pipex->argv_childs[i] = NULL;
// 	executable = ft_search_executable_ast(pipex->argv_childs[0]);
// 	execve(executable, pipex->argv_childs, pipex->parent_env);
// 	exit(EXIT_SUCCESS);
// }