///////////////////////////
// Problem 10 - Hoof It

#include <array>

#include "AdventOfCode2024.h"

class Problem10 : public ProblemBase
{
public:
    virtual int GetProblemNum() const override { return 10; }

    virtual void Run() override
    {
        RunOnData("Day10Example.txt", true);
        RunOnData("Day10Input.txt", false);
    }

private:
    static const std::array<BigInt, 4> sm_xInc;
    static const std::array<BigInt, 4> sm_yInc;

    typedef BigIntUnorderedMap PeakMap;

    void RunOnData(const char* filename, bool verbose)
    {
        printf("Running on data in file '%s':\n", filename);

        StringList grid;
        ReadFileLines(filename, grid);

        PeakMap peakMap;

        BigInt totalPeakVisitationScore = 0;
        BigInt totalRating = 0;
        BigInt trailHeadCounter = 1;
        for (BigInt y = 0; y < (BigInt)grid.size(); ++y)
        {
            for (BigInt x = 0; x < (BigInt)grid[y].size(); ++x)
            {
                const BigInt currHeight = GetHeight(grid, x, y);
                if (currHeight != 0)
                    continue;

                if (verbose)
                    printf("  Starting a trail at trailhead %lld, %lld\n", x, y);

                BigInt numPeaksVisited = 0;
                BigInt trailheadRating = 0;
                Recurse(peakMap, trailHeadCounter, grid, x, y, currHeight, -1, numPeaksVisited, totalRating, verbose);
                totalPeakVisitationScore += numPeaksVisited;
                totalRating += trailheadRating;
                ++trailHeadCounter;

                if (verbose)
                    printf("    visited %lld peaks from here, trailhead rating = %lld\n", numPeaksVisited, trailheadRating);
            }
        }

        printf("  Total peak visitation score = %lld, total rating = %lld\n", totalPeakVisitationScore, totalRating);
    }

    void Recurse(
        PeakMap& peakMap,
        BigInt trailHeadCounter,
        const StringList& grid,
        BigInt x,
        BigInt y,
        BigInt currHeight,
        BigInt lastMovedDir,
        BigInt& numPeaksVisited,
        BigInt& totalRating,
        bool verbose)
    {
        if (currHeight >= 9)
        {
            BigInt& peakMapValue = GetPeakMapValue(peakMap, x, y);
            if (peakMapValue != trailHeadCounter)
            {
                peakMapValue = trailHeadCounter;
                ++numPeaksVisited;

                if (verbose)
                    printf("    visited peak at %lld, %lld\n", x, y);
            }

            ++totalRating;
            if (verbose)
                printf("    ended unique valid trail at %lld, %lld\n", x, y);

            return;
        }

        const BigInt nextDesiredHeight = currHeight + 1;
        const BigInt cameFromDir = (lastMovedDir >= 0) ? ((lastMovedDir + 2) % 4) : -1;

        for (BigInt dir = 0; dir < 4; ++dir)
        {
            if (dir == cameFromDir)
                continue;

            const BigInt nextX = x + sm_xInc[dir];
            const BigInt nextY = y + sm_yInc[dir];

            const BigInt nextHeight = GetHeight(grid, nextX, nextY);
            if (nextHeight != nextDesiredHeight)
                continue;

            Recurse(peakMap, trailHeadCounter, grid, nextX, nextY, nextHeight, dir, numPeaksVisited, totalRating, verbose);
        }
    }

    BigInt GetHeight(const StringList& grid, BigInt x, BigInt y)
    {
        if ((y < 0) || (y >= (BigInt)grid.size()))
            return -1;
        if ((x < 0) || (x >= (BigInt)grid[y].size()))
            return -1;

        return (BigInt)(BigUInt)(unsigned char)grid[y][x] - (BigInt)(BigUInt)(unsigned char)'0';
    }

    BigInt& GetPeakMapValue(PeakMap& peakMap, BigInt x, BigInt y)
    {
        const BigInt coordKey = (x << 8) | y;
        return peakMap[coordKey];
    }
};

const std::array<BigInt, 4> Problem10::sm_xInc = { 0, +1, 0, -1 };
const std::array<BigInt, 4> Problem10::sm_yInc = { -1, 0, +1, 0 };

Problem10 problem10;
