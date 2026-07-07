/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdelmas2 <tdelmas2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 21:39:07 by tdelmas2          #+#    #+#             */
/*   Updated: 2026/06/29 12:23:10 by tdelmas2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/BitcoinExchange.hpp"

std::string BitcoinExchange::trim(const std::string &str)
{
    if (str.empty())
        return str;
    size_t first = 0;
    size_t last = str.size() - 1;

    while (first <= last && std::isspace(static_cast<unsigned char>(str[first])))
        first++;
    while (last >= first && std::isspace(static_cast<unsigned char>(str[last])))
    {
        if (last == 0)
            break;
        last--;
    }
    if (first > last)
        return "";
    return str.substr(first, (last - first) + 1);
}

void BitcoinExchange::CheckDate(std::string const &date) const
{
    if (date.size() != 10 || date[4] != '-' || date[7] != '-')
        throw GoodExample();
    for (size_t i = 0; i < date.size(); i++)
    {
        if (i == 4 || i == 7)
            continue;
        if (!std::isdigit(static_cast<unsigned char>(date[i])))
            throw GoodExample();
    }
}

void BitcoinExchange::CheckValue(std::string const &value) const
{
    if (value.empty())
        throw GoodExample();

    size_t i = 0;
    if (value[i] == '+')
        i++;
    if (value[i] == '-')
        throw Negative();

    size_t pointCount = 0;
    size_t digitCount = 0;

    for (; i < value.size(); i++)
    {
        if (value[i] == '.')
        {
            pointCount++;
            if (pointCount > 1 || digitCount == 0 || i == value.size() - 1)
                throw ValueFormat();
        }
        else if (std::isdigit(static_cast<unsigned char>(value[i])))
        {
            digitCount++;
        }
        else
        {
            throw ValueFormat();
        }
    }
    if (digitCount == 0)
        throw ValueFormat();
}

void BitcoinExchange::CheckFormat(std::string &line, bool isFirstLine, bool isCsv)
{
    if (isFirstLine)
    {
        if (!isCsv && line == "date | value")
            return;
        if (isCsv && line == "date,exchange_rate")
            return;
        throw GoodExample();
    }

    size_t pipePos = isCsv ? line.find(',') : line.find('|');
    if (pipePos == std::string::npos)
        throw GoodExample();

    std::string date = trim(line.substr(0, pipePos));
    std::string valuePart = trim(line.substr(pipePos + 1));

    CheckDate(date);
    CheckValue(valuePart);
}

bool BitcoinExchange::LoadDatabase(const std::string &filename, std::map<std::string, float> &database)
{
    std::ifstream file(filename.c_str());
    if (!file.is_open())
    {
        std::cout << "Error: could not open the file." << "\n";
        return false;
    }

    std::string line;
    bool isFirstLine = true;

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        if (isFirstLine)
        {
            try
            {
                CheckFormat(line, true, true);
            }
            catch (const std::exception &e)
            {
                std::cout << e.what() << "\n";
            }
            isFirstLine = false;
            continue;
        }

        try
        {
            CheckFormat(line, false, true);
            size_t commaPos = line.find(',');
            std::string datePart = trim(line.substr(0, commaPos));
            std::string pricePart = trim(line.substr(commaPos + 1));

            float price;
            std::stringstream ss(pricePart);
            if (!(ss >> price))
                throw ValueFormat();
            database[datePart] = price;
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << "\n";
        }
    }
    file.close();
    return true;
}

void BitcoinExchange::Conversion()
{
    std::ifstream file(_input.c_str());
    
    if (!file.is_open())
    {
        std::cout << "Error: could not open the file." << "\n";
        return;
    }

    std::string line;
    bool isFirstLine = true;
    std::map<std::string, float> database;

    if (!LoadDatabase("data.csv", database))
    {
        file.close();
        return;
    }

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        if (isFirstLine)
        {
            try
            {
                CheckFormat(line, true, false);
            }
            catch (const std::exception &e)
            {
                std::cout << e.what() << "\n";
            }
            isFirstLine = false;
            continue;
        }

        try
        {
            CheckFormat(line, false, false);

            size_t pipePos = line.find('|');
            std::string datePart = trim(line.substr(0, pipePos));
            std::string pricePart = trim(line.substr(pipePos + 1));

            float price;
            std::stringstream ss(pricePart);
            if (!(ss >> price))
                throw ValueFormat();

            if (price > 1000)
            {
                std::cout << "Error: too large a number." << "\n";
                continue;
            }

            std::map<std::string, float>::const_iterator it = database.lower_bound(datePart);

            if (it == database.end() || it->first != datePart)
            {
                if (it == database.begin())
                {
                    std::cout << "Error: date too old => " << datePart << "\n";
                    continue;
                }
                --it;
            }

            float exchangeRate = it->second;
            float result = price * exchangeRate;

            std::cout << datePart << " => " << price << " = " << result << "\n";
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << "\n";
        }
    }
    file.close();
}

BitcoinExchange::BitcoinExchange(std::string input) : _input(input)
{
}

BitcoinExchange::BitcoinExchange(BitcoinExchange const &src)
{
    _input = src._input;
}

BitcoinExchange &BitcoinExchange::operator=(BitcoinExchange const &rhs)
{
    if (this != &rhs)
        _input = rhs._input;
    return *this;
}

BitcoinExchange::~BitcoinExchange()
{
}
