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
    void ProcessOneReport(const BigIntList& reportNumList, bool& isSafe, bool verbose)
    {
        isSafe = true;

        bool readFirstNum = false;
        bool determinedDirection = false;
        bool dirIsRising = false;
        BigInt prevNum = 0;
        for (BigInt num: reportNumList)
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
        BigInt numSafeReportsWithDampener = 0;

        BigIntList reportNumList;
        BigIntList altReportNumList;

        for (const std::string& line: lines)
        {
            bool isSafe = false;
            bool isSafeWithProblemDampener = false;

            if (verbose)
                printf("working on report: %s\n", line.c_str());

            BigIntList intList;
            ParseBigIntList(line, reportNumList, ' ');

            ProcessOneReport(reportNumList, isSafe, verbose);

            if (isSafe)
            {
                // safe is safe!

                ++numSafeReports;
                ++numSafeReportsWithDampener;

                if (verbose)
                    printf("just plain SAFE\n");
            }
            else
            {
                // see if we can eke out a with-dampener safety (omitting a single entry)

                altReportNumList = reportNumList;
                altReportNumList.pop_back();

                BigInt indexToPatch = altReportNumList.size() - 1;
                for (;;)
                {
                    if (verbose)
                    {
                        printf("working on snipped version of report: ");
                        for (BigInt num: altReportNumList)
                        {
                            printf("%lld ", num);
                        }
                        printf("\n");
                    }

                    ProcessOneReport(altReportNumList, isSafeWithProblemDampener, verbose);
                    if (isSafeWithProblemDampener)
                    {
                        ++numSafeReportsWithDampener;
                        if (verbose)
                            printf("SAFE because we DID find a with-problem-dampener safe option\n");
                        break;
                    }

                    if (indexToPatch < 0)
                    {
                        if (verbose)
                            printf("UNSAFE because could not find a with-problem-dampener safe option\n");
                        break;
                    }

                    // patch alt list, to test next one-item-removed combination
                    altReportNumList[indexToPatch] = reportNumList[indexToPatch + 1];
                    --indexToPatch;
                }
            }
        }

        printf("Num safe reports = %lld, num safe reports with problem dampener = %lld\n\n", numSafeReports, numSafeReportsWithDampener);
    }
};

Problem2 problem2;
