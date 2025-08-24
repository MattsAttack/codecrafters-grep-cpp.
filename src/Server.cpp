#include <iostream>
#include <string>
#include <cctype> // For isalnum()
#include <vector>

/*

My current algorithm only handles one regex char and checks entire input for it
Refactor to check each char for this pattern

*/

/*

Need to return true if we reach the end of regex


// Loops through every char in input_line from top level
bool regex(const std::string &input_line, const std::string &pattern)
{
    std::vector<string> patternVector = separate_pattern(patternVector, pattern);
    for (int absIndex = 0; absIndex < input_line.size(); absIndex++)
    {
        for (int relIndex = 0; relIndex <= pattern.size(); relIndex++)
        {
            // If we reach the end of the regex string and haven't broke, match found.
            if (relIndex == pattern.size()) return true; // TODO implement a better method for this

            if(input_line[relIndex] == pattern[relIndex]])
            {
                continue;
            }
            else
            {
                break; // Start next absIndex Loop. Verify this
            }
        }
    }
    return false;
}

void separate_pattern(std::vector<string> &patternVector,std::string &pattern) // Reimplement with pointer for memory efficiency (dont copy over a string). Can't do reference cause we're editing values
{
    std::vector<string> patternVector;
    for(int i = 0; i < pattern.size(); i++)
    {
        // Account for certain character classes
        if(pattern[i] == "\\")
        {
            patternVector.add(pattern[i:i+1]);
            i++;
        }
        else if(pattern[i] == "[")
        {
            do{
            patternVector.add(pattern[i]);
            i++;
            }while(pattern[i] != "]") // Could be simplified with find index to allow turn into one liner
        }
        else
        {
            patternVector.add(pattern[i]);
        }
    }
}

// Takes in a patternChar. Do this to allow for \*
bool match_pattern(const char c, const std::string patternChar)
{
    // Same code as before but with simplified checkers. No need to loop through things anymore
}
*/

// Loops through every char in input_line from top level

void separate_pattern(std::vector<std::string> &patternVector, const std::string &pattern)
{
    for (int i = 0; i < pattern.size(); i++)
    {
        // Account for certain character classes
        if (pattern[i] == '\\')
        {
            patternVector.push_back(pattern.substr(i, 2));
            i++;
        }
        else if (pattern[i] == '[')
        {
            // Calculate bracketLength by finding index of back bracket and subtracting by current index. May cause off by one error
            int bracketLength = pattern.find(']', i) - i + 1;
            std::cout << pattern.substr(i, bracketLength);
            patternVector.push_back(pattern.substr(i, bracketLength));
            i += bracketLength;
        }
        else
        {
            patternVector.push_back(pattern.substr(i, 1)); // Need to use substr
        }
    }
}

bool match_pattern(const std::string c, const std::string pattern)
{
    // If only 1 char, see if values align
    if (pattern.length() == 1)
    {
        return c == pattern;
    }
    else if (pattern == "\\d")
    {
        return c.find_first_of("0123456789") != std::string::npos; // npos is value returned if no matching index is found
    }
    else if (pattern == "\\w")
    {
        return (std::isalnum(c[0]) || c[0] == '_');
    }
    else if (pattern.at(0) == '[' && pattern.back() == ']') // TODO rewrite to be efficient with one char
    {
        bool negativeCharGroup = (pattern.at(1) == '^'); // Check for negative char group
        bool foundMatchingChar = false;

        // If using a negative char group, start at index 2 to account for ^
        int patternStartIndex = (negativeCharGroup) ? 2 : 1;
        std::string checkPatternChars;

        // Iterate through input string
        for (char val : c)
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
                    if (val == pattern.at(patternIndex))
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
// TODO update with function headers
bool regex(const std::string &input_line, const std::string &pattern)
{
    std::vector<std::string> patternVector;
    separate_pattern(patternVector, pattern);
    for (std::string pattern : patternVector)
    {
        std::cout << pattern << "\n";
    }
    for (int absIndex = 0; absIndex < input_line.size(); absIndex++)
    {
        for (int relIndex = 0; relIndex <= patternVector.size(); relIndex++)
        {
            // If we reach the end of the regex string and haven't broke, match found.
            if (relIndex == patternVector.size())
                return true; // TODO implement a better method for this

            if (match_pattern(input_line.substr(relIndex + absIndex, 1), patternVector.at(relIndex)))
            {
                continue;
            }
            else
            {
                break; // Start next absIndex Loop. Verify this
            }
        }
    }
    return false;
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

        if (regex(input_line, pattern))
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
