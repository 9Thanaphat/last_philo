/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttangcha <ttangcha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 19:45:08 by ttangcha          #+#    #+#             */
/*   Updated: 2025/08/24 22:31:28 by ttangcha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *arg)
{
	int	i;
	int	num;
	int	neg;

	i = 0;
	num = 0;
	neg = 1;
	while (arg[i] == 32 || (arg[i] >= 9 && arg[i] <= 13))
		i++;
	if (arg[i] == '-' || arg[i] == '+')
	{
		if (arg[i] == '-')
			neg = -1;
		i++;
	}
	while (arg[i])
	{
		if (arg[i] >= '0' && arg[i] <= '9')
			num = (num * 10) + (arg[i] - 48);
		else
			break ;
		i++;
	}
	return (num * neg);
}

static const char	*digits_ptr_and_len(const char *s, int *len_out)
{
	size_t	i;

	i = 0;
	if (!s || !*s)
		return (NULL);
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (NULL);
		i++;
	}
	while (*s == '0' && s[1])
		s++;
	i = 0;
	while (s[i])
		i++;
	*len_out = (int)i;
	return (s);
}

static bool	is_positive_int(const char *s)
{
	const char	*d;
	const char	*int_max;
	int			len;
	int			i;

	int_max = "2147483647";
	d = digits_ptr_and_len(s, &len);
	if (!d)
		return (false);
	if (len < 10)
		return (true);
	if (len > 10)
		return (false);
	i = 0;
	while (i < 10)
	{
		if (d[i] < int_max[i])
			return (true);
		if (d[i] > int_max[i])
			return (false);
		i++;
	}
	return (true);
}

void	parse(t_table *table, char **av)
{
	int	limit_meal;

	if (!is_positive_int(av[1]) || !is_positive_int(av[2])
		|| !is_positive_int(av[3]) || !is_positive_int(av[4]))
		error_exit("input error");
	if (av[5] && !is_positive_int(av[5]))
		error_exit("input error");
	table->philo_nbr = ft_atoi(av[1]);
	table->time_to_die = ft_atoi(av[2]);
	table->time_to_eat = ft_atoi(av[3]);
	table->time_to_sleep = ft_atoi(av[4]);
	if (table->philo_nbr < 1)
		error_exit("philo nbr must more than 0.");
	if (table->time_to_die < 60 || table->time_to_eat < 60
		|| table->time_to_sleep < 60)
		error_exit("use time stamp more than 60ms");
	limit_meal = -1;
	if (av[5])
	{
		limit_meal = ft_atoi(av[5]);
		if (limit_meal < 1)
			error_exit("limit meal nbr must more than zero.");
	}
	table->limit_meal_nbr = limit_meal;
}
