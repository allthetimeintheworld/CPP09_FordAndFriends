#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <string>

class PmergeMe
{
private:
	std::vector<int> _vectorData;
	std::deque<int> _dequeData;
	
	// Helper functions for vector
	void mergeInsertSortVector(std::vector<int>& arr);
	void insertionSortVector(std::vector<int>& arr, int left, int right);
	
	// Helper functions for deque
	void mergeInsertSortDeque(std::deque<int>& arr);
	void insertionSortDeque(std::deque<int>& arr, int left, int right);
	
	// Validation
	bool isValidNumber(const std::string& str) const;

public:
	// Orthodox Canonical Form
	PmergeMe();
	PmergeMe(const PmergeMe& other);
	PmergeMe& operator=(const PmergeMe& other);
	~PmergeMe();

	// Methods
	bool parseInput(int argc, char** argv);
	void sort();
	void displayResults() const;
	
	const std::vector<int>& getVectorData() const;
	const std::deque<int>& getDequeData() const;
};

#endif
