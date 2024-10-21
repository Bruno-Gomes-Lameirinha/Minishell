/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira < livieira@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 17:16:26 by livieira          #+#    #+#             */
/*   Updated: 2024/10/08 20:59:05 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void delete_env_key(char *key_to_delete)
{
    int     i;
    int     j;
    char    *env_key;
    char    **new_env;
    char    **current_env;

    current_env = *ft_get_env();
    i = 0;
    while (current_env[i])
        i++;
    new_env = (char **)ft_calloc(i + 1, sizeof(char *)); // Não precisamos de i + 2
    if (!new_env)
    {
        perror("malloc");
        ft_update_status_error(1);
        return;
    }
    i = -1;
    j = -1;
    while (current_env[++i])
    {
        env_key = ft_get_key(current_env[i]);
        if (ft_strcmp(key_to_delete, env_key) == 0)
        {
            // Strings iguais, removemos a variável
            free(current_env[i]); // Liberar a variável removida
        }
        else
        {
            // Mantemos a variável
            new_env[++j] = current_env[i];
        }
        free(env_key); // Liberar env_key após uso
    }
    new_env[++j] = NULL;
    free(current_env);
    *ft_get_env() = new_env; // Atualizar o ponteiro estático
    __environ = new_env;
}

int ft_unset_command(t_ast_node *command)
{
    int     i;
    char    **args;
    int     status;

    i = 0;
    status = 0;
    args = ft_convert_tokens_to_args(command);
    if (!args[1])
    {
        ft_update_status_error(0);
        free(args); // Liberar args
        return (0);
    }
    while (args[++i])
    {
        if (!ft_is_valid_identifier(args[i], args[0]) && ++status)
            continue;
        delete_env_key(args[i]);
    }
    ft_update_status_error(status);
    free(args); // Liberar args após o uso
    return (status);
}
