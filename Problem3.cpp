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
        RunOnData("Day3Example.txt", false, true);
        RunOnData("Day3Input.txt", false, false);
        RunOnData("Day3Example2.txt", true, true);
        RunOnData("Day3Input.txt", true, false);
    }

private:
    enum class State
    {
        Nothing,

        M,
        U,
        L,
        OpenParenMul,
        Arg1,
        Comma,
        Arg2,

        D,
        O,
        N,
        Apostrophe,
        T,
        OpenParenDo,
        OpenParenDont,
    };

    void RunOnData(const char* filename, bool enableDisable, bool verbose)
    {
        printf("Running on data in file '%s'%s:\n", filename, enableDisable ? "with mul enable/disable" : "");

        FILE* pFile = OpenInputFile(filename);
        assert(pFile);

        State state = State::Nothing;
        std::string arg1;
        std::string arg2;

        BigInt runningProductTotal = 0;

        bool mulsAreEnabled = true;

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
                    else if (enableDisable && (ch == 'd'))
                        state = State::D;
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
                        state = State::OpenParenMul;
                    else
                        state = State::Nothing;
                    break;
                case State::OpenParenMul:
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
                            if (enableDisable && !mulsAreEnabled)
                            {
                                if (verbose)
                                {
                                    const BigInt i1 = StringToBigInt(arg1);
                                    const BigInt i2 = StringToBigInt(arg2);

                                    printf(
                                        "  found a valid mul instruction mul(%lld,%lld), but ignoring because muls are currently disabled\n",
                                        i1,
                                        i2);
                                }
                            }
                            else
                            {
                                // finalize a valid instruction!

                                const BigInt i1 = StringToBigInt(arg1);
                                const BigInt i2 = StringToBigInt(arg2);

                                const BigInt product = i1 * i2;

                                runningProductTotal += product;

                                if (verbose)
                                    printf(
                                        "  found a valid mul instruction mul(%lld,%lld);  product = %lld and running product total now = %lld\n",
                                        i1,
                                        i2,
                                        product,
                                        runningProductTotal);
                            }
                        }

                        state = State::Nothing;
                    }
                    break;

                case State::D:
                    if (ch == 'o')
                        state = State::O;
                    else
                        state = State::Nothing;
                    break;
                case State::O:
                    if (ch == '(')
                        state = State::OpenParenDo;
                    else if (ch == 'n')
                        state = State::N;
                    else
                        state = State::Nothing;
                    break;
                case State::N:
                    if (ch == '\'')
                        state = State::Apostrophe;
                    else
                        state = State::Nothing;
                    break;
                case State::Apostrophe:
                    if (ch == 't')
                        state = State::T;
                    else
                        state = State::Nothing;
                    break;
                case State::T:
                    if (ch == '(')
                        state = State::OpenParenDont;
                    else
                        state = State::Nothing;
                    break;
                case State::OpenParenDo:
                    mulsAreEnabled = true;
                    if (verbose)
                        printf("  enabling muls\n");
                    state = State::Nothing;
                    break;
                case State::OpenParenDont:
                    mulsAreEnabled = false;
                    if (verbose)
                        printf("  disabling muls\n");
                    state = State::Nothing;
                    break;

                default:
                    assert(false);
                    break;
            }
        }

        printf("  Product total of valid mul instructions = %lld\n\n", runningProductTotal);
    }
};

Problem3 problem3;
