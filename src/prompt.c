/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 18:33:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/08/28 17:11:33 by bgomes-l         ###   ########.fr       */
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
