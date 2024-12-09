///////////////////////////
// Problem 8 - Resonant Collinearity

#include "AdventOfCode2024.h"

class Problem8 : public ProblemBase
{
public:
    virtual int GetProblemNum() const override { return 8; }

    virtual void Run() override
    {
        RunOnData("Day8Example.txt", false, true);
        RunOnData("Day8Input.txt", false, false);

        RunOnData("Day8Example.txt", true, true);
        RunOnData("Day8Input.txt", true, false);
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

    void RunOnData(const char* filename, bool withHarmonics, bool verbose)
    {
        printf("Running on data in file '%s', %s harmonics:\n", filename, withHarmonics ? "with" : "without");

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

            for (BigInt i = 0; i < ((BigInt)coordsList.size()-1); ++i)
            {
                for (BigInt j = i + 1; j < (BigInt)coordsList.size(); ++j)
                {
                    const Coords& antenna1 = coordsList[i];
                    const Coords& antenna2 = coordsList[j];

                    const Coords diff = { antenna2.x - antenna1.x, antenna2.y - antenna1.y };

                    if (!withHarmonics)
                    {
                        const Coords antinode1 = { antenna1.x - diff.x, antenna1.y - diff.y };
                        const Coords antinode2 = { antenna2.x + diff.x, antenna2.y + diff.y };

                        bool newAntinode = false;
                        bool outOfBounds = false;

                        PlotAntinode(antinodeGrid, antinode1, newAntinode, outOfBounds);
                        if (newAntinode)
                            ++totalNumAntinodes;

                        PlotAntinode(antinodeGrid, antinode2, newAntinode, outOfBounds);
                        if (newAntinode)
                            ++totalNumAntinodes;
                    }
                    else
                    {
                        const Coords step = CalcMinimalDiff(antenna1, antenna2);

                        bool newAntinode = false;
                        bool outOfBounds = false;

                        // step forward from antenna1

                        Coords curr = antenna1;
                        for (;;)
                        {
                            PlotAntinode(antinodeGrid, curr, newAntinode, outOfBounds);
                            if (outOfBounds)
                                break;

                            if (newAntinode)
                                ++totalNumAntinodes;

                            curr.x += step.x;
                            curr.y += step.y;
                        }

                        // step backwards from antenna1

                        curr = Coords { antenna1.x - step.x, antenna1.y - step.y };
                        for (;;)
                        {
                            PlotAntinode(antinodeGrid, curr, newAntinode, outOfBounds);
                            if (outOfBounds)
                                break;

                            if (newAntinode)
                                ++totalNumAntinodes;

                            curr.x -= step.x;
                            curr.y -= step.y;
                        }
                    }
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

    void PlotAntinode(StringList& antinodeGrid, const Coords& coords, bool& newAntinode, bool& outOfBounds)
    {
        if ((coords.x < 0) || (coords.x >= (BigInt)antinodeGrid[0].size()) ||
            (coords.y < 0) || (coords.y >= (BigInt)antinodeGrid.size()))
        {
            newAntinode = false;
            outOfBounds = true;
            return;
        }

        if (antinodeGrid[coords.y][coords.x] == sm_antinode)
        {
            newAntinode = false;
            outOfBounds = false;
            return;
        }

        antinodeGrid[coords.y][coords.x] = sm_antinode;

        newAntinode = true;
        outOfBounds = false;
    }

    Coords CalcMinimalDiff(const Coords& coords1, const Coords& coords2)
    {
        const Coords diff = { coords2.x - coords1.x, coords2.y - coords1.y };
        if (diff.x == 0)
            return Coords { 0, (diff.y < 0) ? -1 : +1 };
        if (diff.y == 0)
            return Coords{ (diff.x < 0) ? -1 : +1, 0 };

        const BigInt diffGCF = GetGreatestCommonFactor(std::abs(diff.x), std::abs(diff.y));

        return Coords { diff.x / diffGCF, diff.y / diffGCF };
    }
};

Problem8 problem8;
