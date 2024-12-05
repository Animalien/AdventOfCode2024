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
        BigInt fixedMiddlePageNumberSum = 0;

        for (; lineIndex < (BigInt)lines.size(); ++lineIndex)
        {
            if (verbose)
                printf("  checking update: %s\n", lines[lineIndex].c_str());

            ParseBigIntList(lines[lineIndex], numList, ',');

            // go pair by pair, checking to make sure no rules are broken

            bool breaksAnyRules = false;

            // numRight starts from end of list and goes to index # 1
            // numLeft starts from just left of numRight, and goes to index # 0
            // therefore, numLeft is indeed always left of numRight in this update
            // we want to find out if any numRight is instead supposed to go before a numLeft
            //
            // a visual example of a pair being tested in the update list:
            //
            //      | - - - - numLeft - - - - numRight - - - |
            //

            const BigInt testIndexRightStart = (BigInt)numList.size() - 1;
            for (BigInt testIndexRight = testIndexRightStart; testIndexRight >= 1; --testIndexRight)
            {
                const BigInt numRight = numList[testIndexRight];

                for (BigInt testIndexLeft = testIndexRight - 1; testIndexLeft >= 0; --testIndexLeft)
                {
                    const BigInt numLeft = numList[testIndexLeft];

                    // check pair to see if numRight must come before numLeft

                    if (!NumbersAreInValidOrder(pairMapSet, numLeft, numRight))
                    {
                        // numRight is indeed supposed to come before numLeft, therefore this update is BAD

                        if (verbose)
                            printf("    this update breaks the rule \"%lld comes before %lld\"\n", numRight, numLeft);

                        breaksAnyRules = true;

                        // fix this update with respect to this pair

                        FixPairInUpdate(numList, pairMapSet, testIndexLeft, testIndexRight);

                        // now continue finding other pairs to fix
                        // (we can continue with these test indices because we already ensured everything else between them and to the right is still valid)
                    }
                }
            }

            const BigInt middlePageNumber = numList[numList.size() / 2];

            if (!breaksAnyRules)
            {
                middlePageNumberSum += middlePageNumber;

                if (verbose)
                    printf("    this update is VALID!  its middle page number is %lld\n", middlePageNumber);
            }
            else
            {
                fixedMiddlePageNumberSum += middlePageNumber;

                if (verbose)
                    printf("    this update was invalid, but we fixed it.  its new middle page number is %lld\n", middlePageNumber);
            }
        }

        printf(
            "  Middle page number sum = %lld, Fixed middle page number sum = %lld\n\n",
            middlePageNumberSum,
            fixedMiddlePageNumberSum);
    }

    bool NumbersAreInValidOrder(const BigIntUnorderedMapSet& pairMapSet, BigInt numLeft, BigInt numRight)
    {
        const BigIntUnorderedMapSet::const_iterator findIter = pairMapSet.find(numRight);

        // numRight is not even in rule set, so don't worry
        if (findIter == pairMapSet.end())
            return true;

        // numRight|numLeft are in rule set, so this pairing is bad
        if (findIter->second.count(numLeft) > 0)
            return false;

        // we are fine
        return true;
    }

    void FixPairInUpdate(BigIntList& numList, const BigIntUnorderedMapSet& pairMapSet, BigInt testIndexLeft, BigInt testIndexRight)
    {
        // fix in such a way that we don't invalidate any pairs when we move numbers
        // basically move the number at testIndexLeft as far to the right as we can, either just past testIndexRight, or until it can't be further moved without breaking rules
        // if we weren't able to fix it that way, move the number at testIndexRight as far left as we can until it goes past testIndexLeft

        //for (;;)
        //{
        //}
    }
};

Problem5 problem5;
