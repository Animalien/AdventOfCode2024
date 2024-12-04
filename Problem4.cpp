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
        BigInt foundFormationCount = 0;

        for (BigInt y = 0; y <= maxY; ++y)
        {
            for (BigInt x = 0; x <= maxX; ++x)
            {
                for (BigInt dir = 0; dir < numDirs; ++dir)
                {
                    //
                    // Find XMAS word
                    // 

                    // can the word even fit in this direction

                    const BigInt xEnd = x + (wordToFindLenStep * xInc[dir]);
                    const BigInt yEnd = y + (wordToFindLenStep * yInc[dir]);

                    if ((xEnd >= 0) && (xEnd <= maxX) && (yEnd >= 0) && (yEnd <= maxY))
                    {
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

                //
                // Find X-MAS formation
                //

                if ((x == 0) || (x == maxX) || (y == 0) || (y == maxY))
                    continue;
                if (lines[y][x] != 'A')
                    continue;

                BigUInt mask = 0;
                bool bad = false;
                for (BigInt dir = 1; dir < numDirs; dir += 2)
                {
                    mask <<= 1;

                    const char ch = lines[y + yInc[dir]][x + xInc[dir]];
                    if (ch == 'S')
                    {
                        mask = mask | 1;
                    }
                    else if (ch != 'M')
                    {
                        bad = true;
                        break;
                    }
                }

                if (bad)
                    continue;

                // mask can be:  0011, 0110, 1100, or 1001
                if ((mask != 3) && (mask != 6) && (mask != 12) && (mask != 9))
                    continue;

                ++foundFormationCount;

                if (verbose)
                    printf("  found formation at %lld,%lld\n", x, y);
            }
        }

        printf("  Total num times found word = %lld, total num times found formation = %lld\n\n", foundWordCount, foundFormationCount);
    }
};

Problem4 problem4;
