///////////////////////////
// Problem 3 - Mull It Over

#include "AdventOfCode2024.h"

#include <cctype>


class Problem3 : public ProblemBase
{
public:
    virtual int GetProblemNum() const override { return 3; }

    virtual void Run() override
    {
        RunOnData("Day3Example.txt", true);
        RunOnData("Day3Input.txt", false);
    }

private:
    enum class State
    {
        Nothing,
        M,
        U,
        L,
        OpenParen,
        Arg1,
        Comma,
        Arg2,
    };

    void RunOnData(const char* filename, bool verbose)
    {
        printf("Running on data in file '%s':\n", filename);

        FILE* pFile = OpenInputFile(filename);
        assert(pFile);

        State state = State::Nothing;
        std::string arg1;
        std::string arg2;

        BigInt runningProductTotal = 0;

        int chInt = EOF;
        for (;;)
        {
            chInt = fgetc(pFile);
            if (chInt == EOF)
                break;

            char ch = (char)chInt;

            switch (state)
            {
                case State::Nothing:
                    if (ch == 'm')
                        state = State::M;
                    break;
                case State::M:
                    if (ch == 'u')
                        state = State::U;
                    else
                        state = State::Nothing;
                    break;
                case State::U:
                    if (ch == 'l')
                        state = State::L;
                    else
                        state = State::Nothing;
                    break;
                case State::L:
                    if (ch == '(')
                        state = State::OpenParen;
                    else
                        state = State::Nothing;
                    break;
                case State::OpenParen:
                    if (std::isdigit(chInt))
                    {
                        state = State::Arg1;
                        arg1 = ch;
                    }
                    else
                    {
                        state = State::Nothing;
                    }
                    break;
                case State::Arg1:
                    if (std::isdigit(chInt))
                    {
                        arg1 += ch;
                    }
                    else if (ch == ',')
                    {
                        state = State::Comma;
                    }
                    else
                    {
                        state = State::Nothing;
                    }
                    break;
                case State::Comma:
                    if (std::isdigit(chInt))
                    {
                        state = State::Arg2;
                        arg2 = ch;
                    }
                    else
                    {
                        state = State::Nothing;
                    }
                    break;
                case State::Arg2:
                    if (std::isdigit(chInt))
                    {
                        arg2 += ch;
                    }
                    else
                    {
                        if (ch == ')')
                        {
                            // finalize a valid instruction!

                            const BigInt i1 = StringToBigInt(arg1);
                            const BigInt i2 = StringToBigInt(arg2);

                            const BigInt product = i1 * i2;

                            runningProductTotal += product;

                            if (verbose)
                                printf(
                                    "found a valid mul instruction mul(%lld,%lld);  product = %lld and running product total now = %lld\n",
                                    i1,
                                    i2,
                                    product,
                                    runningProductTotal);
                        }

                        state = State::Nothing;
                    }
                    break;
                default:
                    assert(false);
                    break;
            }
        }

        printf("Product total of valid mul instructions = %lld\n\n", runningProductTotal);
    }
};

Problem3 problem3;
