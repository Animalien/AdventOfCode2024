///////////////////////////
// Problem 11 - Plutonian Pebbles

#include "AdventOfCode2024.h"

class Problem11 : public ProblemBase
{
public:
    virtual int GetProblemNum() const override { return 11; }

    virtual void Run() override
    {
        RunOnData("Day11Example.txt", 25, 6);
        RunOnData("Day11Input.txt", 25, 6);
        RunOnData("Day11Input.txt", 75, 6);
    }

private:
    class Pebble
    {
    public:
        Pebble() = default;
        Pebble(const std::string& stIn) : m_st(stIn), m_stValid(true), m_i(-1), m_iValid(false) {}
        Pebble(const std::string& stIn, BigInt intIn) : m_st(stIn), m_stValid(true), m_i(intIn), m_iValid(true) {}

        BigInt GetInt() const
        {
            if (!m_iValid && m_stValid)
            {
                m_i = StringToBigInt(m_st);
                m_iValid = true;
            }
            return m_i;
        }
        const std::string& GetString() const
        {
            if (!m_stValid && m_iValid)
            {
                m_st = std::to_string(m_i);
                m_stValid = true;
            }
            return m_st;
        }
        Pebble MakeEndSubStringPebble(BigInt pos) const
        {
            const std::string& st = GetString();
            const BigInt lastPos = st.length() - 1;
            while ((pos < lastPos) && (st[pos] == '0'))
                ++pos;
            return Pebble(st.substr(pos));
        }

        void Set(const std::string& stIn, BigInt intIn)
        {
            m_st = stIn;
            m_stValid = true;
            m_i = intIn;
            m_iValid = true;
        }
        void SetString(const std::string& stIn)
        {
            m_st = stIn;
            m_stValid = true;
            m_i = -1;
            m_iValid = false;
        }
        void SetInt(BigInt intIn)
        {
            m_i = intIn;
            m_iValid = true;
            m_st.clear();
            m_stValid = false;
        }

        void EraseStringEndDigits(BigInt numEndDigits)
        {
            GetString();
            m_st.erase(numEndDigits);
            m_i = -1;
            m_iValid = false;
        }

        BigInt GetNumDigits() const { return (BigInt)GetString().length(); }

        bool operator==(const Pebble& rhs) const
        {
            return (m_stValid && rhs.m_stValid && (m_st == rhs.m_st)) || (m_iValid && rhs.m_iValid && (m_i == rhs.m_i));
        }

        void Swap(Pebble& rhs)
        {
            std::swap(m_st, rhs.m_st);
            std::swap(m_stValid, rhs.m_stValid);
            std::swap(m_i, rhs.m_i);
            std::swap(m_iValid, rhs.m_iValid);
        }

    private:
        mutable std::string m_st;
        mutable bool m_stValid = false;
        mutable BigInt m_i = -1;
        mutable bool m_iValid = false;
    };

    typedef std::list<Pebble> PebbleList;

    void RunOnData(const char* filename, BigInt numBlinks, BigInt numBlinksToShow)
    {
        printf("Running on data in file '%s', with %lld blinks:\n", filename, numBlinks);

        std::string text;
        ReadFile(filename, text);

        // build the pebble list

        StringList initPebbleList;
        Tokenize(text, initPebbleList, ' ');

        PebbleList pebbleList;
        for (const std::string& pebble: initPebbleList)
        {
            pebbleList.push_back(Pebble(pebble));
        }

        // do the blinks

        for (BigInt i = 0; i < numBlinks; ++i)
        {
            for (PebbleList::iterator iter = pebbleList.begin(); iter != pebbleList.end(); ++iter)
            {
                // the rules

                static Pebble s_zeroPebble("0", 0);
                static Pebble s_onePebble("1", 1);

                if (*iter == s_zeroPebble)
                {
                    // simply set to "1"

                    *iter = s_onePebble;
                }
                else
                {
                    const BigInt numDigits = iter->GetNumDigits();
                    if ((numDigits & 1) == 0)
                    {
                        // insert a pebble before this one, and split the digits between the two

                        const BigInt halfNumDigits = numDigits / 2;
                        const PebbleList::iterator newPebbleIter = pebbleList.insert(iter, iter->MakeEndSubStringPebble(halfNumDigits));
                        iter->EraseStringEndDigits(halfNumDigits);
                        iter->Swap(*newPebbleIter);
                    }
                    else
                    {
                        // multiply by 2024

                        iter->SetInt(iter->GetInt() * 2024);
                    }
                }
            }

            if (i < numBlinksToShow)
            {
                printf("  pebbles after blink # %lld: ", i + 1);
                for (const Pebble& pebble: pebbleList)
                {
                    printf("%s ", pebble.GetString().c_str());
                }
                printf("\n");
            }
        }

        printf("  Num pebbles after all blinks = %lld\n\n", (BigInt)pebbleList.size());
    }
};

Problem11 problem11;
