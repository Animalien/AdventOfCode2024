///////////////////////////
// Problem 5 - Print Queue

#include "AdventOfCode2024.h"

class Problem5 : public ProblemBase
{
public:
    virtual int GetProblemNum() const override { return 5; }

    virtual void Run() override
    {
        RunOnData("Day5Example.txt", true);
        RunOnData("Day5Input.txt", false);
    }

private:
    void RunOnData(const char* filename, bool verbose)
    {
        printf("Running on data in file '%s':\n", filename);

        StringList lines;
        ReadFileLines(filename, lines);

        // populate the rule map

        BigIntUnorderedMapSet pairMapSet;

        BigInt lineIndex = 0;
        BigIntList numList;
        for (; lineIndex < (BigInt)lines.size(); ++lineIndex)
        {
            ParseBigIntList(lines[lineIndex], numList, '|');
            if (numList.size() == 0)
            {
                ++lineIndex;
                break;
            }

            assert(numList.size() == 2);

            pairMapSet[numList[0]].insert(numList[1]);
        }

        // go through the updates

        BigInt middlePageNumberSum = 0;

        for (; lineIndex < (BigInt)lines.size(); ++lineIndex)
        {
            if (verbose)
                printf("  checking update: %s\n", lines[lineIndex].c_str());

            ParseBigIntList(lines[lineIndex], numList, ',');

            // go pair by pair, checking to make sure no rules are broken

            bool breaksAnyRules = false;

            // num 1 starts from end of list and goes to index # 1
            // num 2 starts from just left of num 1, and goes to index # 0
            // therefore, num 2 is before num 1 in this update
            // we want to find out if any num 1 is instead supposed to go before a num 2
            //
            // a visual example of a pair being tested in the update list:
            // 
            //      | - - - - num2 - - - - num1 - - - |
            //

            const BigInt testIndex1Start = (BigInt)numList.size() - 1;
            for (BigInt testIndex1 = testIndex1Start; testIndex1 >= 1; --testIndex1)
            {
                const BigInt num1 = numList[testIndex1];

                for (BigInt testIndex2 = testIndex1 - 1; testIndex2 >= 0; --testIndex2)
                {
                    const BigInt num2 = numList[testIndex2];

                    // check pair to see if num1 must come before num2

                    const BigIntUnorderedMapSet::const_iterator findIter = pairMapSet.find(num1);
                    if ((findIter != pairMapSet.end()) && (findIter->second.count(num2) > 0))
                    {
                        // num1 is indeed supposed to come before num2, therefore this update is BAD

                        if (verbose)
                            printf("    this update breaks the rule \"%lld comes before %lld\"\n", num1, num2);

                        breaksAnyRules = true;
                        break;
                    }
                }

                if (breaksAnyRules)
                    break;
            }

            if (!breaksAnyRules)
            {
                const BigInt middlePageNumber = numList[numList.size() / 2];

                middlePageNumberSum += middlePageNumber;

                if (verbose)
                    printf("    this update is VALID!  its middle page number is %lld\n", middlePageNumber);
            }
        }

        printf("  Middle page number sum = %lld\n\n", middlePageNumberSum);
    }
};

Problem5 problem5;
