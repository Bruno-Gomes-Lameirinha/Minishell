/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enviroment.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira < livieira@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 20:43:45 by livieira          #+#    #+#             */
/*   Updated: 2024/10/01 19:02:53 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	***ft_get_env(void)
{
	static char	**env;
	
	return (&env);
}

//"clona" o ambiente atual para que o minishell possa fazer modificações sem afetar o processo pai.
void	ft_copy_env(void)
{
	char	**copy;
	int		i;

	i = 0;
	while (__environ[i])
		i++;
	*ft_get_env() = malloc(sizeof(char *) * (i + 1));
	copy = *ft_get_env();
	i = 0;
	while (__environ[i])
	{
		copy[i] = ft_strdup(__environ[i]);
		i++;
	}
	copy[i] = NULL;
	__environ = copy;
}

//adiciona uma nova variável de ambiente ao conjunto de variáveis gerenciado pelo minishell.
void	ft_add_env(char *string)
{
	char	**new_var;
	char	**env;
	int		i;

	i = 0;
	env = *ft_get_env();
	while (env[i])
		i++;
	new_var = ft_calloc(i + 2, sizeof(char *));
	i = 0;
	while (env[i])
	{
		new_var[i] = env[i];
		i++;
	}
	new_var[i++] = ft_strdup(string);
	new_var[i] = NULL;
	*ft_get_env() = new_var;
	__environ = new_var;
}

// implementa o comando env no minishell.
void	ft_env_command(t_token **cmd_tokens)
{
    int     i;
    char    **env;
    t_token *current_token;
    
    env = ft_get_env();
    current_token = *cmd_tokens;
	if (current_token->next)
		return (write(STDERR_FILENO, "env: too many arguments\n", 24));
    if (strcmp(current_token->token_node, "env") == 0)
	{
		i = 0;
		while (env[i])
		{
			ft_putstr_fd(env[i], STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
			i++;
		}
		return (SUCCESS);
	}
        
}
