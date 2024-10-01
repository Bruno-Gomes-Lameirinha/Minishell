/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira < livieira@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 12:02:24 by livieira          #+#    #+#             */
/*   Updated: 2024/10/01 19:42:46 by livieira         ###   ########.fr       */
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
//É a função principal que implementa o comando export
int	ft_export_command(t_token *tokens)
{
	int		i;
	char	**args;
	char	*equal_sign;
	char	*key;
	int		status;

	i = 0;
	status = 0;
	args = tokens;
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
