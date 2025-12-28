#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <string>
#include <map>

class BitcoinExchange
{
private:
	std::map<std::string, double> _data;
	
	bool isValidDate(const std::string& date) const;
	bool isValidValue(const std::string& valueStr, double& value) const;
	std::string trim(const std::string& str) const;
	bool parseDate(const std::string& date, int& year, int& month, int& day) const;
	std::string findClosestDate(const std::string& date) const;

public:
	// Orthodox Canonical Form
	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange& other);
	BitcoinExchange& operator=(const BitcoinExchange& other);
	~BitcoinExchange();

	// Methods
	bool loadDatabase(const std::string& filename);
	void processInputFile(const std::string& filename);
};

#endif
