/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop_life.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabae <dabae@student.42perpignan.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 16:26:05 by dabae             #+#    #+#             */
/*   Updated: 2024/04/25 13:43:21 by dabae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

/*check if any philopher died*/
void	*anyone_dead(void *philo)
{
	t_philo		*phi;
	uint64_t	current_time;

	phi = (t_philo *)philo;
	while (1)
	{
		pthread_mutex_lock(&phi->lock);
		if (phi->state == DEAD)
		{
			pthread_mutex_unlock(&phi->lock);
			break ;
		}
		pthread_mutex_unlock(&phi->lock);
		pthread_mutex_lock(&phi->param->stop_lock);
		if (phi->param->stop == 1)
		{
			pthread_mutex_unlock(&phi->param->stop_lock);
			break ;
		}
		pthread_mutex_unlock(&phi->param->stop_lock);
		current_time = get_time();
		if (current_time >= phi->time_limit_to_death && phi->state != EAT)
		{
			print(phi, " died");
			phi->state = DEAD;
			pthread_mutex_lock(&phi->param->stop_lock);
			phi->param->stop = 1;
			pthread_mutex_unlock(&phi->param->stop_lock);
			pthread_mutex_unlock(&phi->lock);
			break ;
		}
		pthread_mutex_unlock(&phi->lock);
		ft_usleep(1000);
	}
	return ((void *)0);
}

void	*is_everyone_full(void *param)
{
	t_param	*data;

	data = (t_param *)param;
	pthread_mutex_lock(&data->lock);
	while (data->stop == 0 && data->num_full < data->num_philo)
	{
		pthread_mutex_unlock(&data->lock);
		ft_usleep(1000);
		pthread_mutex_lock(&data->lock);
	}
	if (data->stop == 0 && data->num_full >= data->num_philo)
	{
		pthread_mutex_lock(&data->print);
		printf("Everyone has eaten as many times as %d\n", data->num_must_eat);
		data->stop = 1;
		pthread_mutex_unlock(&data->print);
		pthread_mutex_unlock(&data->lock);
	}
	pthread_mutex_unlock(&data->lock);
	return (NULL);
}