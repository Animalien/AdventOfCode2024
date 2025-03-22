// Hi, this is my AdventOfCode 2024 stuff

#include <algorithm>
#include <assert.h>
#include <cctype>
#include <deque>
#include <limits.h>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


////////////////////////////
////////////////////////////
// Shared tools

////////////////////////////
// Basic Types

typedef long long BigInt;
typedef std::initializer_list<BigInt> BigIntInitList;
typedef std::vector<BigInt> BigIntList;
typedef std::vector<BigIntList> BigIntListList;
typedef std::list<BigInt> BigIntLinkedList;
typedef std::map<BigInt, BigInt> BigIntMap;
typedef std::unordered_map<BigInt, BigInt> BigIntUnorderedMap;
typedef std::set<BigInt> BigIntSet;
typedef std::multiset<BigInt> BigIntMultiSet;
typedef std::unordered_set<BigInt> BigIntUnorderedSet;
typedef std::deque<BigInt> BigIntDeque;
typedef std::unordered_map<BigInt, std::unordered_set<BigInt>> BigIntUnorderedMapSet;

typedef unsigned long long BigUInt;

const BigInt MAX_BIG_INT = LLONG_MAX;
const BigInt MIN_BIG_INT = LLONG_MIN;
const BigInt MAX_BIG_UINT = ULLONG_MAX;

typedef std::vector<bool> BoolList;

inline BigInt BigIntAbs(BigInt i) { return llabs(i); }

void PrintBigIntList(const BigIntList& intList, bool endLine = true);

BigInt GetGreatestCommonFactor(BigInt int1, BigInt int2);


////////////////////////////
// Factorization

class Factorization : public BigIntMap
{
public:
    Factorization() : BigIntMap() {}

    bool IsPrime() const { return ((size() == 1) && (begin()->second == 1)); }

    void Absorb(const Factorization& other)
    {
        for (auto iter = other.begin(); iter != other.end(); ++iter)
        {
            Absorb(iter->first, iter->second);
        }
    }

    BigInt CalcProduct() const
    {
        BigInt product = 1;
        for (auto iter = begin(); iter != end(); ++iter)
        {
            for (BigInt i = 0; i < iter->second; ++i)
            {
                product *= iter->first;
            }
        }
        return product;
    }

    void PrintFactors() const
    {
        for (auto iter = begin(); iter != end(); ++iter)
        {
            printf("(%lldn of %lld) ", iter->second, iter->first);
        }
    }

    BigInt CalcNumDivisors() const
    {
        if (IsPrime())
        {
            // if prime, then number of divisors is simply:  1, and itself
            return 2;
        }

        BigInt numDivisors = 1;
        // the number of divisors will be the numbers of combinations of prime factors.
        // in a given divisor, each prime factor can be included from 0 to N times, where
        // N is the number of times that prime factor exists in the original number.
        // (the divisor with ZERO of any prime factors included, is the divisor 1, which every number has.)
        for (auto iter = begin(); iter != end(); ++iter)
        {
            numDivisors *= (iter->second + 1);
        }
        // add 1 more for the original number, being one of its own divisors
        numDivisors += 1;

        return numDivisors;
    }

    BigInt GetGreatestCommonFactor(const Factorization& rhs) const
    {
        BigInt gcf = 1;

        BigIntMap::const_iterator lhsIter = this->cbegin();
        BigIntMap::const_iterator rhsIter = rhs.cbegin();

        for (;;)
        {
            if ((lhsIter == this->cend()) || (rhsIter == rhs.cend()))
                return gcf;

            if (lhsIter->first == rhsIter->first)
            {
                const BigInt minNumFactors = std::min(lhsIter->second, rhsIter->second);
                for (BigInt i = 0; i < minNumFactors; ++i)
                {
                    gcf *= lhsIter->first;
                }
                ++lhsIter;
                ++rhsIter;
            }
            else if (lhsIter->first < rhsIter->first)
            {
                ++lhsIter;
            }
            else
            {
                ++rhsIter;
            }
        }
    }

private:
    void Absorb(BigInt number, BigInt numFactors)
    {
        auto iter = find(number);
        if (iter != end())
        {
            iter->second = std::max(iter->second, numFactors);
        }
        else
        {
            insert(value_type(number, numFactors));
        }
    }
};

class FactorizationCache : public std::map<BigInt, Factorization>
{
public:
    FactorizationCache() : std::map<BigInt, Factorization>() {}

    void PopulateCache(BigInt num) { Factorize(num * 2); }

    const Factorization& Factorize(BigInt num)
    {
        iterator fiter = find(num);
        if (fiter == end())
        {
            fiter = NewFactorize(num);
        }

        return fiter->second;
    }

private:
    iterator NewFactorize(BigInt num)
    {
        auto newValue = insert(value_type(num, Factorization()));
        iterator newIter = newValue.first;
        Factorization& newFactorization = newIter->second;

        const BigInt halfNum = num / 2;
        BigInt prodRemaining = num;
        for (BigInt i = 2; i <= halfNum; ++i)
        {
            const Factorization& f = Factorize(i);
            if (f.IsPrime())
            {
                // i is prime, so see how many i factors fit into this number
                BigInt numFactors = 0;
                for (;;)
                {
                    const lldiv_t divRem = lldiv(prodRemaining, i);
                    if (divRem.rem != 0)
                    {
                        break;
                    }
                    ++numFactors;
                    prodRemaining = divRem.quot;
                }
                if (numFactors > 0)
                {
                    newFactorization.emplace(i, numFactors);
                }
            }

            if (prodRemaining == 1)
            {
                break;
            }
        }
        if (newFactorization.empty())
        {
            newFactorization.emplace(num, 1);
        }

        return newIter;
    }
};

extern FactorizationCache g_factorizationCache;


////////////////////////////
// Strings

typedef std::vector<std::string> StringList;
typedef std::vector<StringList> StringListList;
typedef std::set<std::string> StringSet;
typedef std::map<std::string, std::string> StringMap;
typedef std::unordered_set<std::string> UnorderedStringSet;

FILE* OpenInputFile(const char* fileName);
void ReadFile(const char* fileName, std::string& text);
void ReadFileLines(const char* fileName, StringList& lines);
void Tokenize(const std::string& st, StringList& tokens, char delim);

inline BigInt StringToBigInt(const std::string& st) { return atoll(st.c_str()); }

template <typename ListType>
void ParseBigIntList(const std::string& st, ListType& intList, char delim)
{
    StringList tokens;
    Tokenize(st, tokens, delim);

    intList.clear();
    for (const auto& intString: tokens)
    {
        intList.push_back(StringToBigInt(intString));
    }
}

void ConvertStringListToBigIntList(const StringList& stringList, BigIntList& intList);
bool ParseNextBigInt(const char*& st, BigInt& num, bool checkForNegation = false);
bool StringHasDigits(const std::string& st, BigInt start = 0, BigInt end = -1);
bool StringHasLowerCaseAlphaNumeric(const std::string& st, BigInt start = 0, BigInt end = -1);
bool StringIsIntWithinRange(const std::string& st, BigInt min, BigInt max, BigInt skipLastChars = 0);
bool StringIsIntWithinRangeAndSuffix(const std::string& st, BigInt min, BigInt max, const char* suffix, BigInt suffixLen);
void MultiplyStringLists(StringList& origList, const StringList& rhsList);


////////////////////////////
// Sets

template<typename T>
void IntersectSet(std::set<T>& lhs, const std::set<T>& rhs)
{
    std::set<T> newSet;

    auto iterLeft = lhs.cbegin();
    const auto iterLeftEnd = lhs.cend();

    auto iterRight = rhs.cbegin();
    const auto iterRightEnd = rhs.cend();

    for (;;)
    {
        if (iterLeft == iterLeftEnd)
            break;
        if (iterRight == iterRightEnd)
            break;

        const T& leftValue = *iterLeft;
        const T& rightValue = *iterRight;

        if (leftValue == rightValue)
        {
            newSet.insert(*iterLeft);
            ++iterLeft;
            ++iterRight;
        }
        else if (leftValue < rightValue)
        {
            ++iterLeft;
        }
        else
        {
            ++iterRight;
        }
    }

    lhs.swap(newSet);
}


////////////////////////////
// Circular Buffer

template<typename T, BigInt NUM_CAPACITY_BITS>
class CircularBuffer
{
public:
    CircularBuffer() : m_buffer(), m_numInBuffer(0), m_readIndex(0), m_writeIndex(0) {}
    CircularBuffer(const CircularBuffer& other) : m_buffer(), m_numInBuffer(0), m_readIndex(0), m_writeIndex(0) { *this = other; }

    BigInt GetCapacity() const { return CAPACITY; }
    BigInt GetSize() const { return m_numInBuffer; }
    bool IsEmpty() const { return m_numInBuffer == 0; }

    void Clear() { m_numInBuffer = m_readIndex = m_writeIndex = 0; }

    CircularBuffer& operator=(const CircularBuffer& rhs)
    {
        Clear();
        for (const T& item: rhs)
            Write(item);
        return *this;
    }

    const T& PeekRead() const
    {
        assert(!IsEmpty());
        return m_buffer[m_readIndex];
    }
    T Read()
    {
        const T retVal = PeekRead();
        PopRead();
        return retVal;
    }
    void PopRead()
    {
        assert(!IsEmpty());
        m_readIndex = IncrementIndex(m_readIndex);
        --m_numInBuffer;
    }
    void PopWrite()
    {
        assert(!IsEmpty());
        m_writeIndex = DecrementIndex(m_writeIndex);
        --m_numInBuffer;
    }

    void Write(const T& newValue)
    {
        m_buffer[m_writeIndex] = newValue;
        m_writeIndex = IncrementIndex(m_writeIndex);
        ++m_numInBuffer;
    }

    struct ConstIterator
    {
        const CircularBuffer& buffer;
        BigInt index;

        ConstIterator(const CircularBuffer& b, BigInt i) : buffer(b), index(i) {}
        ConstIterator(const ConstIterator& other) : buffer(other.buffer), index(other.index) {}

        bool operator==(const ConstIterator& rhs) const { return ((&buffer == &rhs.buffer) && (index == rhs.index)); }
        bool operator!=(const ConstIterator& rhs) const { return ((&buffer != &rhs.buffer) || (index != rhs.index)); }

        const T& operator*() const { return buffer.m_buffer[index]; }

        void operator++() { index = StepIndex(index, +1); }
    };
    struct Iterator
    {
        CircularBuffer& buffer;
        BigInt index;

        Iterator(CircularBuffer& b, BigInt i) : buffer(b), index(i) {}
        Iterator(const Iterator& other) : buffer(other.buffer), index(other.index) {}

        bool operator==(const Iterator& rhs) const { return ((&buffer == &rhs.buffer) && (index == rhs.index)); }
        bool operator!=(const Iterator& rhs) const { return ((&buffer != &rhs.buffer) || (index != rhs.index)); }

        T& operator*() const { return buffer.m_buffer[index]; }

        void operator++() { index = StepIndex(index, +1); }
    };

    bool operator==(const CircularBuffer& rhs) const
    {
        if (GetSize() != rhs.GetSize())
            return false;
        ConstIterator iter = cbegin();
        const ConstIterator iterEnd = cend();
        ConstIterator rhsIter = rhs.cbegin();
        for (; iter != iterEnd; ++iter, ++rhsIter)
        {
            if (*iter != *rhsIter)
                return false;
        }

        return true;
    }
    bool operator!=(const CircularBuffer& rhs) const { return !(*this == rhs); }

    // STL

    bool empty() const { return IsEmpty(); }
    size_t size() const { return GetSize(); }
    void resize(size_t newSize)
    {
        const BigInt sizeDiff = (BigInt)newSize - GetSize();
        if (sizeDiff > 0)
        {
            while (sizeDiff > 0)
            {
                Write(T());
                --sizeDiff;
            }
        }
        else
        {
            while (sizeDiff < 0)
            {
                PopWrite();
                ++sizeDiff;
            }
        }
    }
    const T& front() const { return PeekRead(); }
    void pop_front() { PopRead(); }
    void push_back(const T& newValue) { Write(newValue); }
    ConstIterator cbegin() const { return ConstIterator(*this, m_readIndex); }
    ConstIterator cend() const { return ConstIterator(*this, m_writeIndex); }
    ConstIterator begin() const { return ConstIterator(*this, m_readIndex); }
    ConstIterator end() const { return ConstIterator(*this, m_writeIndex); }
    Iterator begin() { return Iterator(*this, m_readIndex); }
    Iterator end() { return Iterator(*this, m_writeIndex); }

private:
    static const BigInt CAPACITY = 1LL << NUM_CAPACITY_BITS;
    static const BigInt CAPACITY_MASK = CAPACITY - 1;

    static BigInt IncrementIndex(BigInt index) { return StepIndex(index, +1); }
    static BigInt DecrementIndex(BigInt index) { return StepIndex(index, -1); }
    static BigInt StepIndex(BigInt index, BigInt step) { return (index + step + CAPACITY) & CAPACITY_MASK; }

    T m_buffer[CAPACITY];
    BigInt m_numInBuffer;
    BigInt m_readIndex;
    BigInt m_writeIndex;
};


////////////////////////////
// Permutation Iterator

template<typename T>
class PermutationIterator
{
public:
    PermutationIterator(T firstValue, T lastValue, BigInt startIndex = 0)
    {
        for (BigInt i = startIndex, value = firstValue; value<=lastValue; ++i, ++value)
            m_list.push_back(value);
        m_haveMorePermutations = true;
    }
    PermutationIterator(const std::vector<T>& sourceList, BigInt startIndex = 0)
    {
        for (BigInt i = startIndex; i < (BigInt)sourceList.size(); ++i)
            m_list.push_back(sourceList[i]);
        m_haveMorePermutations = true;
    }

    const std::vector<T>& GetCurrentPermutation() const { return m_list; }

    bool HaveMorePermutations() const { return m_haveMorePermutations; }

    void Step()
    {
        m_haveMorePermutations = std::next_permutation(m_list.begin(), m_list.end());
    }


private:
    std::vector<T> m_list;
    bool m_haveMorePermutations = false;
};

typedef PermutationIterator<BigInt> BigIntListPermutationIterator;

void TestPermutationIterator(BigInt numEntries);


////////////////////////////
// StringNum
//      An integer number represented by a string.
//      Can do addition + multiplication.

class StringNum
{
};


////////////////////////////
// Problem Harness

class ProblemBase
{
public:
    inline ProblemBase();

    virtual int GetProblemNum() const = 0;
    virtual void Run() = 0;

private:
};

class ProblemRegistry
{
public:
    static void RegisterProblem(ProblemBase* newProblem) { m_problemsToBeRegistered.push_back(newProblem); }

    static void Init()
    {
        for (ProblemBase* problem: m_problemsToBeRegistered)
        {
            m_problems[problem->GetProblemNum()] = problem;
        }
        m_problemsToBeRegistered.clear();
    }

    static bool RunProblem(int number)
    {
        auto iter = m_problems.find(number);
        if (iter == m_problems.end())
            return false;

        iter->second->Run();
        return true;
    }

private:
    static std::vector<ProblemBase*> m_problemsToBeRegistered;
    static std::map<int, ProblemBase*> m_problems;
};

inline ProblemBase::ProblemBase()
{
    ProblemRegistry::RegisterProblem(this);
}
