///////////////////////////
// Problem 7 - Bridge Repair

#include "AdventOfCode2024.h"

class Problem7 : public ProblemBase
{
public:
    virtual int GetProblemNum() const override { return 7; }

    virtual void Run() override
    {
        RunOnData("Day7Example.txt", true);
        RunOnData("Day7Input.txt", false);
    }

private:
    void RunOnData(const char* filename, bool verbose)
    {
        printf("Running on data in file '%s':\n", filename);

        StringList lines;
        ReadFileLines(filename, lines);

        StringList lineFirstTokenize;
        BigIntList operandList;
        std::vector<bool> operatorList;  // true = mult

        BigInt totalValidTestValues = 0;

        for (const std::string& line: lines)
        {
            Tokenize(line, lineFirstTokenize, ':');

            const BigInt testValue = StringToBigInt(lineFirstTokenize[0]);

            ParseBigIntList(lineFirstTokenize[1], operandList, ' ');

            BigInt runningValue = operandList[0];
            bool valid = false;
            Recurse(testValue, 1, runningValue, operandList, valid, verbose);

            if (valid)
            {
                totalValidTestValues += testValue;

                if (verbose)
                    printf("  Equation for test value %lld is VALID\n", testValue);
            }
        }

        printf("  Total valid test values = %lld\n\n", totalValidTestValues);
    }

    void Recurse(
        BigInt testValue,
        BigInt operandIndex,
        BigInt runningValue,
        const BigIntList& operandList,
        bool& valid,
        bool verbose)
    {
        if (runningValue > testValue)
        {
            valid = false;
            return;
        }

        if (operandIndex >= (BigInt)operandList.size())
        {
            valid = (runningValue == testValue);
            return;
        }

        const BigInt nextOperand = operandList[operandIndex];

        // addition

        Recurse(testValue, operandIndex + 1, runningValue + nextOperand, operandList, valid, verbose);
        if (valid)
            return;

        // multiplication

        Recurse(testValue, operandIndex + 1, runningValue * nextOperand, operandList, valid, verbose);
    }
};

Problem7 problem7;
