/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdelmas2 <tdelmas2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 11:56:27 by tdelmas2          #+#    #+#             */
/*   Updated: 2026/06/29 16:46:39 by tdelmas2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPN_HPP
#define RPN_HPP

#include <cstdlib>
#include <iostream>
#include <vector>
#include <set>
#include <sys/time.h>

class PmergeMe
{
  private:
  std::vector<int> _A;
  std::vector<std::pair<int, int> > _pairs;
  std::vector<int> _mainChain;
  std::vector<int> _pendChain;
  std::vector<size_t> _jacob;
  int _straggler;
  bool _hasStraggler;

  template <typename T>
  void printContainer(const T& container) const
  {
    typename T::const_iterator it;
    int i = 0;
    int range;
    
    it = container.begin();
    if (container.size() > 5)
        range = 4;
    else
        range = container.size();
    while (it != container.end() && i < range)
    {
        std::cout << *it << ' ';
        it++;
        i++;
    }
    if (container.size() > 5)
        std::cout << "[...]";
    std::cout << '\n';
  }

  void fillPairs(void);
  void sortPairs(std::vector<std::pair<int, int> >& pairsVec);
  void mergePairs(std::vector<std::pair<int, int> >& left, 
                          std::vector<std::pair<int, int> >& right, 
                          std::vector<std::pair<int, int> >& result);
  void separateChains(void);
  void generateJacobsthal(size_t pendSize);
  void pendToMain(void);

  public:
    PmergeMe(void);
    PmergeMe(PmergeMe const &src);
    PmergeMe &operator=(PmergeMe const &rhs);
    ~PmergeMe();

    bool parser(std::vector<std::string> args);
    void sort(std::vector<std::string> args);
    
    template <typename F>
    double measureTime(F sortFunction, std::vector<std::string> args)
    {
      struct timeval start, end;

      gettimeofday(&start, NULL);
      
      (this->*sortFunction)(args);
      gettimeofday(&end, NULL);
      long seconds = end.tv_sec - start.tv_sec;
      long microseconds = end.tv_usec - start.tv_usec;
      double duration = (seconds * 1000000.0) + microseconds;

      return (duration);
    }
};

#endif