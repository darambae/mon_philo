/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabae <dabae@student.42perpignan.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:48:17 by dabae             #+#    #+#             */
/*   Updated: 2024/04/22 16:23:23 by dabae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	change_state(t_philo *philo, int state)
{
	pthread_mutex_lock(&philo->lock);
	if (state == DEAD)
		philo->param->stop = 1;
	else if (state == EAT)
	{
		philo->time_last_meal = get_time();
		philo->time_limit_to_death = philo->time_last_meal \
			+ philo->param->time_to_die;
	}
	philo->state = state;
	pthread_mutex_unlock(&philo->lock);
}

/*By taking forks, start to eat*/
void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	pthread_mutex_lock(philo->left_fork);
	print(philo, " has taken both forks");
}

/*by putting down forks, stop eating*/
void	put_down_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->lock);
	philo->num_eat++;
	if (philo->num_eat == philo->param->num_must_eat)
	{
		philo->state = FULL;
		philo->param->num_full++;
	}
	philo->time_last_meal = get_time();
	ft_usleep(philo->param->time_to_eat);
	pthread_mutex_unlock(&philo->lock);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}
