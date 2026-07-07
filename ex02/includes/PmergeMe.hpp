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
#include <deque>
#include <set>
#include <sys/time.h>
#include <exception>


template <template <typename, typename> class ContainerTemplate, typename T>
struct SortState
{
  typedef ContainerTemplate<T, std::allocator<T> > A_type;
  
  ContainerTemplate<T, std::allocator<T> > A;
  ContainerTemplate<std::pair<T, T>, std::allocator<std::pair<T, T> > > pairs;
  ContainerTemplate<T, std::allocator<T> > mainChain;
  ContainerTemplate<T, std::allocator<T> > pendChain;
  ContainerTemplate<size_t, std::allocator<size_t> > jacob;

  T straggler;
  bool hasStraggler;
};

class PmergeMe
{
  private:

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

  template <typename StateType>
  void fillPairs(StateType &state)
  {
    state.pairs.clear();
    if (state.A.size() % 2 != 0) {
        state.straggler = state.A.back();
        state.hasStraggler = true;
    } else {
        state.hasStraggler = false;
    }
    for (size_t i = 0; i + 1 < state.A.size(); i += 2) {
        if (state.A[i] > state.A[i + 1])
            state.pairs.push_back(std::make_pair(state.A[i], state.A[i + 1]));
        else
            state.pairs.push_back(std::make_pair(state.A[i + 1], state.A[i]));
    }
  }
  
template <typename PairContainer>
  void sortPairs(PairContainer & pairs)
  {
    if (pairs.size() <= 1)
      return;
    size_t mid = pairs.size() / 2;
    PairContainer left(pairs.begin(), pairs.begin() + mid);
    PairContainer right(pairs.begin() + mid, pairs.end());

    sortPairs(left);
    sortPairs(right);
    mergePairs(left, right, pairs);
  }

  template <typename PairContainer>
  void mergePairs(PairContainer &left, PairContainer &right, PairContainer &result)
  {
    size_t i = 0, j = 0;
    result.clear();
    while (i < left.size() && j < right.size()) {
        if (left[i].first < right[j].first) {
            result.push_back(left[i]);
            i++;
        } else {
            result.push_back(right[j]);
            j++;
        }
    }
    for (; i < left.size(); i++) result.push_back(left[i]);
    for (; j < right.size(); j++) result.push_back(right[j]);
  }

  template <typename StateType>
  void separateChains(StateType &state)
  {
    state.mainChain.clear();
    state.pendChain.clear();
    for (size_t i = 0; i < state.pairs.size(); i++) {
        state.mainChain.push_back(state.pairs[i].first);
        state.pendChain.push_back(state.pairs[i].second);
    }
    if (!state.pendChain.empty())
        state.mainChain.insert(state.mainChain.begin(), state.pendChain[0]);
  }

  template <typename StateType>
  void generateJacobsthal(StateType &state)
  {
    state.jacob.clear();
    state.jacob.push_back(0);
    state.jacob.push_back(1);
    size_t i = 2;
    size_t pendSize = state.pendChain.size();
    
    while (true) {
        size_t nextJacob = state.jacob[i - 1] + 2 * state.jacob[i - 2];
        if (nextJacob >= pendSize) {
            state.jacob.push_back(pendSize);
            break;
        }
        state.jacob.push_back(nextJacob);
        i++;
    }
  }

  template <typename StateType>
  void pendToMain(StateType &state)
  {
    for (size_t i = 2; i < state.jacob.size(); i++) {
      size_t lastJacob = state.jacob[i - 1];
      size_t curJacob = state.jacob[i];

      if (curJacob > state.pendChain.size())
          curJacob = state.pendChain.size();

      while (curJacob > lastJacob) {
          if (curJacob - 1 > 0) {
              // 💡 NOTE : Ici, on extrait dynamiquement le type de l'itérateur du conteneur principal de l'état
              typename StateType::A_type::iterator it = std::lower_bound(
                  state.mainChain.begin(), state.mainChain.end(), state.pendChain[curJacob - 1]
              );
              state.mainChain.insert(it, state.pendChain[curJacob - 1]);
          }
          curJacob--;
      }
  }
  if (state.hasStraggler) {
      typename StateType::A_type::iterator it = std::lower_bound(
          state.mainChain.begin(), state.mainChain.end(), state.straggler
      );
      state.mainChain.insert(it, state.straggler);
  }
  }

  public:
    PmergeMe(void);
    PmergeMe(PmergeMe const &src);
    PmergeMe &operator=(PmergeMe const &rhs);
    ~PmergeMe();

    template <typename Container>
    void parser(std::vector<std::string> args, Container &target)
    {
      char *endptr;
      std::set<int> duplicateCheck;

      if (args.empty())
          throw Error();

      target.clear();
      for (size_t i = 0; i < args.size(); i++)
      {
              errno = 0;
          long val = std::strtol(args[i].c_str(), &endptr, 10);
          if (endptr == args[i].c_str() || *endptr != '\0' || errno == ERANGE || val > 2147483647 || val < 0)
              throw Error();
          if (duplicateCheck.count(static_cast<int>(val)) > 0)
              throw Error(); 
          duplicateCheck.insert(static_cast<int>(val));
          target.push_back(static_cast<int>(val));
      }
    }

    template <template <typename, typename> class ContainerTemplate>
    void sort(std::vector<std::string> args)
    {
      SortState<ContainerTemplate, int> state;

      parser(args, state.A);

      std::cout << "Before: ";
      printContainer<std::vector<std::string> >(args);

      fillPairs(state);
      sortPairs(state.pairs);
      separateChains(state);
      generateJacobsthal(state);
      pendToMain(state);
      std::cout << "After: ";
      printContainer(state.mainChain);
    }
    
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

    class Error : public std::exception
    {
        public:
        virtual const char *what() const throw()
        {
            return "Error";
        }
    };
};

#endif