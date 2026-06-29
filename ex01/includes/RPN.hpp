/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdelmas2 <tdelmas2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 11:56:27 by tdelmas2          #+#    #+#             */
/*   Updated: 2026/06/29 16:46:39 by tdelmas2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPN_HPP
#define RPN_HPP

#include <iostream>
#include <stack>
#include <exception>
#include <cstdlib>

class RPN
{
  private:
    std::string _input;

  public:
    RPN(std::string input);
    RPN(RPN const &src);
    RPN &operator=(RPN const &rhs);
    ~RPN();

    void Calculs(void);

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