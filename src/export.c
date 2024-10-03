/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira < livieira@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 12:02:24 by livieira          #+#    #+#             */
/*   Updated: 2024/10/03 00:45:11 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//Formata uma variável de ambiente para a saída, preparando-a para ser exibida no terminal.
void	format_and_print(const char *env_var)
{
	char	*name;
	char	*equal;
	char	*value;
	char	*copy;

	copy = ft_strdup(env_var);
	equal = ft_strchr(copy, '=');
	if (!equal)
		printf("declare -x %s\n", copy);
	else
	{
		*equal = '\0';
		name = copy;
		value = equal + 1;
		printf("declare -x %s=\"%s\"\n", name, value);
	}
	free(copy);
}

// Encontra a variável de ambiente ainda não impressa com o menor valor lexicográfico e a imprime.
int	print_smallest_unprinted(char **env, size_t env_size, char *printed)
{
	int	small_pos;

	small_pos = -1;
	while (env_size-- > 0)
	{
		if (printed[env_size])
			continue ;
		if (small_pos == -1)
			small_pos = env_size;
		else
		{
			if (ft_strcmp(env[small_pos], env[env_size]) > 0)
				small_pos = env_size;
		}
	}
	if (small_pos != -1)
	{
		format_and_print(env[small_pos]);
		printed[small_pos]++;
		return (1);
	}
	return (SUCCESS);
}

//Imprime todas as variáveis de ambiente em ordem alfabética, utilizando as funções anteriores para formatar e encontrar as variáveis.
void	ft_print_env_sort(void)
{
	char	*printed;
	size_t	size;
	char	**env;

	env = *ft_get_env();
	size = 0;
	while (env[size])
		size++;
	printed = ft_calloc(size + 1, sizeof(char));
	while (print_smallest_unprinted(env, size, printed))
		;
	free(printed);
}

int count_tokens(t_token *token_node)
{
    int count = 0;
    t_token *current = token_node;

    // Percorre a lista até o final (quando current == NULL)
    while (current != NULL)
    {
        count++;
        current = current->next;
    }
    return count;
}
char    **convert_tokens_to_args(t_token *token_node)
{
    char    **args;
    int     i;
    t_token *current;

    // Supondo que você saiba o número de tokens ou possa contar eles
    int num_tokens = count_tokens(token_node);
    args = malloc(sizeof(char *) * (num_tokens + 1));
    if (!args)
        return NULL;
    
    current = token_node;
    i = 0;
    while (current)
    {
        args[i++] = current->token_node; // Ou o campo correto da estrutura de t_token
        current = current->next;
    }
    args[i] = NULL; // Terminar a lista com NULL
    return args;
}
//É a função principal que implementa o comando export
int	ft_export_command(t_token *token_node)
{
	int		i;
	char	**args;
	char	*equal_sign;
	char	*key;
	int		status;

	i = 0;
	status = 0;
	args = convert_tokens_to_args(token_node);
	if (!args[1])
		ft_print_env_sort();
	while (args[++i])
	{
		if (!is_valid_identifier(args[i], args[0]) && ++status)
			continue ;
		key = get_key(args[i]);
		equal_sign = ft_strchr(args[i], '=');
		if (equal_sign)
			ft_set_env(args[i], key, equal_sign + 1);
		else
			ft_set_env(args[i], key, NULL);
	}
	return (set_exit_status(status));
}
