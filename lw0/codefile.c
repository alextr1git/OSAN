#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint GetBirthInDays(char *str);

ulong SumSymblsCodes(char *str);

int main(int args, char *argv[])
{
    if (args != 6)
    {
        printf("Недостаточно аргументов\n");
        return -1;
    }

    int K = atoi(argv[5]);
    int len = 0;

    ulong TotalSum = 0;

    TotalSum += SumSymblsCodes(argv[1]);
    TotalSum += SumSymblsCodes(argv[2]);
    TotalSum += SumSymblsCodes(argv[3]);

    printf("\nS1: %lu", TotalSum);

    TotalSum *= GetBirthInDays(argv[4]);


    ulong cpS = TotalSum;
    do
    {
        len++;
        cpS = cpS / 10;
    } while (cpS > 0);


    int ArrayOfDigits[len];
    int len2 = len;
    len2--;

    printf("Sum: %lu\n", TotalSum);

    do
    {
        ArrayOfDigits[len2--] = TotalSum % 10;
        TotalSum /= 10;
    } while (TotalSum > 0);

    int S1 = ArrayOfDigits[len / 2 -1] * 100 + ArrayOfDigits[len / 2] * 10 + ArrayOfDigits[len / 2+1];
    printf("Цифры: %d\n", S1);
    int N = S1 % K + 1;
    printf("Вариант: %d\n", N);

    return 0;
}

uint GetBirthInDays(char *str)
{
char* tokens = strtok(str, ".");
    int flag = 0, days, months, years;

    while (tokens != NULL)
    {
        switch (flag)
        {
        case 0:
            days = atoi(tokens);
            flag = 1;
            break;
        case 1:
            months = atoi(tokens);
            flag = 2;
            break;
        case 2:
            years = atoi(tokens);
            flag = 3;
            break;
        }
        tokens = strtok(NULL, ".");
    }
    printf("\nДата: %d\n", (days+months*30+years*365));
    return days + months * 30 + years * 365;
}

ulong SumSymblsCodes(char *str)
{
    int len = strlen(str);
    ulong sum = 0;

    for (int i = 0; i < len; i+=2)
    {
        u_char symb = str[i];
        u_char symb2 = str[i+1];
        uint tmp = (symb << 8) +symb2;
        sum += tmp*tmp;
        printf("%x\n", tmp); 
    }

    return sum;
}
