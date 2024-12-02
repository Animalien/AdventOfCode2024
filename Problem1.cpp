///////////////////////////
// Problem 1 - Historian Histeria

#include "AdventOfCode2024.h"


class Problem1 : public ProblemBase
{
public:
    virtual int GetProblemNum() const override { return 1; }

    virtual void Run() override
    {
        RunOnData("Day1Example.txt", true);
        RunOnData("Day1Input.txt", false);
    }

private:
    void IncrementNumberCountInMap(BigIntMap& map, BigInt num)
    {
        BigIntMap::iterator mapFindIter = map.find(num);
        if (mapFindIter != map.end())
        {
            ++(mapFindIter->second);
        }
        else
        {
            map.insert(BigIntMap::value_type(num, 1));
        }
    }

    void RunOnData(const char* filename, bool verbose)
    {
        printf("Running on data in file '%s':\n", filename);

        StringList lines;
        ReadFileLines(filename, lines);

        BigIntList leftList, rightList;
        BigIntMap leftCountMap, leftRightCountMap;
        BigIntMultiSet rightNumberSet;

        for (const std::string& line: lines)
        {
            BigIntList intList;
            ParseBigIntList(line, intList, ' ');
            leftList.push_back(intList[0]);
            rightList.push_back(intList[1]);

            IncrementNumberCountInMap(leftCountMap, intList[0]);
            IncrementNumberCountInMap(leftRightCountMap, intList[1]);
            rightNumberSet.insert(intList[1]);
        }

        BigInt sumDists = 0;

        BigIntMap::const_iterator leftCountMapIter = leftCountMap.cbegin();
        BigIntMultiSet::const_iterator rightNumberSetIter = rightNumberSet.cbegin();
        
        for (; leftCountMapIter != leftCountMap.cend(); ++leftCountMapIter, ++rightNumberSetIter)
        {
            const BigInt dist = BigIntAbs(leftCountMapIter->first - *rightNumberSetIter);
            sumDists += dist;

            if (verbose)
            {
                printf(
                    "dist between %lld and %lld = %lld, sumDists = %lld\n",
                    leftCountMapIter->first,
                    *rightNumberSetIter,
                    dist,
                    sumDists);
            }
        }

        printf("Sum dists = %lld\n\n", sumDists);

        BigInt totalSimilarityScore = 0;

        for (const BigIntMap::value_type& pair: leftCountMap)
        {
            const BigInt leftRightCount = leftRightCountMap[pair.first];
            const BigInt simScore = pair.first * pair.second * leftRightCount;
            totalSimilarityScore += simScore;
            if (verbose)
                printf(
                    "left value %lld shows up in left column %lld times and right column %lld times, so similarity score = %lld, total sim score = %lld\n",
                    pair.first, pair.second, leftRightCount, simScore, totalSimilarityScore);
        }

        printf("Total similarity score = %lld\n\n", totalSimilarityScore);
    }
};

Problem1 problem1;
