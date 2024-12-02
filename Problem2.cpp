///////////////////////////
// Problem 2 - Red-Nosed Reports

#include "AdventOfCode2024.h"

class Problem2 : public ProblemBase
{
public:
    virtual int GetProblemNum() const override { return 2; }

    virtual void Run() override
    {
        RunOnData("Day2Example.txt", true);
        RunOnData("Day2Input.txt", false);
    }

private:
    void ProcessOneReport(const std::string& report, bool& isSafe, bool verbose)
    {
        if (verbose)
            printf("working on report: %s\n", report.c_str());

        BigIntList intList;
        ParseBigIntList(report, intList, ' ');

        isSafe = true;

        bool readFirstNum = false;
        bool determinedDirection = false;
        bool dirIsRising = false;
        BigInt prevNum = 0;
        for (BigInt num: intList)
        {
            if (readFirstNum)
            {
                const BigInt diff = num - prevNum;
                if (!determinedDirection)
                {
                    dirIsRising = (diff > 0);
                    determinedDirection = true;
                }

                const bool thisNumIsRising = (diff > 0);
                if (thisNumIsRising != dirIsRising)
                {
                    isSafe = false;

                    if (verbose)
                        printf(
                            "report is NOT safe because %lld -> %lld does not match established direction '%s'\n",
                            prevNum,
                            num,
                            dirIsRising ? "rising" : "falling");

                    return;
                }

                const BigInt absDiff = BigIntAbs(diff);
                if ((absDiff < 1) || (absDiff > 3))
                {
                    isSafe = false;

                    if (verbose)
                        printf(
                            "report is NOT safe because %lld -> %lld is not a valid step size\n",
                            prevNum,
                            num);

                    return;
                }
            }

            readFirstNum = true;
            prevNum = num;
        }
    }

    void RunOnData(const char* filename, bool verbose)
    {
        printf("Running on data in file '%s':\n", filename);

        StringList lines;
        ReadFileLines(filename, lines);

        BigInt numSafeReports = 0;
        for (const std::string& line: lines)
        {
            bool isSafe = false;
            ProcessOneReport(line, isSafe, verbose);

            if (isSafe)
                ++numSafeReports;
        }

        printf("Num safe reports = %lld\n\n", numSafeReports);
    }
};

Problem2 problem2;
