/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdelmas2 <tdelmas2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 11:56:27 by tdelmas2          #+#    #+#             */
/*   Updated: 2026/06/25 18:29:26 by tdelmas2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINTEXCHANGE_HPP
#define BITCOINTEXCHANGE_HPP

#include <fstream>
#include <iostream>
#include <exception>
#include <string>
#include <cctype>
#include <map>
#include <sstream>

class BitcoinExchange{
    private:
    std::string _input;
    
    void CheckFormat(std::string &Line, bool IsFirstLine, bool isCsv);
    std::string trim (const std::string &str);
    void CheckDate(std::string const &date) const;
    void CheckValue(std::string const &value) const;
    bool LoadDatabase(const std::string &file, std::map<std::string, float> &database);
    
    public:
    BitcoinExchange(std::string input);
    BitcoinExchange(BitcoinExchange const &src);
    BitcoinExchange &operator=(BitcoinExchange const &rhs);
    ~BitcoinExchange();

    void Conversion(void);

    class DateValue : public std::exception
    {
        public:
        virtual const char *what() const throw()
        {
            return "Error: The indication 'date | value' must be the first line of the document.";
        }
    };
    class Negative : public std::exception
    {
        public:
        virtual const char *what() const throw()
        {
            return "Error: Not a positive number.";
        }
    };
    class GoodExample : public std::exception
    {
        public:
        virtual const char *what() const throw()
        {
            return "Error: The good format is : 'YYYY-MM-DD | value.";
        }
    };
    class ValueFormat : public std::exception
    {
        public:
        virtual const char *what() const throw()
        {
            return "Error: The value format isn't correct";
        }
    };
};


#endif