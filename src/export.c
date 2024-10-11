/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira < livieira@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 12:02:24 by livieira          #+#    #+#             */
/*   Updated: 2024/10/11 15:12:56 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
	if (!printed)
	{
		update_status_error(1);
		return ;
	}
	while (print_smallest_unprinted(env, size, printed))
		;
	free(printed);
}

int count_tokens(t_ast_node *command)
{
    int			count;
    t_ast_node	*current;

	count = 0;
	current = command;
    while (current != NULL)
    {
        count++;
        current = current->right;
    }
    return count;
}

char    **convert_tokens_to_args(t_ast_node *command)
{
    char		**args;
    int			i;
    t_ast_node	*current;
	int			num_tokens; 
	
    i = 0;
    current = command;
	num_tokens = count_tokens(command);
    args = malloc(sizeof(char *) * (num_tokens + 1));
    if (!args)
        return NULL;
    
    while (current)
    {
        args[i++] = current->value;
        current = current->right;
    }
    args[i] = NULL;
    return args;
}

int	ft_export_command(t_ast_node *command)
{
	int		i;
	char	**args;
	char	*equal_sign;
	char	*key;
	int		status;

	i = 0;
	status = 0;
	args = convert_tokens_to_args(command);
	if (!args[1])
	{
		ft_print_env_sort();
		update_status_error(0);
		return set_exit_status(status);
	}
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
	update_status_error(0);
	return (set_exit_status(status));
}
