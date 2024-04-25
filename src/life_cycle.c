/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_cycle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabae <dabae@student.42perpignan.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 10:25:19 by dabae             #+#    #+#             */
/*   Updated: 2024/04/25 13:41:55 by dabae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static int	eat_phase(t_philo *philo)
{
	// if (philo->param->num_philo == 1)
	// 	return (1);
	take_forks(philo);
	change_state(philo, EAT);
	print(philo, " is eating");
	put_down_forks(philo);
	return (0);
}

static void	sleep_phase(t_philo *philo)
{
	change_state(philo, SLEEP);
	print(philo, " is sleeping");
	ft_usleep(philo->param->time_to_sleep);
}

static void	think_phase(t_philo *philo)
{
	change_state(philo, THINK);
	print(philo, " is thinking");
}

int	check_to_stop(t_philo *philo)
{
	pthread_mutex_lock(&philo->param->stop_lock);
	if (philo->param->stop == 1)
	{
		pthread_mutex_unlock(&philo->param->stop_lock);
		return (0);
	}
	pthread_mutex_unlock(&philo->param->stop_lock);
	pthread_mutex_lock(&philo->lock);
	if (philo->state == DEAD)
	{
		pthread_mutex_unlock(&philo->lock);
		return (0);
	}
	pthread_mutex_unlock(&philo->lock);
	return (1);
}

void	*life_start(void *philo)
{
	t_philo	*phi;

	phi = (t_philo *)philo;
	phi->time_limit_to_death = get_time() + phi->param->time_to_die;
	if (pthread_create(&phi->thread, NULL, &anyone_dead, phi) != 0)
		return ((void *)1);
	while (check_to_stop(phi))
	{
		if (!check_to_stop(phi))
			break ;	
		eat_phase(phi);
		if (!check_to_stop(phi))
			break ;
		sleep_phase(phi);
		if (!check_to_stop(phi))
			break ;
		think_phase(phi);
	}
	if (pthread_join(phi->thread, NULL) != 0)
		return ((void *)1);
	return ((void *)0);
}

int	life_cycle(t_param *param)
{
	pthread_t	thread;
	int			i;
	
	if (param->num_must_eat > 0)
	{
		if (pthread_create(&thread, NULL, &is_everyone_full, param) != 0)
			return (1);
	}
	i = -1;
	while (++i < param->num_philo)
	{
		if (pthread_create(&param->tids[i], NULL, &life_start,
				&param->philo[i]) != 0)
			return (1);
		ft_usleep(1);
	}
	i = -1;
	while (++i < param->num_philo)
	{
		if (pthread_join(param->tids[i], NULL))
			return (1);
	}
	if (param->num_must_eat > 0)
	{
		if (pthread_join(thread, NULL))
			return (1);
	}
	return (0);
}