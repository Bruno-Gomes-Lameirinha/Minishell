/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira < livieira@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 18:33:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/10/08 20:24:32 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
void	ft_expand_variables(t_token	**lexeme)
{
	t_token	*current;
	char	*var_key;
	char	*var_value;
	char	*x;

	current = *lexeme;

	while (current != NULL)
	{
		if (current->type_token == WORD || current->type_token == DOUBLE_QUOTES)
		{
			x = ft_strchr(current->token_node, '$');
			if (x != NULL)
			{
				var_key = ft_substr(current->token_node, ft_findchr(current->token_node, '$'), ft_strlen_var(x+1));
				var_value = getenv(var_key);
				if (var_value != NULL)
				{
					free(var_key);
					var_key = ft_strjoin(var_value, (x + (ft_strlen_var(x + 1) +1 )));
					free(current->token_node);
					current->token_node = ft_strdup(var_key);
					var_value = NULL;
				}
			}
		}
		current = current->next;
	}
}

*/

char *ft_expand_variables_input(char *input)
{
	char	*start;
	char	*var_key;
	char	*var_value;
	char 	*expanded_input;
	char 	*temp;

	start = input;
	expanded_input = ft_strdup(""); // Inicializa a string resultante como vazia

	while (*input)
	{
		// Avança até encontrar '$' ou '\''
		while (*input != '$' && *input != '\'' && *input != '\0')
			input++;
		
		// Verifica se encontrou uma aspa simples e pula o conteúdo entre aspas
		if (*input == '\'')
		{
			input++; // Pula a primeira aspa
			while (*input != '\'' && *input != '\0') // Avança até a aspa de fechamento
				input++;
			if (*input == '\'')
				input++; // Pula a aspa de fechamento
			continue; // Continue o loop sem expandir variáveis dentro de aspas simples
		}

		// Verifica se encontrou um '$'
		if (*input == '$')
		{
			// Extrai o texto antes do '$' e adiciona à string expandida
			temp = ft_substr(start, 0, input - start);
			expanded_input = ft_strjoin_free(expanded_input, temp); // Concatenar e liberar temp
			
			// Extrai o nome da variável
			input++; // Pula o $
			var_key = ft_substr(input, 0, ft_strlen_var(input));

			// Obtém o valor da variável de ambiente
			var_value = ft_get_env_value(var_key);

			// Se a variável existir, concatene o valor dela à string expandida
			if (var_value)
    			expanded_input = ft_strjoin_free(expanded_input, var_value);
			else
    			expanded_input = ft_strjoin_free(expanded_input, ""); // Substitui por string vazia

			// Libera a chave e atualiza o input para a próxima parte
			free(var_key);
			
			input += ft_strlen_var(input); // Avança o input após o nome da variável
			start = input; // Atualiza o ponto de início para o próximo processamento
		}
	}

	// Após o loop, adicione o restante da string (se houver) à string expandida
	if (*start != '\0')
	{
		temp = ft_strdup(start);
		expanded_input = ft_strjoin_free(expanded_input, temp);
		free(temp);
	}

	return expanded_input; // Retorna a string expandida completa
}

size_t	ft_strlen_var(const char *s)
{
	int	lenght;

	lenght = 0;
	while (ft_isalnum(*s++) || *s++ == '_')
	{
		lenght ++;
	}
	return (lenght);
}

char *ft_strjoin_free(char *s1, char *s2)
{
	char *result;

	result = ft_strjoin(s1, s2);
	free(s1); // Libera a primeira string
	return result;
}

char *ft_get_env_value(char *key)
{
    char **env;
    char *env_key;
    int i;

    env = *ft_get_env(); // Usa o ambiente clonado
    i = 0;
    while (env[i])
    {
        env_key = get_key(env[i]); // Extrai a chave da variável do formato "key=value"
        if (ft_strcmp(env_key, key) == 0) // Compara a chave com a variável procurada
        {
            return (ft_strchr(env[i], '=') + 1); // Retorna o valor após o '='
        }
        i++;
    }
    return (NULL); // Se não encontrar, retorna NULL
}

// void	ft_expand_variables_input(char	*input)
// {
// 	char	*start;
// 	char	*var_key;
// 	char	*var_value;
// 	char 	*temp;

// 	start = input;
// 	while(*input)
// 	{
// 		while (*input != '$' && *input != '\'')
// 		{
// 			input++;
// 		}
// 		if (*input == '\'')
// 		{
// 			input++;
// 			while(*input != '\'')
// 				input++;
// 			input++;
// 		}
// 		if (*input == '$')
// 		{
// 			temp = ft_substr(start, 0, ft_strlen_start_end(start, input));
// 			var_key = ft_substr(input + 1 , 0, ft_strlen_var(input+1));
// 			var_value = getenv(var_key);
// 			printf("%s", var_value);
// 			printf("%s", temp);
// 			input++;
// 		}
		
// 	}
// }

// size_t	ft_strlen_start_end(const char *start, const char *end)
// {
// 	int	lenght;

// 	lenght = 0;
// 	while (start++ != end)
// 	{
// 		lenght ++;
// 	}
// 	return (lenght);
// }
