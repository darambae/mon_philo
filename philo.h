/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabae <dabae@student.42perpignan.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 10:41:34 by dabae             #+#    #+#             */
/*   Updated: 2024/04/22 16:28:38 by dabae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <unistd.h>
# include <sys/time.h>
# include <errno.h>

# define EAT 0
# define SLEEP 1
# define THINK 2
# define FULL 3
# define DEAD 4

struct	s_philo;

typedef struct s_param
{
	int				num_philo;
	int				num_must_eat;
	int				stop;
	int				num_full;
	uint64_t		time_to_die;
	uint64_t		time_to_eat;
	uint64_t		time_to_sleep;

	struct s_philo	*philo;
	pthread_t		*tids;
	pthread_mutex_t	print;
	pthread_mutex_t	*forks;
	pthread_mutex_t	lock;
}				t_param;

typedef struct s_philo
{
	int				id;
	int				state;
	int				num_eat;
	uint64_t		time_last_meal;
	uint64_t		time_limit_to_death;
	pthread_t		thread;

	pthread_mutex_t	lock;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_param			*param;
}					t_philo;

int			is_digit(char **args);
int			is_positive(char **str);
long		ft_atoi(char *str);
void		ft_usleep(uint64_t elapsed);
void		ft_exit(t_param *param, int err, char *msg);
void		cleanup(t_param *param);
uint64_t	get_time(void);

void		init_param(t_param *param, char **args);
int			init_philo(t_param *param);
int			life_cycle(t_param *param);
void		*life_start(void *philo);
void		take_forks(t_philo *philo);
void		put_down_forks(t_philo *philo);
void		*is_everyone_full(void *param);
void		*anyone_dead(void *philo);
void		print(t_philo *philo, char *str);
void		change_state(t_philo *philo, int state);

#endif
