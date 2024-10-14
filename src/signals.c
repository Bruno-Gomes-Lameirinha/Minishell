/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira < livieira@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 21:56:44 by livieira          #+#    #+#             */
/*   Updated: 2024/10/09 23:35:00 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	sigint_handler(int signum)
{
	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);   
		rl_on_new_line();                
		rl_replace_line("", 0);          
		rl_redisplay();                  
	}
}

void	handle_eof(char	*prompt, t_token **lexeme)
{
	rl_replace_line("", 0);              
	write(STDOUT_FILENO, "exit\n", 5);
	free(prompt);
	free(lexeme); 
	exit(0);                             
}

void	setup_signal_handlers(void)
{
	struct sigaction sa;

	sa.sa_handler = sigint_handler;
	sa.sa_flags = SA_RESTART;            
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;            
	sigaction(SIGQUIT, &sa, NULL);
}
