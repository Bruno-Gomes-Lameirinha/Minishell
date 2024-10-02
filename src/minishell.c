/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira < livieira@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 18:33:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/09/24 15:35:58 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_strcpy(char *dst, const char *src)
{
	int i;

	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

void	ft_free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
		free(split[i++]);
	free(split);
}

char *ft_get_prompt(void)
{
	char cwd[1024];
	char *home_dir = getenv("HOME");
	char *relative_cwd;
	size_t cwd_len;
	size_t prompt_len;
	char *prompt;

	if (getcwd(cwd, sizeof(cwd)) != NULL) 
	{
		relative_cwd = cwd;

		if (home_dir && strncmp(cwd, home_dir, strlen(home_dir)) == 0) 
		{
			relative_cwd = cwd + strlen(home_dir);
			if (*relative_cwd == '/')
				relative_cwd++;
		}
		cwd_len = strlen(relative_cwd) + 1;
		prompt_len = strlen("Minishell $ ");
		prompt = malloc(cwd_len + prompt_len + 3);
		if (prompt == NULL) 
		{
			perror("Failed to allocate memory for prompt");
			exit(EXIT_FAILURE);
		}
		ft_strcpy(prompt, "Minishell ");
		if (relative_cwd == cwd) 
			ft_strlcat(prompt, cwd, cwd_len + prompt_len + 3);
		else 
		{
			ft_strlcat(prompt, "~", cwd_len + prompt_len + 3);
			ft_strlcat(prompt, "/", cwd_len + prompt_len + 3);
			ft_strlcat(prompt, relative_cwd, cwd_len + prompt_len + 3); 
		}
		ft_strlcat(prompt, "$ ", cwd_len + prompt_len + 3); 
		return prompt;
	} 
	else 
	{
		perror("getcwd() error");
		exit(EXIT_FAILURE);
	}
}


int	main(void)
{
	char	*input;
	t_env	*env;
	t_token	**lexeme;
	t_ast_node *ast;
	char	*prompt;
	
	env = (t_env*)malloc(sizeof(t_env));
	lexeme = (t_token**)malloc(sizeof(t_token*));
	if (!lexeme)
	{
		perror("Failed to allocate memory for lexeme");
		exit(EXIT_FAILURE);
	}
	*lexeme = NULL;
	while (1)
	{
		prompt = ft_get_prompt();
		input = readline(prompt);
		add_history(input);
		if (input)
		{
			ft_tokenize(input, lexeme);
			//ft_print_linked_list(lexeme);
			ast = ft_build_ast(lexeme);
			ft_collect_heredocs(ast);
			ft_execute_ast(ast);
			ft_clean_token_list(lexeme);
			free(input);
			free(prompt);
			ft_free_ast(ast);
		}
	}
	free(lexeme);
	free(env);
	return (0);
}

int	get_exit_status(int exit_status)
{
	return ((exit_status & 0xff00) >> 8);
}