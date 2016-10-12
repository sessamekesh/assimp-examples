#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <stdexcept>

int main()
{
	unsigned int r = 0u, c = 0u;
	std::cin >> r >> c;
	std::vector<std::pair<unsigned int, unsigned int> > startPositions[26];

	// For a test to be valid, must fall within a certain range of rows and cols
	if (r < 1 || r > 2000 || c < 1 || c > 2000)
	{
		throw std::runtime_error("Row or column size is out of bounds. Test failed");
	}

	char** grid = new char*[r];
	bool** result = new bool*[r];
	for (unsigned int row = 0u; row < r; row++)
	{
		grid[row] = new char[c];
		result[row] = new bool[c];
		for (unsigned int col = 0u; col < c; col++)
		{
			std::cin >> grid[row][col];

			// For a test to be valid, all characters must be uppercase English letters
			if (grid[row][col] > 'Z' || grid[row][col] < 'A')
			{
				throw std::runtime_error("Grid character falls outside of accepted range!");
			}

			startPositions[grid[row][col] - 'A'].push_back({ row, col });

			result[row][col] = false;
		}
	}

	unsigned int n = 0;
	std::cin >> n;
	std::cin.ignore();

	std::vector<std::string> words(n);
	for (unsigned int wordIdx = 0u; wordIdx < n; wordIdx++)
	{
		getline(std::cin, words[wordIdx]);
	}

	// Everything has been input. Search for the words
	for (std::string word : words)
	{
		bool wordFound = false;

		for (auto start : startPositions[word[0] - 'A'])
		{
			// Check left to right
			if (start.second + word.size() <= c)
			{
				bool ltr = true;
				for (unsigned int i = 0u; i < word.size(); i++)
				{
					if (grid[start.first][start.second + i] != word[i])
					{
						ltr = false;
						break;
					}
				}
				if (ltr && wordFound)
				{
					throw std::runtime_error("Word has already been found!");
				}
				else if (ltr)
				{
					wordFound = true;
					for (unsigned int i = 0u; i < word.size(); i++)
					{
						result[start.first][start.second + i] = true;
					}
					break;
				}
			}

			// Check top to bottom
			if (start.first + word.size() <= r)
			{
				bool ttb = true;
				for (unsigned int i = 0u; i < word.size(); i++)
				{
					if (grid[start.first + i][start.second] != word[i])
					{
						ttb = false;
						break;
					}
				}
				if (ttb && wordFound)
				{
					throw std::runtime_error("Word has already been found!");
				}
				else if (ttb)
				{
					wordFound = true;
					for (unsigned int i = 0u; i < word.size(); i++)
					{
						result[start.first + i][start.second] = true;
					}
					break;
				}
			}

			// Check up diagonal
			if (start.first >= (word.size() + 1) && start.second + word.size() <= c)
			{
				bool updiag = true;
				for (unsigned int i = 0u; i < word.size(); i++)
				{
					if (grid[start.first - i][start.second + i] != word[i])
					{
						updiag = false;
						break;
					}
				}
				if (updiag && wordFound)
				{
					throw std::runtime_error("Word has already been found!");
				}
				else if (updiag)
				{
					wordFound = true;
					for (unsigned int i = 0u; i < word.size(); i++)
					{
						result[start.first - i][start.second + i] = true;
					}
					break;
				}
			}

			// Check down diagonal
			if (start.first + word.size() <= r && start.second + word.size() <= c)
			{
				bool downdiag = true;
				for (unsigned int i = 0u; i < word.size(); i++)
				{
					if (grid[start.first + i][start.second + i] != word[i])
					{
						downdiag = false;
						break;
					}
				}
				if (downdiag && wordFound)
				{
					throw std::runtime_error("Word has already been found!");
				}
				else if (downdiag)
				{
					wordFound = true;
					for (unsigned int i = 0u; i < word.size(); i++)
					{
						result[start.first + i][start.second + i] = true;
					}
					break;
				}
			}
		}

		if (!wordFound)
		{
			throw std::runtime_error(std::string("Word not found: ") + word);
		}
	}

	// Output result
	for (unsigned int row = 0u; row < r; row++)
	{
		for (unsigned int col = 0u; col < c; col++)
		{
			if (result[row][col])
			{
				std::cout << grid[row][col] << " ";
			}
			else
			{
				std::cout << 'x' << " ";
			}
		}
		std::cout << std::endl;
	}

	for (unsigned int row = 0u; row < r; row++)
	{
		delete[] grid[row];
		grid[row] = nullptr;

		delete[] result[row];
		result[row] = nullptr;
	}
	delete[] grid;
	grid = nullptr;
	delete[] result;
	result = nullptr;

	return 0;
}