/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira < livieira@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 21:56:44 by livieira          #+#    #+#             */
/*   Updated: 2024/10/09 23:34:17 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Função para lidar com SIGINT (Ctrl+C)
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

// Função para lidar com a saída do shell (Ctrl+D)
void	handle_eof(void)
{
	rl_replace_line("", 0);              
	write(STDOUT_FILENO, "exit\n", 5);    
	exit(0);                             
}

// Função para configurar os sinais no modo interativo
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
