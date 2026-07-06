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

bool PmergeMe::parser(std::vector<std::string> args)
{
    char *endptr;
    std::set<int> duplicateCheck;

    if (args.empty())
        return false;

    _A.reserve(args.size());
    for (size_t i = 0; i < args.size(); i++)
    {
            errno = 0;
        long val = std::strtol(args[i].c_str(), &endptr, 10);
        if (endptr == args[i].c_str() || *endptr != '\0' || errno == ERANGE || val > 2147483647 || val < 0)
            return false;
        if (duplicateCheck.count(static_cast<int>(val)) > 0)
            return false; 
        duplicateCheck.insert(static_cast<int>(val));
        _A.push_back(static_cast<int>(val));
    }
    return true;
}

void PmergeMe::fillPairs(void)
{
    if (_A.size() % 2 != 0)
    {
        _straggler = _A.back();
        _hasStraggler = true;
    }
    else
        _hasStraggler = false;
    for (size_t i = 0; i + 1 < _A.size(); i += 2)
    {
        if (_A[i] > _A[i + 1])
            _pairs.push_back(std::make_pair(_A[i], _A[i + 1]));
        else
            _pairs.push_back(std::make_pair(_A[i + 1], _A[i]));
    }
}

void PmergeMe::mergePairs(std::vector<std::pair<int, int> >& left, 
    std::vector<std::pair<int, int> >& right, std::vector<std::pair<int, int> >& result)
{
    size_t i = 0, j = 0;
    
    result.clear();
    while (i < left.size() && j < right.size())
    {
        if (left[i].first < right[j].first)
        {
            result.push_back(left[i]);
            i++;
        }
        else
        {
            result.push_back(right[j]);
            j++;
        }
    }
    for (;i < left.size(); i++)
        result.push_back(left[i]);
    for (;j < right.size(); j++)
        result.push_back(right[j]);

}

void PmergeMe::sortPairs(std::vector<std::pair<int, int> >& pairsVec)
{
    if (pairsVec.size() <= 1)
        return;
    size_t mid = pairsVec.size() / 2;
    std::vector<std::pair<int, int> > left(pairsVec.begin(), pairsVec.begin() + mid);
    std::vector<std::pair<int, int> > right(pairsVec.begin() + mid, pairsVec.end());

    sortPairs(left);
    sortPairs(right);

    mergePairs(left, right, pairsVec);
}

void PmergeMe::separateChains(void)
{
    _mainChain.clear();
    _pendChain.clear();
    for (size_t i = 0; i < _pairs.size(); i++)
    {
        _mainChain.push_back(_pairs[i].first);
        _pendChain.push_back(_pairs[i].second);
    }
    _mainChain.insert(_mainChain.begin(), _pendChain[0]);
}

void PmergeMe::generateJacobsthal(size_t pendSize)
{
    size_t i = 2;

    _jacob.push_back(0);
    _jacob.push_back(1);

    while (true)
    {
        size_t nextJacob = _jacob[i - 1] + 2 * _jacob[i - 2];
        if (nextJacob >= pendSize)
        {
                _jacob.push_back(pendSize);
                break;
        }
        _jacob.push_back(nextJacob);
        i++;
    }
}

void PmergeMe::pendToMain(void)
{
    for (size_t i = 2; i < _jacob.size(); i++)
    {
        size_t lastJacob = _jacob[i - 1];
        size_t curJacob = _jacob[i];

        while (curJacob > lastJacob)
        {
            if (curJacob - 1 == 0)
            {
                curJacob--;
                continue;;
            }
            std::vector<int>::iterator it = std::lower_bound(_mainChain.begin(), _mainChain.end(), _pendChain[curJacob - 1]);
            _mainChain.insert(it, _pendChain[curJacob - 1]);
            curJacob--;
        }
        if (_hasStraggler)
        {
            std::vector<int>::iterator it = std::lower_bound(_mainChain.begin(), _mainChain.end(), _straggler);
            _mainChain.insert(it, _straggler);
        }
    }
}

void PmergeMe::sort(std::vector<std::string> args)
{
    std::cout << "Before: ";
    printContainer<std::vector<std::string> >(args);
    fillPairs();
    sortPairs(_pairs);
    separateChains();
    generateJacobsthal(_pendChain.size());
    pendToMain();
    std::cout << "After: ";
    printContainer<std::vector<int> >(_mainChain);
}