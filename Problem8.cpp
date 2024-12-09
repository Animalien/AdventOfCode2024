///////////////////////////
// Problem 8 - Resonant Collinearity

#include "AdventOfCode2024.h"

class Problem8 : public ProblemBase
{
public:
    virtual int GetProblemNum() const override { return 8; }

    virtual void Run() override
    {
        RunOnData("Day8Example.txt", true);
        RunOnData("Day8Input.txt", false);
    }

private:
    struct Coords
    {
        BigInt x;
        BigInt y;
    };

    typedef std::unordered_map<char, std::vector<Coords>> AntennaMap;

    static const char sm_empty = '.';
    static const char sm_antinode = '#';

    void RunOnData(const char* filename, bool verbose)
    {
        printf("Running on data in file '%s':\n", filename);

        StringList antennaGrid;
        ReadFileLines(filename, antennaGrid);

        StringList antinodeGrid = antennaGrid;

        AntennaMap antennaMap;

        // find all the antennas

        for (BigInt y = 0; y < (BigInt)antennaGrid.size(); ++y)
        {
            for (BigInt x = 0; x < (BigInt)antennaGrid[0].size(); ++x)
            {
                const char antennaType = antennaGrid[y][x];

                if (antennaType == sm_empty)
                    continue;

                antennaMap[antennaType].push_back({ x, y });
            }
        }

        // now plot and count all the antinodes

        BigInt totalNumAntinodes = 0;

        for (const AntennaMap::value_type& mapNode: antennaMap)
        {
            // per type

            const std::vector<Coords>& coordsList = mapNode.second;

            // pair up the antennas to find antinodes

            for (BigInt i = 0; i < (BigInt)coordsList.size(); ++i)
            {
                for (BigInt j = i + 1; j < (BigInt)coordsList.size(); ++j)
                {
                    const Coords& antenna1 = coordsList[i];
                    const Coords& antenna2 = coordsList[j];

                    const Coords diff = { antenna2.x - antenna1.x, antenna2.y - antenna1.y };

                    const Coords antinode1 = { antenna1.x - diff.x, antenna1.y - diff.y };
                    const Coords antinode2 = { antenna2.x + diff.x, antenna2.y + diff.y };

                    if (PlotAntinode(antinodeGrid, antinode1.x, antinode1.y))
                        ++totalNumAntinodes;

                    if (PlotAntinode(antinodeGrid, antinode2.x, antinode2.y))
                        ++totalNumAntinodes;
                }
            }
        }

        if (verbose)
        {
            for (const std::string& antinodeGridLine: antinodeGrid)
            {
                printf("  %s\n", antinodeGridLine.c_str());
            }
            printf("\n");
        }

        printf("  Total num antinodes = %lld\n\n", totalNumAntinodes);
    }

    bool PlotAntinode(StringList& antinodeGrid, BigInt x, BigInt y)
    {
        if ((x < 0) || (x >= (BigInt)antinodeGrid[0].size()) || (y < 0) || (y >= (BigInt)antinodeGrid.size()))
            return false;

        if (antinodeGrid[y][x] == sm_antinode)
            return false;

        antinodeGrid[y][x] = sm_antinode;

        return true;
    }
};

Problem8 problem8;
