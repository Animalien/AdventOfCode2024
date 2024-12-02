///////////////////////////
// Problem 1 - Historian Histeria

#include "AdventOfCode2024.h"


class Problem1 : public ProblemBase
{
public:
    virtual int GetProblemNum() const override { return 1; }

    virtual void Run() override
    {
        RunThing("Day1Example.txt", true);
        RunThing("Day1Input.txt", false);
    }

private:
    void RunThing(const char* filename, bool verbose)
    {
        printf("Working on '%s':\n", filename);

        StringList lines;
        ReadFileLines(filename, lines);

        BigIntList leftList, rightList;

        for (const std::string& line: lines)
        {
            BigIntList intList;
            ParseBigIntList(line, intList, ' ');
            leftList.push_back(intList[0]);
            rightList.push_back(intList[1]);
        }

        std::sort(leftList.begin(), leftList.end());
        std::sort(rightList.begin(), rightList.end());

        BigInt sumDists = 0;

        for (BigInt i = 0; i < (BigInt)leftList.size(); ++i)
        {
            const BigInt dist = BigIntAbs(leftList[i] - rightList[i]);
            sumDists += dist;

            if (verbose)
                printf("dist between %lld and %lld = %lld, sumDists = %lld\n", leftList[i], rightList[i], dist, sumDists);
        }

        printf("Sum dists = %lld\n\n", sumDists);
    }

};

Problem1 problem1;
