// Hi, this is my AdventOfCode 2024 stuff

#include "AdventOfCode2024.h"


////////////////////////////
////////////////////////////
// Shared tools

FactorizationCache g_factorizationCache;


////////////////////////////
// Strings

FILE* OpenInputFile(const char* fileName)
{
    static const char* const fileNameBase = "..\\Input\\";

    std::string fullFileName = fileNameBase;
    fullFileName += fileName;
    return fopen(fullFileName.c_str(), "rt");
}

void ReadFileLines(const char* fileName, StringList& lines)
{
    lines.clear();

    FILE* pFile = OpenInputFile(fileName);
    assert(pFile);

    char string[1024];
    for (;;)
    {
        const char* fgetsRet = nullptr;
        BigInt len = 0;
        bool pushed = false;
        do
        {
            fgetsRet = fgets(string, sizeof(string), pFile);
            if (!fgetsRet)
                break;

            len = strlen(string);
            if ((len > 0) && (string[len - 1] == '\n'))
                string[len - 1] = 0;

            if (!pushed)
            {
                lines.push_back(string);
                pushed = true;
            }
            else
            {
                lines.back() += string;
            }
        } while (len >= (BigInt)(sizeof(string) - 1));

        if (!fgetsRet || feof(pFile))
            break;
    }

    fclose(pFile);
}

void Tokenize(const std::string& st, StringList& tokens, char delim)
{
    std::stringstream stream(st);

    tokens.clear();
    std::string token;
    while (std::getline(stream, token, delim))
    {
        if (!token.empty())
            tokens.push_back(token);
    }
}

void ParseBigIntList(const std::string& st, BigIntList& intList, char delim)
{
    StringList tokens;
    Tokenize(st, tokens, delim);

    intList.clear();
    for (const auto& intString: tokens)
    {
        intList.push_back(StringToBigInt(intString));
    }
}

bool ParseNextBigInt(const char*& st, BigInt& num, bool checkForNegation)
{
    num = 0;
    bool foundNum = false;

    bool isNegated = false;
    while (*st && !isdigit(*st))
    {
        if (checkForNegation)
            isNegated = (*st == '-');
        ++st;
    }

    while (isdigit(*st))
    {
        const BigInt digit = (BigInt)*st - '0';
        num *= 10;
        num += digit;
        foundNum = true; 
        ++st;
    }

    if (foundNum && isNegated)
        num = -num;

    return foundNum;
}

bool StringHasDigits(const std::string& st, BigInt start, BigInt end)
{
    if (end <= 0)
        end = st.length() - 1;

    for (BigInt i = start; i <= end; ++i)
    {
        const char ch = st[i];
        if (ch < '0')
            return false;
        if (ch > '9')
            return false;
    }

    return true;
}

bool StringHasLowerCaseAlphaNumeric(const std::string& st, BigInt start, BigInt end)
{
    if (end <= 0)
        end = st.length() - 1;

    for (BigInt i = start; i <= end; ++i)
    {
        const char ch = st[i];
        if ((ch >= '0') && (ch <= '9'))
            continue;
        if ((ch >= 'a') && (ch <= 'z'))
            continue;
        return false;
    }

    return true;
}

bool StringIsIntWithinRange(const std::string& st, BigInt min, BigInt max, BigInt skipLastChars)
{
    if (st.empty())
        return false;
    if (!StringHasDigits(st, 0, st.length() - skipLastChars - 1))
        return false;

    const BigInt value = atoi(st.c_str());
    return ((value >= min) && (value <= max));
}

bool StringIsIntWithinRangeAndSuffix(const std::string& st, BigInt min, BigInt max, const char* suffix, BigInt suffixLen)
{
    if ((BigInt)st.length() <= suffixLen)
        return false;
    if (st.compare(st.length() - suffixLen, suffixLen, suffix) != 0)
        return false;

    return StringIsIntWithinRange(st, min, max, suffixLen);
}

void MultiplyStringLists(StringList& origList, const StringList& rhsList)
{
    if (origList.empty())
    {
        origList = rhsList;
        return;
    }

    StringList newList;
    for (const auto& lhs: origList)
    {
        for (const auto& rhs: rhsList)
        {
            newList.push_back(lhs + rhs);
        }
    }

    origList.swap(newList);
}


////////////////////////////
// Permutation Iterator

void TestPermutationIterator(BigInt numEntries)
{
    BigIntListPermutationIterator permIter(1, numEntries);
    printf("Testing permutations of %lld entries:\n", numEntries);
    while (permIter.HaveMorePermutations())
    {
        const BigIntList& currPerm = permIter.GetCurrentPermutation();
        printf("  ");
        for (BigInt value: currPerm)
            printf("%lld ", value);
        printf("\n");

        permIter.Step();
    }
    printf("\n");
}



////////////////////////////
////////////////////////////
// ProblemRegistry

std::vector<ProblemBase*> ProblemRegistry::m_problemsToBeRegistered;
std::map<int, ProblemBase*> ProblemRegistry::m_problems;


////////////////////////////
////////////////////////////
// Main

int main(int argc, char** argv)
{
    if (argc <= 1)
    {
        printf(
            "Usages:\n"
            "  AdventOfCode2023 <problem#>\n");
        return 0;
    }

    ProblemRegistry::Init();

    const int problemNum = atoi(argv[1]);
    if (!ProblemRegistry::RunProblem(problemNum))
    {
        printf("Invalid problem # %d!\n\n", problemNum);
        return 0;
    }

    return 0;
}
