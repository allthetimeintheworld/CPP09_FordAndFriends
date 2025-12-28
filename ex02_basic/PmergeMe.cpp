#include "PmergeMe.hpp"
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <sys/time.h>
#include <iomanip>

// Orthodox Canonical Form
PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& other) 
	: _vectorData(other._vectorData), _dequeData(other._dequeData) {}

PmergeMe& PmergeMe::operator=(const PmergeMe& other)
{
	if (this != &other)
	{
		_vectorData = other._vectorData;
		_dequeData = other._dequeData;
	}
	return *this;
}

PmergeMe::~PmergeMe() {}

// Validation
bool PmergeMe::isValidNumber(const std::string& str) const
{
	if (str.empty())
		return false;
	
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (!isdigit(str[i]))
			return false;
	}
	
	// Check for positive integer
	long num = atol(str.c_str());
	if (num <= 0 || num > 2147483647)
		return false;
	
	return true;
}

bool PmergeMe::parseInput(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cerr << "Error: no input provided" << std::endl;
		return false;
	}
	
	for (int i = 1; i < argc; ++i)
	{
		std::string arg(argv[i]);
		
		if (!isValidNumber(arg))
		{
			std::cerr << "Error" << std::endl;
			return false;
		}
		
		int num = atoi(arg.c_str());
		_vectorData.push_back(num);
		_dequeData.push_back(num);
	}
	
	return true;
}

// Insertion sort for small arrays (vector version)
void PmergeMe::insertionSortVector(std::vector<int>& arr, int left, int right)
{
	for (int i = left + 1; i <= right; ++i)
	{
		int key = arr[i];
		int j = i - 1;
		
		while (j >= left && arr[j] > key)
		{
			arr[j + 1] = arr[j];
			--j;
		}
		arr[j + 1] = key;
	}
}

// Ford-Johnson merge-insert sort for vector
void PmergeMe::mergeInsertSortVector(std::vector<int>& arr)
{
	int n = arr.size();
	
	// Base case: use insertion sort for small arrays
	if (n <= 10)
	{
		insertionSortVector(arr, 0, n - 1);
		return;
	}
	
	// Create pairs and sort them
	std::vector<std::pair<int, int> > pairs;
	int i;
	
	for (i = 0; i + 1 < n; i += 2)
	{
		if (arr[i] > arr[i + 1])
			pairs.push_back(std::make_pair(arr[i + 1], arr[i]));
		else
			pairs.push_back(std::make_pair(arr[i], arr[i + 1]));
	}
	
	// Handle odd element
	int straggler = -1;
	if (i < n)
		straggler = arr[i];
	
	// Sort pairs by their larger element
	for (size_t j = 1; j < pairs.size(); ++j)
	{
		std::pair<int, int> key = pairs[j];
		int k = j - 1;
		
		while (k >= 0 && pairs[k].second > key.second)
		{
			pairs[k + 1] = pairs[k];
			--k;
		}
		pairs[k + 1] = key;
	}
	
	// Build main chain (larger elements) and pend (smaller elements)
	std::vector<int> mainChain;
	std::vector<int> pend;
	
	for (size_t j = 0; j < pairs.size(); ++j)
	{
		mainChain.push_back(pairs[j].second);
		pend.push_back(pairs[j].first);
	}
	
	// Insert first pend element at the beginning
	if (!pend.empty())
		mainChain.insert(mainChain.begin(), pend[0]);
	
	// Insert remaining pend elements using binary search
	for (size_t j = 1; j < pend.size(); ++j)
	{
		std::vector<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), pend[j]);
		mainChain.insert(pos, pend[j]);
	}
	
	// Insert straggler if exists
	if (straggler != -1)
	{
		std::vector<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), straggler);
		mainChain.insert(pos, straggler);
	}
	
	arr = mainChain;
}

// Insertion sort for deque
void PmergeMe::insertionSortDeque(std::deque<int>& arr, int left, int right)
{
	for (int i = left + 1; i <= right; ++i)
	{
		int key = arr[i];
		int j = i - 1;
		
		while (j >= left && arr[j] > key)
		{
			arr[j + 1] = arr[j];
			--j;
		}
		arr[j + 1] = key;
	}
}

// Ford-Johnson merge-insert sort for deque
void PmergeMe::mergeInsertSortDeque(std::deque<int>& arr)
{
	int n = arr.size();
	
	// Base case: use insertion sort for small arrays
	if (n <= 10)
	{
		insertionSortDeque(arr, 0, n - 1);
		return;
	}
	
	// Create pairs and sort them
	std::vector<std::pair<int, int> > pairs;
	int i;
	
	for (i = 0; i + 1 < n; i += 2)
	{
		if (arr[i] > arr[i + 1])
			pairs.push_back(std::make_pair(arr[i + 1], arr[i]));
		else
			pairs.push_back(std::make_pair(arr[i], arr[i + 1]));
	}
	
	// Handle odd element
	int straggler = -1;
	if (i < n)
		straggler = arr[i];
	
	// Sort pairs by their larger element
	for (size_t j = 1; j < pairs.size(); ++j)
	{
		std::pair<int, int> key = pairs[j];
		int k = j - 1;
		
		while (k >= 0 && pairs[k].second > key.second)
		{
			pairs[k + 1] = pairs[k];
			--k;
		}
		pairs[k + 1] = key;
	}
	
	// Build main chain and pend
	std::deque<int> mainChain;
	std::deque<int> pend;
	
	for (size_t j = 0; j < pairs.size(); ++j)
	{
		mainChain.push_back(pairs[j].second);
		pend.push_back(pairs[j].first);
	}
	
	// Insert first pend element at the beginning
	if (!pend.empty())
		mainChain.push_front(pend[0]);
	
	// Insert remaining pend elements using binary search
	for (size_t j = 1; j < pend.size(); ++j)
	{
		std::deque<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), pend[j]);
		mainChain.insert(pos, pend[j]);
	}
	
	// Insert straggler if exists
	if (straggler != -1)
	{
		std::deque<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), straggler);
		mainChain.insert(pos, straggler);
	}
	
	arr = mainChain;
}

void PmergeMe::sort()
{
	struct timeval start, end;
	
	// Display before
	std::cout << "Before: ";
	for (size_t i = 0; i < _vectorData.size() && i < 5; ++i)
		std::cout << _vectorData[i] << " ";
	if (_vectorData.size() > 5)
		std::cout << "[...]";
	std::cout << std::endl;
	
	// Sort with vector
	gettimeofday(&start, NULL);
	mergeInsertSortVector(_vectorData);
	gettimeofday(&end, NULL);
	
	double vectorTime = (end.tv_sec - start.tv_sec) * 1000000.0;
	vectorTime += (end.tv_usec - start.tv_usec);
	
	// Display after
	std::cout << "After:  ";
	for (size_t i = 0; i < _vectorData.size() && i < 5; ++i)
		std::cout << _vectorData[i] << " ";
	if (_vectorData.size() > 5)
		std::cout << "[...]";
	std::cout << std::endl;
	
	// Sort with deque
	gettimeofday(&start, NULL);
	mergeInsertSortDeque(_dequeData);
	gettimeofday(&end, NULL);
	
	double dequeTime = (end.tv_sec - start.tv_sec) * 1000000.0;
	dequeTime += (end.tv_usec - start.tv_usec);
	
	// Display times
	std::cout << std::fixed << std::setprecision(5);
	std::cout << "Time to process a range of " << _vectorData.size() 
			  << " elements with std::vector : " << vectorTime << " us" << std::endl;
	std::cout << "Time to process a range of " << _dequeData.size() 
			  << " elements with std::deque : " << dequeTime << " us" << std::endl;
}

void PmergeMe::displayResults() const
{
	// Results already displayed in sort()
}

const std::vector<int>& PmergeMe::getVectorData() const
{
	return _vectorData;
}

const std::deque<int>& PmergeMe::getDequeData() const
{
	return _dequeData;
}
