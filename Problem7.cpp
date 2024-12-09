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
        StringList operandStringList;
        BigIntList operandList;
        std::vector<bool> operatorList;  // true = mult


        // do the normal testing, with add and mul

        BigInt totalValidTestValues = 0;

        for (const std::string& line: lines)
        {
            Tokenize(line, lineFirstTokenize, ':');

            const BigInt testValue = StringToBigInt(lineFirstTokenize[0]);

            ParseBigIntList(lineFirstTokenize[1], operandList, ' ');

            BigInt runningValue = operandList[0];
            bool valid = false;
            Recurse(nullptr, testValue, 1, runningValue, nullptr, operandList, valid, verbose);

            if (valid)
            {
                totalValidTestValues += testValue;

                if (verbose)
                    printf("  Equation for test value %lld is VALID\n", testValue);
            }
        }

        printf("  Total valid test values = %lld\n\n", totalValidTestValues);


        // okay now do the testing with including joinability of adjacent numbers

        BigInt totalValidTestValuesJoinable = 0;

        for (const std::string& line: lines)
        {
            Tokenize(line, lineFirstTokenize, ':');

            const std::string& testValueString = lineFirstTokenize[0];
            const BigInt testValue = StringToBigInt(testValueString);

            Tokenize(lineFirstTokenize[1], operandStringList, ' ');
            ConvertStringListToBigIntList(operandStringList, operandList);

            BigInt runningValue = operandList[0];
            bool valid = false;
            Recurse(&testValueString, testValue, 1, runningValue, &operandStringList, operandList, valid, verbose);

            if (valid)
            {
                totalValidTestValuesJoinable += testValue;

                if (verbose)
                    printf("  Equation for test value %lld is VALID\n", testValue);
            }
        }

        printf("  Total valid test values with joinability = %lld\n\n", totalValidTestValuesJoinable);
    }

    void Recurse(
        const std::string* testValueString,
        BigInt testValue,
        BigInt operandIndex,
        BigInt runningValue,
        const StringList* operandStringList,
        const BigIntList& operandList,
        bool& valid,
        bool verbose)
    {
        static std::string runningValueString;

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

        Recurse(testValueString, testValue, operandIndex + 1, runningValue + nextOperand, operandStringList, operandList, valid, verbose);
        if (valid)
            return;

        // multiplication

        Recurse(
            testValueString,
            testValue,
            operandIndex + 1,
            runningValue * nextOperand,
            operandStringList,
            operandList,
            valid,
            verbose);
        if (valid)
            return;

        // joining

        if (operandStringList)
        {
            assert(testValueString);

            runningValueString = std::to_string(runningValue);
            const std::string& nextOperandString = (*operandStringList)[operandIndex];

            if ((runningValueString.length() + nextOperandString.length()) > testValueString->length())
            {
                valid = false;
                return;
            }

            runningValueString.append(nextOperandString);
            Recurse(
                testValueString,
                testValue,
                operandIndex + 1,
                StringToBigInt(runningValueString),
                operandStringList,
                operandList,
                valid,
                verbose);
        }
    }
};

Problem7 problem7;
