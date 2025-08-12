#include <iostream>
#include <string>
#include <cctype> // For isalnum()

bool match_pattern(const std::string &input_line, const std::string &pattern)
{
    if (pattern.length() == 1)
    {
        return input_line.find(pattern) != std::string::npos; // npos is the return value of find commands if they fail
    }
    else if (pattern == "\\d")
    {
        return input_line.find_first_of("0123456789") != std::string::npos; // Have this find a number
    }
    else if (pattern == "\\w")
    {
        // Need to verify char value is between 48-57 65-90 95 97-122
        for (char c : input_line)
        {
            if (std::isalnum(c) || c == '_')
            {
                return true;
            }
        }
        return false;
    }
    else if (pattern.at(0) == '[' && pattern.back() == ']')
    {
        bool negativeCharGroup = (pattern.at(1) == '^'); // Check for negative char group
        bool foundMatchingChar = false;

        // If using a negative char group, start at index 2 to account for ^
        int patternStartIndex = (negativeCharGroup) ? 2 : 1;
        std::string checkPatternChars;

        // Iterate through input string
        for (char c : input_line)
        {
            // Reset value of checkPatternChars for every char in input
            checkPatternChars = "";

            // Start at 1 and end early by 1 to account for brackets
            for (int patternIndex = patternStartIndex; patternIndex < pattern.size() - 1; patternIndex++)
            {
                // if char has not already been checked for match, add char to str and check it
                if (checkPatternChars.find(pattern.at(patternIndex)) == std::string::npos)
                {
                    // Append single unique character from pattern
                    checkPatternChars.push_back(pattern.at(patternIndex));

                    // Check if each char in pattern is in the input string
                    if (c == pattern.at(patternIndex))
                    {
                        if (!negativeCharGroup)
                        {
                            return true;
                        }
                        foundMatchingChar = true;
                    }
                }
            }

            // If a matching char was not found after iterating through our pattern on a input char, that means the input char is not in our pattern and is a negative character.
            if (!foundMatchingChar && negativeCharGroup)
            {
                return true;
            }
            foundMatchingChar = false;
        }
        return false;
    }
    else
    {
        throw std::runtime_error("Unhandled pattern " + pattern);
    }
}

int main(int argc, char *argv[])
{
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    std::cerr << "Logs from your program will appear here" << std::endl;

    if (argc != 3)
    {
        std::cerr << "Expected two arguments" << std::endl;
        return 1;
    }

    std::string flag = argv[1];
    std::string pattern = argv[2];

    if (flag != "-E")
    {
        std::cerr << "Expected first argument to be '-E'" << std::endl;
        return 1;
    }

    // Uncomment this block to pass the first stage
    //
    std::string input_line;
    std::getline(std::cin, input_line);

    try
    {
        if (match_pattern(input_line, pattern))
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
