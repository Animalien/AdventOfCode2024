///////////////////////////
// Problem 6 - Guard Gallivant

#include <array>

#include "AdventOfCode2024.h"

class Problem6 : public ProblemBase
{
public:
    virtual int GetProblemNum() const override { return 6; }

    virtual void Run() override
    {
        RunOnData("Day6Example.txt", true);
        RunOnData("Day6Input.txt", false);
    }

private:
    static const std::string sm_facings;

    static const char sm_emptySpace = '.';
    static const char sm_obstacle = '#';
    static const char sm_visited = 'X';
    static const std::array<BigInt, 4> sm_xInc;
    static const std::array<BigInt, 4> sm_yInc;


    void RunOnData(const char* filename, bool verbose)
    {
        printf("Running on data in file '%s':\n", filename);

        StringList grid;
        ReadFileLines(filename, grid);

        BigInt startXPos = 0;
        BigInt startYPos = 0;
        BigInt startFacing = 0;
        FindGuardPositionAndFacing(grid, startXPos, startYPos, startFacing);

        grid[startYPos][startXPos] = sm_emptySpace;

        BigInt xPos = startXPos;
        BigInt yPos = startYPos;
        BigInt facing = startFacing;

        // first march the guard around until they move off the grid, using The Algorithm

        for (;;)
        {
            grid[yPos][xPos] = sm_facings[facing];

            if (verbose)
                PrintGrid(grid);

            const BigInt nextXPos = xPos + sm_xInc[facing];
            const BigInt nextYPos = yPos + sm_yInc[facing];

            if ((nextXPos < 0) || (nextXPos >= (BigInt)grid[yPos].size()) || (nextYPos < 0) || (nextYPos >= (BigInt)grid.size()))
            {
                grid[yPos][xPos] = sm_visited;
                break;
            }

            if (grid[nextYPos][nextXPos] == sm_obstacle)
            {
                facing = (facing + 1) % sm_facings.size();
            }
            else
            {
                grid[yPos][xPos] = sm_visited;
                xPos = nextXPos;
                yPos = nextYPos;
            }
        }

        // now count how many visited spaces there are

        BigInt numVisitedSpaces = 0;
        for (const std::string& gridLine: grid)
        {
            numVisitedSpaces += std::count(gridLine.begin(), gridLine.end(), sm_visited);
        }

        printf("  Num visited spaces = %lld\n\n", numVisitedSpaces);

        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        // now let's see where we can place an obstacle to cause a loop
        // note an obstacle must be placed only in a visited position -- marked by an X

        BigInt numObstacles = 0;
        for (BigInt obsY = 0; obsY < (BigInt)grid.size(); ++obsY)
        {
            for (BigInt obsX = 0; obsX < (BigInt)grid[yPos].size(); ++obsX)
            {
                if (grid[obsY][obsX] != sm_visited)
                    continue;

                if (DoesObstacleCauseLoop(startXPos, startYPos, startFacing, obsX, obsY, grid, verbose))
                    ++numObstacles;
            }
        }

        printf("  Num obstacles that will cause a guard loop = %lld\n\n", numObstacles);
    }

    void FindGuardPositionAndFacing(const StringList& grid, BigInt& xPos, BigInt& yPos, BigInt& facing)
    {
        for (BigInt y = 0; y < (BigInt)grid.size(); ++y)
        {
            for (BigInt x = 0; x < (BigInt)grid[y].size(); ++x)
            {
                facing = GetGuardFacingFromChar(grid[y][x]);
                if (facing >= 0)
                {
                    xPos = x;
                    yPos = y;
                    return;
                }
            }
        }

        // deep. shit.
        assert(false);
    }

    BigInt GetGuardFacingFromChar(char ch)
    {
        for (BigInt facing = 0; facing < (BigInt)sm_facings.length(); ++facing)
        {
            if (ch == sm_facings[facing])
                return facing;
        }

        return -1;
    }

    void PrintGrid(const StringList& grid)
    {
        for (const std::string& gridLine: grid)
            printf("  %s\n", gridLine.c_str());
        printf("\n");
    }

    typedef std::vector<BigIntList> VisitationRecordBank;

    bool DoesObstacleCauseLoop(BigInt guardXPos, BigInt guardYPos, BigInt guardFacing, BigInt obsX, BigInt obsY, StringList& grid, bool verbose)
    {
        static VisitationRecordBank visitations;
        visitations.resize(grid.size(), BigIntList(grid[0].size(), 0));
        for (BigIntList& visitRow: visitations)
        {
            visitRow.clear();
            visitRow.resize(grid[0].size(), 0);
        }

        grid[obsY][obsX] = sm_obstacle;
        bool looped = false;
        for (;;)
        {
            const BigInt currVisitation = 1LL << guardFacing;

            BigInt& visitation = visitations[guardYPos][guardXPos];
            if (visitation & currVisitation)
            {
                looped = true;
                break;
            }

            visitation |= currVisitation;

            const BigInt nextXPos = guardXPos + sm_xInc[guardFacing];
            const BigInt nextYPos = guardYPos + sm_yInc[guardFacing];

            if ((nextXPos < 0) || (nextXPos >= (BigInt)grid[guardYPos].size()) ||
                (nextYPos < 0) || (nextYPos >= (BigInt)grid.size()))
                break;

            if (grid[nextYPos][nextXPos] == sm_obstacle)
            {
                guardFacing = (guardFacing + 1) % sm_facings.size();
            }
            else
            {
                guardXPos = nextXPos;
                guardYPos = nextYPos;
            }
        }
        grid[obsY][obsX] = sm_visited;

        if (looped && verbose)
            PrintVisitationRecordGrid(grid, visitations);

        return looped;
    }

    void PrintVisitationRecordGrid(const StringList& grid, const VisitationRecordBank& visitations)
    {
        for (BigInt yPos = 0; yPos < (BigInt)grid.size(); ++yPos)
        {
            printf("  ");
            for (BigInt xPos = 0; xPos < (BigInt)grid[yPos].size(); ++xPos)
            {
                if (visitations[yPos][xPos] > 0)
                    printf("+");
                else
                    printf("%c", (int)grid[yPos][xPos]);
            }
            printf("\n");
        }
        printf("\n");
    }
};

const std::string Problem6::sm_facings = "^>v<";
const std::array<BigInt, 4> Problem6::sm_xInc = { 0, +1, 0, -1 };
const std::array<BigInt, 4> Problem6::sm_yInc = { -1, 0, +1, 0 };

Problem6 problem6;
