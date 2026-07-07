/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdelmas2 <tdelmas2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 21:39:07 by tdelmas2          #+#    #+#             */
/*   Updated: 2026/06/29 17:02:09 by tdelmas2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/PmergeMe.hpp"

PmergeMe::PmergeMe(void)
{
}

PmergeMe::PmergeMe(PmergeMe const &src)
{
    (void)src;
}

PmergeMe &PmergeMe::operator=(PmergeMe const &rhs)
{
    (void)rhs;
    return *this;
}

PmergeMe::~PmergeMe()
{
}