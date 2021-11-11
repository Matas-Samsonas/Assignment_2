// Matas Samsonas 2110629
// PS course 1, group 2
// matas.samsonas@mif.vu.lt
// *1.Ivesti sveika skaièiu dienu, kiek zmogus mokysis per metus.
// Paskirstyti jo mokymosi dienas metu eigoje, kiek imanoma, tolygiau (sveikais skaiciais kiekvienam menesiui),
// t.y.  kad  skirstant  metus  bet  kokio  ilgio  laikotarpiais  (po  1,  2,  3,  4,  ar  6  menesius)
// mokymosi dienu skaièiai kiekviename periode butu, kiek imanoma, vienodi.
// Isvesti, kiek zmogus mokysis kiekvienà menesi, ketvirti ir pusmeti. Papildymas: skirstant atsizvelgti á kiekvieno menesio darbo dienu skaiciu.
#include <stdio.h>
#include <math.h>
#define MAXSIZE 12
#define HOLIDAY_AMOUNT 12
struct PeriodOfTime // A struct which defines a period of time (either months, quarters or halves)
{
    int allDays;
    int workDays;
    int learningDays;
    char *name;
    double ratioLearningToAll;
};
int findPositionOfSmallestValue(struct PeriodOfTime allValues[], int startLoop, int endLoop);
int findPositionOfLargestValue(struct PeriodOfTime allValues[], int startLoop, int endLoop);
int checkWeekDay(int year, int month, int day, struct PeriodOfTime monthAllDays[]);
void evenlyAssignValues(struct PeriodOfTime period[], int startLoop, int endLoop, int comparisonValue);
void printLabelledList(struct PeriodOfTime listAndLabels[], int sizeOfList);
void printSeperator();
int main()
{
    //INITIALIZATION OF VARIABLES
    //{
        int holidayMonth[HOLIDAY_AMOUNT] = {1, 2, 3, 5, 6, 7, 8, 11, 11, 12, 12, 12};
        int holidayDay[HOLIDAY_AMOUNT] = {1, 16, 11, 1, 24, 6, 15, 1, 2, 24, 25, 26};
        int monthAllDays[MAXSIZE] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        char *monthsNames[MAXSIZE] = {"January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December",};
        char *quarterNames[4] = {"First Quarter", "Second Quarter", "Third Quarter", "Forth Quarter"};
        char *halfNames[2] = {"First Half", "Second Half"};
        struct PeriodOfTime month[MAXSIZE] = { 0 }, half[2] = { 0 }, quarter[4] = { 0 };
        for(int i = 0; i < MAXSIZE; ++i)
        {
            month[i].name = monthsNames[i];
            month[i].allDays = monthAllDays[i];
            half[2 * i / MAXSIZE].name = halfNames[2 * i / MAXSIZE];
            half[2 * i / MAXSIZE].allDays += monthAllDays[i];
            quarter[4 * i / MAXSIZE].name = quarterNames[4 * i / MAXSIZE];
            quarter[4 * i / MAXSIZE].allDays += monthAllDays[i];
        }
        int weekDay;
        int year;
        int amountOfLearningDays;
        int sumWorkDays = 0;
        double ratio;
    //}
    //USER INTERFACE + INPUT
    //{
        printf("This program creates a list which consists of the amount of studying days in every month / quarter / half.\n");
        printf("The program receives two natural number inputs.\n");
        printf("The first one is the year which the list will be based on. Only years 2020 and above are supported.\n");
        printf("The second input is the amount of planned days of study.\n");
        printSeperator();
        printf("Note:\n");
        printf("The program accounts for days off.\n");
        printf("Days off are considered to be weekends and national holidays.\n");
        printf("However, due to the erratic dates that Easter falls on, it is not taken into consideration.\n");
        printf("To compensate, one work day is removed from both March and April.\n");
        printSeperator();
        printf("Input the year: ");
        while(1)
        {
            if(!scanf("%4u", &year) || (getchar() != '\n')) // Input validation
            {
                while(getchar() != '\n')
                    ;
                printf("Invalid input. Try again.\n");
                continue;

            }
                if(year < 2020)
                {
                    printf("The program only supports years from 2020 and above.\n");
                }
                else
                {
                    printf("Input accepted.\n");
                    printSeperator();
                    break;
                }

        }
        //WORK DAY / LEAP YEAR CALCULATION
        //{
            if ((!(year % 4) && (year % 100) ) || (!(year % 400)))
            // Leap years are multiples of 4 which are not divisible by 100 except those which are divisible by 400
            {
                month[1].allDays++; // Adding February 29th
            }
            for(int i = 0; i < MAXSIZE; ++i) // Excluding weekends and holidays from working days
            {
                month[i].workDays = month[i].allDays; // Adding the amount of all days to working days
                for(int j = 1; j <= month[i].allDays; ++j)
                {

                    weekDay = checkWeekDay(year, i + 1, j, month);
                    // Calling the function which calculates the day of the week.
                    // The function receives the year, month, day and the array where the month information is.
                    if((weekDay == 6) || (weekDay == 0)) // Checking if the week day is a Sunday (0) or Saturday (6).
                    {
                        --month[i].workDays;
                    }
                    for(int k = 0; k < HOLIDAY_AMOUNT; ++k) // Checking if the day in question is a holiday
                    {
                        if((holidayMonth[k] == i + 1) && (holidayDay[k] == j) && (weekDay != 0) && (weekDay != 6))
                        // If the day is a holiday and it does not fall on a weekend one day is removed from work days.
                        {
                            --month[i].workDays;
                        }
                    }
                }
                if((i == 2)||(i == 3))
                {
                    month[i].workDays--;
                }
                sumWorkDays += month[i].workDays;
                quarter[4 * i / MAXSIZE].workDays += month[i].workDays;
                half[2 * i / MAXSIZE].workDays += month[i].workDays;
            }
        //}

        printf("Please input a natural number less than or equal to %d: ", sumWorkDays);
        while(1)
        {
            if(!scanf("%3u", &amountOfLearningDays) || (getchar() != '\n'))
            {
                while(getchar() != '\n')
                    ;
                printf("Invalid input. Try again.\n");
                continue;

            }
            if(amountOfLearningDays > sumWorkDays)
            {
                printf("The provided number of days is bigger than the amount of working days in the year. Try again.\n");
            }
            else
            {
                printf("Input accepted.\n");
                printSeperator();
                break;
            }
        }

    //}
    //ASSIGNEMENT OF VALUES
    //{
        ratio = (double)amountOfLearningDays / sumWorkDays;
        for(int i = 0; i < MAXSIZE; i++)
        {
            month[i].learningDays = ratio * month[i].workDays;
            quarter[(4 * i) / MAXSIZE].learningDays += ratio * month[i].workDays;
            half[(2 * i) / MAXSIZE].learningDays += ratio * month[i].workDays;
        }
        evenlyAssignValues(month, 0, MAXSIZE, amountOfLearningDays);
        evenlyAssignValues(quarter, 0, 4, amountOfLearningDays);
        evenlyAssignValues(half, 0, 2, amountOfLearningDays);
        for(int i = 0; i < 2; ++i)
        {
            evenlyAssignValues(quarter, 2 * i, 2 * (i + 1), half[i].learningDays);
        }
        for(int i = 0; i < 4; ++i)
        {
            evenlyAssignValues(month, i * MAXSIZE / 4, (1 + i) * MAXSIZE / 4, quarter[i].learningDays);
        }
    //}
    //OUTPUT
    //{
        printf("Months:\n");
        printLabelledList(month, MAXSIZE);
        printSeperator();
        printf("Quarters:\n");
        printLabelledList(quarter, 4);
        printSeperator();
        printf("Halves:\n");
        printLabelledList(half, 2);
    //}
    return 0;
}
int findPositionOfLargestValue(struct PeriodOfTime allValues[], int startLoop, int endLoop)
// Finds the position of the largest number in an array
{
    double largestValue = -1;
    int position;
    for(int i = startLoop; i < endLoop; ++i)
    {
        if(allValues[i].learningDays > largestValue)
        {
            position = i;
            largestValue = allValues[i].learningDays;
        }
        if(allValues[i].learningDays == largestValue)
        {
            if(allValues[i].ratioLearningToAll > allValues[position].ratioLearningToAll)
            {
                position = i;
            }
        }
    }
    return position;
}

int findPositionOfSmallestValue(struct PeriodOfTime allValues[], int startLoop, int endLoop)
// Finds the position of the smallest number in an array
{
    double smallestValue = 366;
    int position;
    for(int i = startLoop; i < endLoop; ++i)
    {
        if(allValues[i].learningDays < smallestValue)
        {
            position = i;
            smallestValue = allValues[i].learningDays;
        }
        if(allValues[i].learningDays == smallestValue)
        {
            if(allValues[i].ratioLearningToAll < allValues[position].ratioLearningToAll)
            {
                position = i;
            }
        }
    }
    return position;
}

int checkWeekDay(int year, int month, int day, struct PeriodOfTime monthAllDays[])
// Finds on which weekday does a specific day of the year fall on
{
    int dayOfWeek; // The day of week (0 - Sunday, 1 - Monday, ... , 6 - Saturday).
    dayOfWeek = 3;
    //The base day for this program is January 1st 2020 which falls on a Wednesday.
    for(int i = 2021; i <= year; ++i)
    // If the inputted year is 2021 or later this loop finds the weekday of January 1st of the inputted year.
    {
        if (!((i - 1) % 4) && (((i - 1) % 100)) || (!((i - 1) % 400)))
        // If the previous year was a leap year the weekday of the current year's January 1st will be offset by 2 compared to the previous year's January 1st
        {
            dayOfWeek += 2;
        }
        else
        // The offset between the weekday of a non-leap year's January 1st and the weekday of the following year's January 1st is 1.
        {
            ++dayOfWeek;
        }
    }
    for(int i = 1; i < month; ++i)
    {
        dayOfWeek += monthAllDays[i - 1].allDays % 7;
    }
    dayOfWeek += day - 1;
    return dayOfWeek % 7;
}

void printLabelledList(struct PeriodOfTime listAndLabels[], int sizeOfList)
// This function prints a list which consists of values which have unique labels.
{
    for(int i = 0; i < sizeOfList; ++i)
    {
        printf("%s: %d\n",listAndLabels[i].name, listAndLabels[i].learningDays);
    }
}

void printSeperator() // Prints a seperator in order to differentiate between parts of the program.
{
    printf("\n------------------------------------ \n\n");
}

void evenlyAssignValues(struct PeriodOfTime period[], int startLoop, int endLoop, int comparisonValue)
// This function evenly assigns learning days to every element of the array.
{
    int sumLearningDays = 0;
    for(int i = startLoop; i < endLoop; ++i)
    // This loop calculates the sum of the approximations.
    {
        sumLearningDays += period[i].learningDays;
    }
    while(sumLearningDays != comparisonValue)
    // The sum of the approximations can be slightly different from the comparisonValue due to rounding issues.
    {
        for(int i = startLoop; i < endLoop; ++i)
        // This loop calculates the ratio between learning and all days of the period. This can be seen as the period's busyness.
        {
            period[i].ratioLearningToAll = (double) period[i].learningDays / period[i].allDays;
            //printf("%f ", period[i].ratioLearningToAll);
        }
        if(sumLearningDays > comparisonValue)
        // If the sum is larger than the input the function finds the most busy period,
        // and then one learning day is subtracted from it.
        {
            int position = findPositionOfLargestValue(period, startLoop, endLoop);
            period[position].learningDays--;
            sumLearningDays--;
        }
        else // The opposite of the previous if statement.
        {
            int position = findPositionOfSmallestValue(period, startLoop, endLoop);
            period[position].learningDays++;
            sumLearningDays++;
        }
    }
}
