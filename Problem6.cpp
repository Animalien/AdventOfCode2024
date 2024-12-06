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

    void RunOnData(const char* filename, bool verbose)
    {
        static const std::array<BigInt, 4> xInc = { 0, +1, 0, -1 };
        static const std::array<BigInt, 4> yInc = { -1, 0, +1, 0 };

        printf("Running on data in file '%s':\n", filename);

        StringList grid;
        ReadFileLines(filename, grid);

        BigInt xPos = 0;
        BigInt yPos = 0;
        BigInt facing = 0;
        FindGuardPositionAndFacing(grid, xPos, yPos, facing);

        grid[yPos][xPos] = sm_emptySpace;

        // first march the guard around until they move off the grid, using The Algorithm

        for (;;)
        {
            grid[yPos][xPos] = sm_facings[facing];

            if (verbose)
                PrintGrid(grid);

            const BigInt nextXPos = xPos + xInc[facing];
            const BigInt nextYPos = yPos + yInc[facing];

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
};

const std::string Problem6::sm_facings = "^>v<";

Problem6 problem6;
