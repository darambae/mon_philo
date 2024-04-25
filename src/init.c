/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabae <dabae@student.42perpignan.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 14:42:55 by dabae             #+#    #+#             */
/*   Updated: 2024/04/22 16:22:39 by dabae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

/*allocating memory for forks, philosophers and thread in parameters*/
static int	allocate_param(t_param *param)
{
	int	i;

	i = -1;
	param->tids = malloc(sizeof(pthread_t) * param->num_philo);
	if (!param->tids)
		return (1);
	param->forks = malloc(sizeof(pthread_mutex_t) * param->num_philo);
	if (!param->forks)
		return (1);
	param->philo = malloc(sizeof(t_philo) * param->num_philo);
	if (!param->philo)
		return (1);
	while (++i < param->num_philo)
	{
		pthread_mutex_init(&param->philo[i].lock, NULL);
		pthread_mutex_init(&param->forks[i], NULL);
	}
	return (0);
}

/*Initializing the parameters*/
void	init_param(t_param *param, char **args)
{
	if (is_digit(args) && is_positive(args))
	{
		pthread_mutex_init(&param->print, NULL);
		pthread_mutex_init(&param->lock, NULL);
		param->stop = 0;
		param->num_full = 0;
		param->num_philo = (int) ft_atoi(args[0]);
		param->time_to_die = (uint64_t) ft_atoi(args[1]);
		param->time_to_eat = (uint64_t) ft_atoi(args[2]);
		param->time_to_sleep = (uint64_t) ft_atoi(args[3]);
		if (args[4] && ft_atoi(args[4]) > 0)
			param->num_must_eat = (int) ft_atoi(args[4]);
		else
			param->num_must_eat = -1;
		if (allocate_param(param) == 1)
			ft_exit(param, 1, "Failed allocation of parameters");
	}
	else
	{
		free(param);
		printf("ERROR : Double check arguments\n");
		exit(1);
	}
}

/*Initializing philosophers*/
int	init_philo(t_param *param)
{
	int			i;

	i = -1;
	while (++i < param->num_philo)
	{
		param->philo[i].id = i;
		param->philo[i].param = param;
		param->philo[i].num_eat = 0;
		param->philo[i].state = -1;
		param->philo[i].time_last_meal = 0;
		param->philo[i].time_limit_to_death = get_time() + param->time_to_die;
		param->philo[i].left_fork = &param->forks[i];
		if (param->num_philo == 1)
			param->philo[0].right_fork = NULL;
		else
		{
			if (i != 0)
				param->philo[i].right_fork = &param->forks[i - 1];
			else
				param->philo[i].right_fork = &param->forks[
					param->num_philo - 1];
		}
	}
	return (0);
}
