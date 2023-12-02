//==================================================================================================
//
// File: main.c
//
//==================================================================================================

//==================================================================================================
// Include directives
//==================================================================================================
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>


//==================================================================================================
// Preprocessor definitions
//==================================================================================================
#define BUFFER_MAX 128
#define ASCII_OFFSET 0x30
#define ASCII_ONE 0x31
#define ASCII_NINE 0x39
#define RETURN_FAILIURE -1


//==================================================================================================
// Structures & enumerations
//==================================================================================================
typedef struct
{
    uint8_t Num;
    uint8_t Index;
    bool Found;
    int8_t RetVal;
} FoundNum;

typedef struct
{
    char Str[10];
    uint8_t Len;
    uint8_t Num;
} StrNum;

//==================================================================================================
// Function prototypes
//==================================================================================================
int check_args(int argc, char** argv);
void FindFirstNum(FoundNum* Result, const char* Buffer, const uint8_t Length);
void FindLastNum(FoundNum* Result, const char* Buffer, const uint8_t Length);
uint8_t Len(const char* Buffer);
uint8_t WordInStr(char* Word, uint8_t WordLen, char* Str, uint8_t StrLen, uint8_t Offset, bool LastNum);
void ExtractSubstring(const char* String, char* Substring, uint8_t StartIndex, uint8_t StopIndex);
void ResetNum(FoundNum* Number);


//==================================================================================================
// Variables, file-wide scope.
//==================================================================================================
char LineBuffer[BUFFER_MAX] = { 0 };
StrNum CompareVals[9] =
{
    {.Str = "one",   .Len = 3, .Num = 1},
    {.Str = "two",   .Len = 3, .Num = 2},
    {.Str = "three", .Len = 5, .Num = 3},
    {.Str = "four",  .Len = 4, .Num = 4},
    {.Str = "five",  .Len = 4, .Num = 5},
    {.Str = "six",   .Len = 3, .Num = 6},
    {.Str = "seven", .Len = 5, .Num = 7},
    {.Str = "eight", .Len = 5, .Num = 8},
    {.Str = "nine",  .Len = 4, .Num = 9}
};
unsigned int Sum = 0;
unsigned int FirstIndex = 0;
unsigned int LastIndex = 0;
uint8_t FirstMallocCoeff = 0;
uint8_t FirstExtractIndex = 0;
uint8_t LastMallocCoeff = 0;
uint8_t LastExtractIndex = 0;
FoundNum FirstNum = {0, 0, false , -1};
FoundNum LastNum = {0, 0, false , -1};


//==================================================================================================
// Program entry point.
//==================================================================================================
int main(int argc, char* argv[])
{
    if (check_args(argc, argv) != EXIT_SUCCESS) exit(EXIT_FAILURE);

    FILE* File = fopen(argv[1], "r");
    if (File == NULL)
    {
        printf("Error opening file...\n");
        exit(EXIT_FAILURE);
    }

    while (fgets(LineBuffer, BUFFER_MAX, File) != NULL)
    {
        FindFirstNum(&FirstNum, LineBuffer, Len(LineBuffer));
        FindLastNum(&LastNum, LineBuffer, Len(LineBuffer));

        // printf("%s", LineBuffer);

        if (FirstNum.Index >= 3 || !FirstNum.Found)
        {
            if (!FirstNum.Found)
            {
                FirstMallocCoeff = (Len(LineBuffer) + 1);
                FirstExtractIndex = (Len(LineBuffer));
            }
            else
            {
                FirstMallocCoeff = (FirstNum.Index + 1);
                FirstExtractIndex = FirstNum.Index;
            }
            char* FirstSubstring = (char*)malloc(sizeof(char) * FirstMallocCoeff);
            ExtractSubstring(LineBuffer, FirstSubstring, 0, FirstExtractIndex);
            // printf("First substring: %s\nLength: %u\n", FirstSubstring, Len(FirstSubstring));

            for (unsigned int i = 0; i < 9; i++)
            {
                if (Len(FirstSubstring) < CompareVals[i].Len) continue;

                FirstNum.RetVal = WordInStr(CompareVals[i].Str, CompareVals[i].Len, FirstSubstring, Len(FirstSubstring), 0, false);
                if (FirstNum.RetVal != RETURN_FAILIURE)
                {
                    // printf("<%s> found in <%s> at index <%u>\n",CompareVals[i].Str, FirstSubstring, FirstNum.RetVal);
                    if (FirstNum.RetVal <= FirstNum.Index)
                    {
                        FirstNum.Index = FirstNum.RetVal;
                        // printf("Updating first num: %u\n", CompareVals[i].Num);
                        FirstNum.Num = CompareVals[i].Num;
                    }
                }
                else
                {
                    // printf("<%s> not found in <%s>\n",CompareVals[i].Str, FirstSubstring);
                }
            }
            free(FirstSubstring);
        }

        if (Len(LineBuffer) - (LastNum.Index + 1) >= 3 || !LastNum.Found)
        {
            if (!LastNum.Found)
            {
                LastMallocCoeff = (Len(LineBuffer) + 1);
                LastExtractIndex = 0;
            }
            else
            {
                LastMallocCoeff = (Len(LineBuffer) - LastNum.Index);
                LastExtractIndex = (LastNum.Index + 1);
            }
            char* LastSubstring = (char*)malloc(sizeof(char) * LastMallocCoeff);
            ExtractSubstring(LineBuffer, LastSubstring, LastExtractIndex, Len(LineBuffer));
            // printf("Last substring: %s\nLength: %u\n", LastSubstring, Len(LastSubstring));

            for (unsigned int i = 0; i < 9; i++)
            {
                if (Len(LastSubstring) < CompareVals[i].Len) continue;
                uint8_t Offset = Len(LineBuffer) - Len(LastSubstring);

                LastNum.RetVal = WordInStr(CompareVals[i].Str, CompareVals[i].Len, LastSubstring, Len(LastSubstring), Offset, true);
                if (LastNum.RetVal != RETURN_FAILIURE)
                {
                    // printf("<%s> found in <%s> at index <%u>\n",CompareVals[i].Str, LastSubstring, LastNum.RetVal);
                    if (LastNum.RetVal >= LastNum.Index)
                    {
                        LastNum.Index = LastNum.RetVal;
                        // printf("Updating last num: %u\n", CompareVals[i].Num);
                        LastNum.Num = CompareVals[i].Num;
                    }
                }
                else
                {
                    // printf("<%s> not found in <%s>\n",CompareVals[i].Str, LastSubstring);
                }
            }
            free(LastSubstring);
        }

        // printf("First: %u\tLast: %u\n", FirstNum.Num, LastNum.Num);
        // printf("------------------------------\n");

        Sum += (10 * (FirstNum.Num)) + (LastNum.Num);
        ResetNum(&FirstNum);
        ResetNum(&LastNum);
    }
    fclose(File);

    printf("Sum: %u\n", Sum);

    return EXIT_SUCCESS;
}


//==================================================================================================
// Function definitions
//==================================================================================================
int check_args(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("Incorrect number of command line arguments!\n");
        return EXIT_FAILURE;
    }
    else
    {
        printf("Input file: %s\n", argv[1]);
        return EXIT_SUCCESS;
    }
}

void FindFirstNum(FoundNum* Result, const char* Buffer, const uint8_t Length)
{
    for (uint8_t i = 0; i < Length; i++)
    {
        if (Buffer[i] >= ASCII_ONE && Buffer[i] <= ASCII_NINE)
        {
            Result->Found = true;
            Result->Index = i;
            Result->Num = (Buffer[i] - ASCII_OFFSET);
            return;
        }
    }
}

void FindLastNum(FoundNum* Result, const char* Buffer, const uint8_t Length)
{
    for (int8_t i = (Length - 1); i >= 0; i--)
    {
        if (Buffer[i] >= ASCII_ONE && Buffer[i] <= ASCII_NINE)
        {
            Result->Found = true;
            Result->Index = i;
            Result->Num = (Buffer[i] - ASCII_OFFSET);
            return;
        }
    }
}

void ResetNum(FoundNum* Number)
{
    Number->Found = false;
    Number->Index = 0;
    Number->Num = 0;
    Number->RetVal = -1;
}

uint8_t Len(const char* Buffer)
{
    uint8_t Length = 0;
    for (char* i = (char*)&Buffer[0]; *i != '\0'; i++)
    {
        Length++;
        if (*i == '\n') Length--;
    }
    return Length;
}

uint8_t WordInStr(char* Word, uint8_t WordLen, char* Str, uint8_t StrLen, uint8_t Offset, bool LastNum)
{
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t k = 0;
    uint8_t MatchCnt = 0;
    int8_t MatchIndex = RETURN_FAILIURE;
    
    for (i = 0; i <= (StrLen - WordLen); i++)
    {
        for (j = i; k < WordLen; j++)
        {
            if (Word[k] != Str[j]) break;
            k++;
        }
        if (k == WordLen)
        {
            if (!LastNum)
            {
                return (j - WordLen + Offset);
            }
            else
            {
                MatchCnt++;
                if ((j - WordLen + Offset) > MatchIndex)
                {
                    MatchIndex = (j - WordLen + Offset);
                }
            }
        }
        k = 0;
    }
    return MatchIndex;
}

void ExtractSubstring(const char* String, char* Substring, uint8_t StartIndex, uint8_t StopIndex)
{
    uint8_t i = 0;
    uint8_t j = 0;

    for (i = StartIndex; i < StopIndex; i++)
    {
        Substring[j] = String[i];
        j++;
    }
    Substring[j] = '\0';
}
