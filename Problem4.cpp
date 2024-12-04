///////////////////////////
// Problem 4 - Ceres Search

#include <array>

#include "AdventOfCode2024.h"

class Problem4 : public ProblemBase
{
public:
    virtual int GetProblemNum() const override { return 4; }

    virtual void Run() override
    {
        RunOnData("Day4Example.txt", true);
        RunOnData("Day4Input.txt", false);
    }

private:
    void RunOnData(const char* filename, bool verbose)
    {
        printf("Running on data in file '%s':\n", filename);

        StringList lines;
        ReadFileLines(filename, lines);

        const BigInt maxY = (BigInt)lines.size() - 1;
        const BigInt maxX = (BigInt)lines[0].length() - 1;

        static const std::string wordToFind = "XMAS";
        static const BigInt wordToFindLen = wordToFind.length();
        static const BigInt wordToFindLenStep = wordToFindLen - 1;

        typedef std::array<BigInt,8> DirArray;

        static const DirArray xInc = { 0, +1, +1, +1, 0, -1, -1, -1 };
        static const DirArray yInc = { -1, -1, 0, +1, +1, +1, 0, -1 };
        static const BigInt numDirs = xInc.size();

        BigInt foundWordCount = 0;

        for (BigInt y = 0; y <= maxY; ++y)
        {
            for (BigInt x = 0; x <= maxX; ++x)
            {
                for (BigInt dir = 0; dir < numDirs; ++dir)
                {
                    // can the word even fit in this direction

                    const BigInt xEnd = x + (wordToFindLenStep * xInc[dir]);
                    if ((xEnd < 0) || (xEnd > maxX))
                        continue;

                    const BigInt yEnd = y + (wordToFindLenStep * yInc[dir]);
                    if ((yEnd < 0) || (yEnd > maxY))
                        continue;

                    // now test letters

                    bool foundWord = true;
                    for (BigInt letter = 0; letter < wordToFindLen; ++letter)
                    {
                        const BigInt xTest = x + (letter * xInc[dir]);
                        const BigInt yTest = y + (letter * yInc[dir]);

                        if (lines[yTest][xTest] != wordToFind[letter])
                        {
                            foundWord = false;
                            break;
                        }
                    }

                    if (foundWord)
                    {
                        ++foundWordCount;

                        if (verbose)
                            printf("  found word at %lld,%lld going in dir %lld\n", x, y, dir);
                    }
                }
            }
        }

        printf("  Total num times found word = %lld\n\n", foundWordCount);
    }
};

Problem4 problem4;
