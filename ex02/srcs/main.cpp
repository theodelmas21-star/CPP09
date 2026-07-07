/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdelmas2 <tdelmas2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 11:56:15 by tdelmas2          #+#    #+#             */
/*   Updated: 2026/07/07 15:10:30 by tdelmas2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/PmergeMe.hpp"

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        std::cerr << "Error: You have to put at least 2 numbers" << '\n';
        return 1;
    }
    std::vector<std::string> args(argv + 1, argv + argc);
    PmergeMe pm;

    try
    {
        std::cout << "------------VECTOR---------------\n";
        double timeVec = pm.measureTime(&PmergeMe::sort<std::vector>, args);
        std::cout << "------------DEQUE---------------\n";
        double timeDeq = pm.measureTime(&PmergeMe::sort<std::deque>, args);

        std::cout << "Time to process a range of " << args.size() << " elements with std::vector : " << timeVec << " us"
                  << '\n';
        std::cout << "Time to process a range of " << args.size() << " elements with std::deque : " << timeDeq << " us"
                  << '\n';
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}