// Szymon Janusz G20792986

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Constant declaration

// The maximum width of the puzzle.
const unsigned int kMaxPuzzleColumns = 14;
// The maximum length of the search words.
const unsigned int kMaxWordLength = 3;
// The name of the file used to retrieve the 1D array.
// This stores the whole row of characters.
const string kOneDPuzzleFile = "text1.txt";
// The name of the file used to retrieve the list of words to search in the 1D Array.
// This stores multiple rows of characters.
const string kOneDSearchFile = "search1.txt";
// The name of the file used to retrieve the 2D array.
// This stores the whole row of characters.
const string kTwoDPuzzleFile = "text2D.txt";
// The name of the file used to retrieve the list of words to search in the 2D Array.
// This stores multiple rows of characters.
const string kTwoDSearchFile = "search2D.txt";
// The maximum number of words in the 1D Array.
const unsigned int kMaxWordAmount1D = 6;
// The maximum number of words in the 2D Array.
const unsigned int kMaxWordAmount2D = 16;
// The maximum number of rows in the 2D Array
const unsigned int kMaxPuzzleRows = 4;
// The filtered output of the text in 1D array.
const string kOneDPuzzleFilteredFile = "text1Filtered.txt";
// The filtered output of the text in 2D array.
const string kTwoDPuzzleFilteredFile = "text2DFiltered.txt";
// The character with which to replace the found words.
const char kReplacementChar = '.';

// Structure declaration

// Used to return a bool if the word is found, and index at which it's found.
struct FoundIndex
{
	bool found;
	int index;
};

// Used to return row number as well as all variables inside FoundIndex
struct FoundIndexRow : FoundIndex
{
	int row;
};

// Function declaration

// Read the file into the char array parameter.
void ReadFileIntoArray(ifstream& inputFileStream, const string kFile, char inputData[])
{
	// Open the file.
	inputFileStream.open(kFile);
	if (!inputFileStream)
	{
		cout << "Error:  File cannot be accessed/does not exist.\n";
		cout << "File: " << kFile << endl;
	}
	else
	{
		// Current column
		int column = 0;
		// While the end of file is not reached
		while (!inputFileStream.eof())
		{
			// Prevent memory leaks with unique pointers.
			char ch;
			// Load char from file.
			inputFileStream >> ch;
			if (!inputFileStream.eof())
			{
				inputData[column++] = ch;
			}
		}
		inputFileStream.close();
	}
}

// Read the file into the char array parameter.
void ReadFileInto2DArray(ifstream& inputFileStream, const string kFile, char inputData[kMaxPuzzleRows][kMaxPuzzleColumns])
{
	// Open the file.
	inputFileStream.open(kFile);
	if (!inputFileStream)
	{
		cout << "Error:  File cannot be accessed/does not exist.\n";
		cout << "File: " << kFile << endl;
	}
	else
	{
		for (int row = 0; row < kMaxPuzzleRows; row++)
		{
			for (int column = 0; column < kMaxPuzzleColumns; column++)
			{
				char ch;
				inputFileStream >> ch;
				if (!inputFileStream.eof())
				{
					inputData[row][column] = ch;
				}
			}
		}
		inputFileStream.close();
	}
}

// Separate the char array into words of kWordLength, and push back to the words string vector.
void SeparateCharsToWords(const int& kWordLength, const char kCharArray[], vector<string>& words, const int& kArraySize)
{
	string word;
	// Separate the char array into words, each word of kWordLength, then push to vector.
	for (int i = 0; i < kArraySize; i++)
	{
		// Add the current char to the word string.
		word += kCharArray[i];
		// Check if the word is of kWordLength length.
		if (word.size() == kWordLength)
		{
			words.push_back(word);
			word.clear();
		}
	}
}

// 1D Puzzle search
FoundIndex FirstPuzzleSearch(const char kMainString[kMaxPuzzleColumns], const char kSubString[kMaxWordLength], char filteredArray[kMaxPuzzleColumns])
{
	// Is the current word found in the puzzle.
	bool found = false;
	// Current index used when iterating over the char array.
	int currentIndex = 0;
	// Iterate over the char array, and check neighbour chars in range of 0 to max word length.
	for (int columnIndex = 0; columnIndex < kMaxPuzzleColumns; columnIndex++)
	{
		for (int i = 0; i < kMaxWordLength; i++)
		{
			// If the chars in the word and the array don't match up, break out of the loop.
			if (kMainString[currentIndex + i] != kSubString[i])
			{
				found = false;
				break;
			}
			// The word was found somewhere in the puzzle.
			found = true;
		}
		// Print found message during the current iteration over chars.
		if (found)
		{
			// Replace the chars in the filtered array.
			for (int wordIndex = 0; wordIndex < kMaxWordLength; wordIndex++)
			{
				filteredArray[currentIndex + wordIndex] = kReplacementChar;
			}
			break;
		}
		else
		{
			currentIndex++;
		}		
	}
	// Output the index in what a human would read.
	const int kArrOffset = 1;
	return { found, currentIndex + kArrOffset};
}

// Save a 1D array to file
void SaveFirstFilteredPuzzle(ofstream& outputStream, const char kString[kMaxPuzzleColumns], const string kFileName)
{
	outputStream.open(kFileName);
	if (!outputStream)
	{
		cout << "Error:  File cannot be accessed/written to.\n";
		cout << "File: " << kFileName << endl;
	}
	else
	{
		for (int column = 0; column < kMaxPuzzleColumns; column++)
		{
			outputStream << kString[column];
		}
		// Check if operation succeeded.
		if (!outputStream)
		{
			cout << "Error: Write operation failed.\n" << endl;
		}
	}
	outputStream.close();
}

FoundIndexRow SecondPuzzleSearch(const char kMainString[kMaxPuzzleRows][kMaxPuzzleColumns], const char kSubString[kMaxWordLength], char filteredArray[kMaxPuzzleRows][kMaxPuzzleColumns], const int& currentRow)
{
	FoundIndexRow foundIndexRow;
	foundIndexRow.row = currentRow;
	foundIndexRow.found = false;
	foundIndexRow.index = 0;

	// Array offset that humans would use
	const int kArrOffset = 1;

	// Iterate over all chars in the current row
	for (int columnIndex = 0; columnIndex < kMaxPuzzleColumns; columnIndex++)
	{
		// Check Right Right only if the current index <= max array size - word length
		if (columnIndex <= kMaxPuzzleColumns - kMaxWordLength)
		{
			// Scan for the word.
			for (int i = 0; i < kMaxWordLength; i++)
			{
				// If the chars in the word and the array don't match up, break out of the loop.
				if (kMainString[currentRow][columnIndex + i] != kSubString[i])
				{
					foundIndexRow.found = false;
					break;
				}
				if (i == kMaxWordLength - kArrOffset)
				{
					// The word was found somewhere in the puzzle.
					foundIndexRow.found = true;
					// Replace chars in the filtered array.
					for (int j = 0; j < kMaxWordLength; j++)
					{
						filteredArray[currentRow][columnIndex + j] = kReplacementChar;
					}
					// Set values of where word was found
					foundIndexRow.index = columnIndex + kArrOffset;
					foundIndexRow.row = currentRow + kArrOffset;
					return foundIndexRow;
				}
			}
		}		

		// Check Left Left
		// Only check left left when iterating over the word won't go out of bounds of the array
		if (columnIndex >= kMaxWordLength - kArrOffset)
		{
			// Scan for the word.
			for (int i = 0; i < kMaxWordLength; i++)
			{
				// If the chars in the word and the array don't match up, break out of the loop.
				if (kMainString[currentRow][columnIndex - i] != kSubString[i])
				{
					foundIndexRow.found = false;
					break;
				}
				if (i == kMaxWordLength - kArrOffset)
				{
					// The word was found somewhere in the puzzle.
					foundIndexRow.found = true;
					// Replace chars in the filtered array.
					for (int j = 0; j < kMaxWordLength; j++)
					{
						filteredArray[currentRow][columnIndex - j] = kReplacementChar;
					}
					// Set values of where word was found
					foundIndexRow.index = columnIndex + kArrOffset;
					foundIndexRow.row = currentRow + kArrOffset;
					return foundIndexRow;
				}
			}
		}

		// Check Up Up
		// Only check up up if the current row >= max word length
		if (currentRow >= kMaxWordLength)
		{
			// Scan for the word.
			for (int i = 0; i < kMaxWordLength; i++)
			{
				if (kMainString[currentRow - i][columnIndex] != kSubString[i])
				{
					foundIndexRow.found = false;
					break;
				}
				if (i == kMaxWordLength - kArrOffset)
				{
					// The word was found somewhere in the puzzle.
					foundIndexRow.found = true;
					// Replace chars in the filtered array.
					for (int j = 0; j < kMaxWordLength; j++)
					{
						filteredArray[currentRow - j][columnIndex] = kReplacementChar;
					}
					// Set values of where word was found
					foundIndexRow.index = columnIndex + kArrOffset;
					foundIndexRow.row = currentRow + kArrOffset;
					return foundIndexRow;
				}
			}
		}
		
		// Check Down Down
		// Only check down down if current row <= total num of rows in array - word length
		if (currentRow <= kMaxPuzzleRows - kMaxWordLength)
		{
			// Scan for the word.
			for (int i = 0; i < kMaxWordLength; i++)
			{
				if (kMainString[currentRow + i][columnIndex] != kSubString[i])
				{
					foundIndexRow.found = false;
					break;
				}
				if (i == kMaxWordLength - kArrOffset)
				{
					// The word was found somewhere in the puzzle.
					foundIndexRow.found = true;
					// Replace chars in the filtered array.
					for (int j = 0; j < kMaxWordLength; j++)
					{
						filteredArray[currentRow + j][columnIndex] = kReplacementChar;
					}
					// Set values of where word was found
					foundIndexRow.index = columnIndex + kArrOffset;
					foundIndexRow.row = currentRow + kArrOffset;
					return foundIndexRow;
				}
			}
		}

		// Check diagonal down right, diagonal down right
		// Only when the current row <= total num of rows - word length AND current column <= max array length - word length
		if (currentRow <= kMaxPuzzleRows - kMaxWordLength && columnIndex <= kMaxPuzzleColumns - kMaxWordLength)
		{
			// Scan for the word.
			for (int i = 0; i < kMaxWordLength; i++)
			{
				if (kMainString[currentRow + i][columnIndex + i] != kSubString[i])
				{
					foundIndexRow.found = false;
					break;
				}
				if (i == kMaxWordLength - kArrOffset)
				{
					// The word was found somewhere in the puzzle.
					foundIndexRow.found = true;
					// Replace chars in the filtered array.
					for (int j = 0; j < kMaxWordLength; j++)
					{
						filteredArray[currentRow + j][columnIndex + j] = kReplacementChar;
					}
					// Set values of where word was found
					foundIndexRow.index = columnIndex + kArrOffset;
					foundIndexRow.row = currentRow + kArrOffset;
					return foundIndexRow;
				}
			}			
		}

		// Check diagonal down left, diagonal down left
		// Only when the current row <= total num of rows - word length AND current column >= word length - array offset (1)
		if (currentRow <= kMaxPuzzleRows - kMaxWordLength && columnIndex >= kMaxWordLength - kArrOffset)
		{
			// Scan for the word.
			for (int i = 0; i < kMaxWordLength; i++)
			{
				if (kMainString[currentRow + i][columnIndex - i] != kSubString[i])
				{
					foundIndexRow.found = false;
					break;
				}
				if (i == kMaxWordLength - kArrOffset)
				{
					// The word was found somewhere in the puzzle.
					foundIndexRow.found = true;
					// Replace chars in the filtered array.
					for (int j = 0; j < kMaxWordLength; j++)
					{
						filteredArray[currentRow + j][columnIndex - j] = kReplacementChar;
					}
					// Set values of where word was found
					foundIndexRow.index = columnIndex + kArrOffset;
					foundIndexRow.row = currentRow + kArrOffset;
					return foundIndexRow;
				}
			}
		}

		// Check orthogonal left down
	}

	// If word is not found, return here.
	return foundIndexRow;
}

// Save 2D array to file
void SaveSecondFilteredPuzzle(ofstream& outputStream, const char kString[kMaxPuzzleRows][kMaxPuzzleColumns], const string kFileName)
{
	outputStream.open(kFileName);
	if (!outputStream)
	{
		cout << "Error:  File cannot be accessed/written to.\n";
		cout << "File: " << kFileName << endl;
	}
	else
	{
		for (int row = 0; row < kMaxPuzzleRows; row++)
		{
			for (int column = 0; column < kMaxPuzzleColumns; column++)
			{
				outputStream << kString[row][column];
			}
			// Write a new line after each row only if it isnt the last line to prevent an empty line at the end of the file.
			if (row < kMaxPuzzleRows - 1)
			{
				outputStream << "\n";
			}
		}
		// Check if operation succeeded.
		if (!outputStream)
		{
			cout << "Error: Write operation failed.\n" << endl;
		}
	}
	outputStream.close();
}

int main()
{
	// Input file stream
	ifstream inputStream;
	// Output file stream
	ofstream outputStream;

	// One Dimensional Array. //////////////////////////////////////////////////////////////
	cout << "Currently scanning 1D Puzzle.\n" << endl;

	// Array storing the characters present in the 1D puzzle file.
	char FirstPuzzle[kMaxPuzzleColumns];
	ReadFileIntoArray(inputStream, kOneDPuzzleFile, FirstPuzzle);

	// create a filtered array
	char FirstPuzzleFiltered[sizeof(FirstPuzzle)];
	// Copy the normal array into a filtered array that will then change.
	copy(begin(FirstPuzzle), end(FirstPuzzle), begin(FirstPuzzleFiltered));

	// Print out the puzzle
	cout << "First puzzle:\n";
	for (const char kChar : FirstPuzzle)
	{
		cout << kChar;
	}
	cout << "\n" << endl;

	// Array storing all the chars from the search file.
	char FirstSearch[kMaxWordAmount1D * kMaxWordLength];
	ReadFileIntoArray(inputStream, kOneDSearchFile, FirstSearch);

	// Separate the search array into vector of words
	vector<string> words;
	SeparateCharsToWords(kMaxWordLength, FirstSearch, words, (kMaxWordAmount1D * kMaxWordLength));

	// Print out the words to search for
	cout << "Search words:\n";
	for (const string kWord : words)
	{
		cout << kWord << " ";
	}
	cout << "\n" << endl;

	// 1D Search here
	cout << "Currently searching...\n";
	for (const string kWord : words)
	{
		FoundIndex foundIndex = FirstPuzzleSearch(FirstPuzzle, kWord.c_str(), FirstPuzzleFiltered);
		if (!foundIndex.found)
		{
			cout << kWord << ", Not Found.\n";
		}
		else
		{
			cout << kWord << ", Found, Location " << foundIndex.index << "\n";
		}
	}
	cout << endl;

	// Print out the filtered search puzzle
	cout << "Filtered file:\n";
	for (const char kChar : FirstPuzzleFiltered)
	{
		cout << kChar;
	}
	cout << "\n" << endl;

	// Save filtered search puzzle to file.
	cout << "Saving Filtered file to " << kOneDPuzzleFilteredFile << "\n" << endl;
	SaveFirstFilteredPuzzle(outputStream, FirstPuzzleFiltered, kOneDPuzzleFilteredFile);

	// Two Dimensional Array ////////////////////////////////////////////////////////////
	cout << "Currently scanning 2D Puzzle.\n" << endl;

	// Array storing the characters present in 2D puzzle file.
	char SecondPuzzle[kMaxPuzzleRows][kMaxPuzzleColumns];
	ReadFileInto2DArray(inputStream, kTwoDPuzzleFile, SecondPuzzle);

	// Print out the puzzle
	cout << "Second puzzle:\n";
	for (int row = 0; row < kMaxPuzzleRows; row++)
	{
		for (const char kChar : SecondPuzzle[row])
		{
			cout << kChar;
		}
		cout << "\n";
	}
	cout << endl;

	// Array storing all the chars from the search file.
	char SecondSearch[kMaxWordAmount2D * kMaxWordLength];
	ReadFileIntoArray(inputStream, kTwoDSearchFile, SecondSearch);

	// create a filtered array
	char SecondPuzzleFiltered[kMaxPuzzleRows][kMaxPuzzleColumns];
	// Copy the normal array into a filtered array that will then change.
	for (int row = 0; row < kMaxPuzzleRows; row++)
	{
		for (int column = 0; column < kMaxPuzzleColumns; column++)
		{
			SecondPuzzleFiltered[row][column] = SecondPuzzle[row][column];
		}
	}

	// Separate the search array into vector of words
	vector<string> words2D;
	SeparateCharsToWords(kMaxWordLength, SecondSearch, words2D, (kMaxWordAmount2D * kMaxWordLength));

	// Print out the words to search for
	cout << "Search words:\n";
	for (const string kWord : words2D)
	{
		cout << kWord << " ";
	}
	cout << "\n" << endl;

	// Second puzzle search.
	// Possible improvement : multithreading.
	cout << "Currently searching...\n";
	for (const string kWord : words2D)
	{
		FoundIndexRow foundIndexRow;
		for (int row = 0; row < kMaxPuzzleRows; row++)
		{
			foundIndexRow = SecondPuzzleSearch(SecondPuzzle, kWord.c_str(), SecondPuzzleFiltered, row);
			if (foundIndexRow.found)
			{
				break;
			}
		}
		if (foundIndexRow.found)
		{
			cout << kWord << ", Found, line " << foundIndexRow.row << ", location " << foundIndexRow.index << "\n";
		}
		else
		{
			cout << kWord << ", Not found\n";
		}
	}
	cout << endl;

	// Print out the filtered search puzzle
	cout << "Filtered file:\n";
	for (int row = 0; row < kMaxPuzzleRows; row++)
	{
		for (const char kChar : SecondPuzzleFiltered[row])
		{
			cout << kChar;
		}
		cout << "\n";
	}
	cout << endl;

	// Save filtered search puzzle to file.
	cout << "Saving Filtered file to " << kTwoDPuzzleFilteredFile << "\n" << endl;
	SaveSecondFilteredPuzzle(outputStream, SecondPuzzleFiltered, kTwoDPuzzleFilteredFile);

	// Pause and exit.
	const string kPauseCommand = "pause";
	system(kPauseCommand.c_str());
	const unsigned int kExitCode = 0;
	exit(kExitCode);
}