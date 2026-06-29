/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdelmas2 <tdelmas2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 21:39:07 by tdelmas2          #+#    #+#             */
/*   Updated: 2026/06/29 17:02:09 by tdelmas2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/RPN.hpp"

RPN::RPN(std::string input) : _input(input)
{
}

RPN::RPN(RPN const &src)
{
    _input = src._input;
}

RPN &RPN::operator=(RPN const &rhs)
{
    if (this != &rhs)
        _input = rhs._input;
    return *this;
}

RPN::~RPN()
{
}

void RPN::Calculs(void){
    std::stack<int> stack;
    bool needSpace = false;

    for (size_t i = 0; i < _input.size(); i++)
    {
        int topValue;
        int secondValue;

       if ((_input[i] >= '0' && _input[i] <= '9') && needSpace == false)
       {
            stack.push(_input[i] - '0');
            needSpace = true;
       }
       else if (needSpace == false)
        {
            if (stack.size() < 2)
                throw Error();
            topValue = stack.top();
            stack.pop();
            secondValue = stack.top();
            stack.pop();
            switch(_input[i])
            {
                case '+':
                    stack.push(secondValue + topValue);
                    break;
                case '-':
                    stack.push(secondValue - topValue);
                    break;
                case '*':
                        stack.push(secondValue * topValue);
                        break;
                case '/':
                    if (topValue == 0)
                        throw Error();
                    stack.push(secondValue / topValue);
                    break;
                default:
                    throw Error();
                    break;
            }
            needSpace = true;
        }
        else if (_input[i] == ' ')
            needSpace = false;
        else
            throw Error();
    }
    if (stack.size() == 1)
        std::cout << stack.top() << '\n';
    else
        throw Error();
}

