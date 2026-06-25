/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdelmas2 <tdelmas2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 21:39:07 by tdelmas2          #+#    #+#             */
/*   Updated: 2026/06/25 18:37:01 by tdelmas2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/BitcoinExchange.hpp"

std::string trim (const std::string &str){
    size_t first = 0;
    size_t last = str.size();

    while (first < last && std::isspace(static_cast <unsigned char>(str[first])))
        first++;
    while (last > first && std::isspace(static_cast <unsigned char>(str[last])))
        last--;
    return str.substr(first, last - first);
}

void BitcoinExchange::CheckDate(std::string const &date) const{
    if (date.size() != 10 || date[4] != '-' || date[7] != '-')
        throw GoodExample();
    for (size_t i = 0; i < date.size(); i++) {
        if (i == 4 || i == 7) 
            continue;
        if (!std::isdigit(static_cast<unsigned char>(date[i])))
            throw GoodExample();
    }
}

void BitcoinExchange::CheckValue(std::string const &value) const{
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
        if (value[i] == '.'){
            pointCount++;
            if (pointCount > 1 || digitCount == 0 || i == value.size() - 1)
                throw ValueFormat();
        }
        else if (std::isdigit(static_cast<unsigned char>(value[i]))) {
            digitCount++;
        } else {
            throw ValueFormat();
        }
    }
    if (digitCount == 0)
        throw ValueFormat();
}


void BitcoinExchange::CheckFormat(std::string &line, bool isFirstLine, bool isCsv){
    size_t pipePos;

    if (line == "date | value" && isFirstLine == false)
        throw DateValue();
    if (!isCsv)
        pipePos = line.find('|');
    else if (isCsv)
        pipePos = line.find(',');
    if (pipePos == std::string::npos)
        throw GoodExample();
    std::string date = trim(line.substr(0, pipePos));
    std::string valuePart = trim (line.substr(pipePos + 1));
    
    CheckDate(date);
    CheckValue(valuePart);
}

bool BitcoinExchange::LoadDatabase(const std::string &filename, std::map<std::string, float> &database){
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
        try
        {
            CheckFormat(line, isFirstLine, true);
            if (isFirstLine) {
                isFirstLine = false;
                continue;
            }
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
        { std::cout << e.what() << "\n"; }
    }
    file.close();
    return true;
}

void BitcoinExchange::Conversion(){
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
        return;
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;
        try
        {
            CheckFormat(line, isFirstLine, false);
            if (isFirstLine) {
                isFirstLine = false;
                continue;
            }
            size_t pipePos = line.find('|');
            std::string datePart = trim(line.substr(0, pipePos));
            std::string pricePart = trim(line.substr(pipePos + 1));
            float price;
            std::stringstream ss(pricePart);
            if (!(ss >> price))
                throw ValueFormat();
            
        }
        catch (const std::exception &e)
        { std::cout << e.what() << "\n"; }
    }
    file.close();
}
    
BitcoinExchange::BitcoinExchange(std::string input) : _input(input) {
}
    
BitcoinExchange::BitcoinExchange(BitcoinExchange const &src){
    _input = src._input;
}
    
BitcoinExchange &BitcoinExchange::operator=(BitcoinExchange const &rhs){
    if (this != &rhs)
        _input = rhs._input;
}
    
BitcoinExchange::~BitcoinExchange(){
}
    