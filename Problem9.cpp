///////////////////////////
// Problem 9 - Disk Fragmenter

#include "AdventOfCode2024.h"

class Problem9 : public ProblemBase
{
public:
    virtual int GetProblemNum() const override { return 9; }

    virtual void Run() override
    {
        RunOnData("Day9Example.txt", true);
        RunOnData("Day9Input.txt", false);
    }

private:
    static const wchar_t sm_empty = L'.';

    void RunOnData(const char* filename, bool verbose)
    {
        printf("Running on data in file '%s':\n", filename);

        FILE* pFile = OpenInputFile(filename);

        std::wstring hdd;

        // build the hdd

        bool freeSpace = false;
        BigUInt fileID = 0;

        for (;;)
        {
            const int chInt = fgetc(pFile);
            if (chInt == EOF)
                break;

            const char ch = (char)chInt;
            const BigInt num = (BigInt)(BigUInt)(unsigned char)ch - (BigInt)(BigUInt)(unsigned char)'0';

            if (!freeSpace)
            {
                const wchar_t fileIDChar = (wchar_t)(uint16_t)(fileID + L'0');
                hdd.append(num, fileIDChar);
                ++fileID;

                assert(fileID < (1ULL << 16));
            }
            else
            {
                hdd.append(num, sm_empty);
            }

            freeSpace = !freeSpace;
        }

        if (verbose)
            wprintf(L"  %s\n", hdd.c_str());

        std::wstring hdd2 = hdd;

        // defragment block by block

        BigInt readIndex = hdd.length() - 1;
        BigInt writeIndex = 0;

        for (;;)
        {
            if (writeIndex >= readIndex)
                break;

            if (hdd[readIndex] == sm_empty)
            {
                --readIndex;
            }
            else
            {
                if (hdd[writeIndex] == sm_empty)
                {
                    hdd[writeIndex] = hdd[readIndex];
                    hdd[readIndex] = sm_empty;
                    --readIndex;

                    if (verbose)
                        wprintf(L"  %s\n", hdd.c_str());
                }
                ++writeIndex;
            }
        }

        // calc the checksum

        BigUInt checksum = 0;

        for (BigUInt i = 0; i < (BigUInt)hdd.size(); ++i)
        {
            if (hdd[i] == sm_empty)
                break;

            const BigUInt fileID = (BigUInt)(uint16_t)hdd[i] - (BigUInt)(uint16_t)L'0';
            const BigUInt addend = fileID * i;
            checksum += addend;

            if (verbose)
                wprintf(L"  Checksum adding %llu * %llu = %llu\n", i, fileID, checksum);
        }

        wprintf(L"  Checksum (defragmenting block by block) = %llu\n\n", checksum);

        // defragment file by file

        readIndex = hdd2.length() - 1;
        writeIndex = 0;

        wchar_t readingFileIDChar = L'\0';
        BigInt numFileBlocks = 0;

        for (;;)
        {
            if (readIndex < 0)
                break;

            bool foundReadable = false;
            ScanForReadableFile(hdd2, readIndex, foundReadable, readingFileIDChar, numFileBlocks);

            if (!foundReadable)
                break;

            bool foundWritableSpace = false;
            ScanForWritableSpace(hdd2, readIndex, numFileBlocks, foundWritableSpace, writeIndex);

            if (foundWritableSpace)
            {
                for (BigInt i = 0; i < numFileBlocks; ++i)
                {
                    hdd2[writeIndex + i] = readingFileIDChar;
                    hdd2[readIndex + i] = sm_empty;
                }

                if (verbose)
                    wprintf(L"  %s\n", hdd2.c_str());
            }
            else if (verbose)
            {
                wprintf(L"  cannot move fileID %llu\n", (BigUInt)(uint16_t)readingFileIDChar - (BigUInt)(uint16_t)L'0');
            }

            --readIndex;
        }

        // calc the checksum2

        BigUInt checksum2 = 0;

        for (BigUInt i = 0; i < (BigUInt)hdd2.size(); ++i)
        {
            if (hdd2[i] == sm_empty)
                continue;

            const BigUInt fileID = (BigUInt)(uint16_t)hdd2[i] - (BigUInt)(uint16_t)L'0';
            const BigUInt addend = fileID * i;
            checksum2 += addend;

            if (verbose)
                wprintf(L"  Checksum2 adding %llu * %llu = %llu\n", i, fileID, checksum2);
        }

        wprintf(L"  Checksum2 (defragmenting file by file) = %llu\n\n", checksum2);
    }

    void ScanForReadableFile(const std::wstring& hdd, BigInt& readIndex, bool& found, wchar_t& fileIDChar, BigInt& num)
    {
        while ((readIndex >= 0) && (hdd[readIndex] == sm_empty))
            --readIndex;

        found = false;
        if (hdd[readIndex] != sm_empty)
        {
            found = true;
            fileIDChar = hdd[readIndex];
            num = 0;
            while ((readIndex >= 0) && (hdd[readIndex] == fileIDChar))
            {
                ++num;
                --readIndex;
            }
        }
        ++readIndex;
    }

    void ScanForWritableSpace(const std::wstring& hdd, BigInt readIndex, BigInt num, bool& found, BigInt& writeIndex)
    {
        const BigInt maxIndex = readIndex - 1;

        BigInt index = 0;

        for (;;)
        {
            if (index >= maxIndex)
            {
                found = false;
                return;
            }

            while ((index <= maxIndex) && (hdd[index] != sm_empty))
                ++index;

            BigInt numEmpty = 0;
            writeIndex = index;
            while ((index <= maxIndex) && (numEmpty < num) && (hdd[index] == sm_empty))
            {
                ++numEmpty;
                ++index;
            }

            if (numEmpty >= num)
            {
                found = true;
                return;
            }
        }
    }
};

Problem9 problem9;
