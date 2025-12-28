#include "BitcoinExchange.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <limits>

// Orthodox Canonical Form
BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : _data(other._data) {}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
	if (this != &other)
		_data = other._data;
	return *this;
}

BitcoinExchange::~BitcoinExchange() {}

// Helper functions
std::string BitcoinExchange::trim(const std::string& str) const
{
	size_t first = str.find_first_not_of(" \t\n\r");
	if (first == std::string::npos)
		return "";
	size_t last = str.find_last_not_of(" \t\n\r");
	return str.substr(first, (last - first + 1));
}

bool BitcoinExchange::parseDate(const std::string& date, int& year, int& month, int& day) const
{
	if (date.length() != 10 || date[4] != '-' || date[7] != '-')
		return false;

	for (size_t i = 0; i < date.length(); ++i)
	{
		if (i == 4 || i == 7)
			continue;
		if (!isdigit(date[i]))
			return false;
	}

	year = atoi(date.substr(0, 4).c_str());
	month = atoi(date.substr(5, 2).c_str());
	day = atoi(date.substr(8, 2).c_str());

	return true;
}

bool BitcoinExchange::isValidDate(const std::string& date) const
{
	int year, month, day;
	
	if (!parseDate(date, year, month, day))
		return false;

	if (year < 0 || month < 1 || month > 12 || day < 1 || day > 31)
		return false;

	// Check days in month
	int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	
	// Check for leap year
	if (month == 2)
	{
		bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
		if (isLeap && day > 29)
			return false;
		if (!isLeap && day > 28)
			return false;
	}
	else if (day > daysInMonth[month - 1])
		return false;

	return true;
}

bool BitcoinExchange::isValidValue(const std::string& valueStr, double& value) const
{
	char* endptr;
	value = strtod(valueStr.c_str(), &endptr);

	// Check if conversion was successful
	if (*endptr != '\0' && *endptr != '\n')
		return false;

	return true;
}

std::string BitcoinExchange::findClosestDate(const std::string& date) const
{
	std::map<std::string, double>::const_iterator it = _data.lower_bound(date);
	
	// If exact match found
	if (it != _data.end() && it->first == date)
		return it->first;
	
	// If lower_bound returns begin, no lower date exists
	if (it == _data.begin())
		return "";
	
	// Go to the previous element (closest lower date)
	--it;
	return it->first;
}

bool BitcoinExchange::loadDatabase(const std::string& filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: could not open database file." << std::endl;
		return false;
	}

	std::string line;
	bool firstLine = true;

	while (std::getline(file, line))
	{
		// Skip header line
		if (firstLine)
		{
			firstLine = false;
			continue;
		}

		size_t commaPos = line.find(',');
		if (commaPos == std::string::npos)
			continue;

		std::string date = trim(line.substr(0, commaPos));
		std::string valueStr = trim(line.substr(commaPos + 1));

		if (!isValidDate(date))
			continue;

		double value;
		if (!isValidValue(valueStr, value))
			continue;

		_data[date] = value;
	}

	file.close();
	return true;
}

void BitcoinExchange::processInputFile(const std::string& filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: could not open file." << std::endl;
		return;
	}

	std::string line;
	bool firstLine = true;

	while (std::getline(file, line))
	{
		// Skip header line
		if (firstLine)
		{
			firstLine = false;
			continue;
		}

		size_t pipePos = line.find('|');
		if (pipePos == std::string::npos)
		{
			std::cerr << "Error: bad input => " << trim(line) << std::endl;
			continue;
		}

		std::string date = trim(line.substr(0, pipePos));
		std::string valueStr = trim(line.substr(pipePos + 1));

		// Validate date
		if (!isValidDate(date))
		{
			std::cerr << "Error: bad input => " << date << std::endl;
			continue;
		}

		// Validate value
		double value;
		if (!isValidValue(valueStr, value))
		{
			std::cerr << "Error: bad input => " << date << std::endl;
			continue;
		}

		// Check if value is negative
		if (value < 0)
		{
			std::cerr << "Error: not a positive number." << std::endl;
			continue;
		}

		// Check if value is too large
		if (value > 1000)
		{
			std::cerr << "Error: too large a number." << std::endl;
			continue;
		}

		// Find the exchange rate
		std::string closestDate = findClosestDate(date);
		if (closestDate.empty())
		{
			std::cerr << "Error: no data available for date => " << date << std::endl;
			continue;
		}

		double rate = _data[closestDate];
		double result = value * rate;

		std::cout << date << " => " << value << " = " << result << std::endl;
	}

	file.close();
}
