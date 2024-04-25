/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabae <dabae@student.42perpignan.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 10:42:53 by dabae             #+#    #+#             */
/*   Updated: 2024/04/25 13:45:31 by dabae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	print(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->param->print);
	printf("At %llu, Philosopher no.%d %s.\n", get_time(), philo->id, str);
	pthread_mutex_unlock(&philo->param->print);
}

void	cleanup(t_param *param)
{
	if (param->tids)
		free(param->tids);
	if (param->forks)
		free(param->forks);
	if (param->philo)
		free(param->philo);
	free(param);
}

void	ft_exit(t_param *param, int err, char *msg)
{
	int	i;

	i = -1;
	if (!param)
		return ;
	while (++i < param->num_philo)
	{
		pthread_mutex_destroy(&param->forks[i]);
		pthread_mutex_destroy(&param->philo[i].lock);
	}
	pthread_mutex_destroy(&param->print);
	pthread_mutex_destroy(&param->lock);
	pthread_mutex_destroy(&param->stop_lock);
	cleanup(param);
	if (err)
	{
		printf("%s\n", msg);	
		exit(1);
	}
	else
		exit(0);
}

static void	only_one_philo(t_param *param)
{
	pthread_create(&param->tids[0], NULL, &anyone_dead, &param->philo[0]);
	pthread_join(param->tids[0], NULL);
	while (param->stop == 0)
		ft_usleep(0);
	ft_exit(param, 0, NULL);
}

int	main(int ac, char **av)
{
	t_param	*param;

	param = malloc(sizeof(t_param));
	if (ac == 5 || ac == 6)
	{
		init_param(param, av + 1);
		init_philo(param);
		if (param->num_philo == 1)
			only_one_philo(param);
		else
		{
			if (life_cycle(param) == 1)
				ft_exit(param, 1, "Life cycle stopped");
			if (param != NULL)
				ft_exit(param, 0, NULL);
		}
	}
	else
	{
		free(param);
		printf("Insufficient or too many arguments\n");
		return (1);
	}
	return (0);
}
