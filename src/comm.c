#include <stdio.h>

#ifndef PROG
#define PROG "sdb_xy"
#endif

void Hello()
{
    printf("\n %s => (s)earch two (d)ata(b)ases using (x)(y) coordinates\n\n", PROG);
    printf(" Default call:\n  %s <star-list-1> <star-list-2>\n", PROG);
    printf(" For more info please type:\n  %s --help\n", PROG);
    printf(" Display version information and exit:\n  %s -v\n\n", PROG);
    printf(" Copyright (c) Przemysław Bruś\n\n");
}

void Version()
{
    printf("%s\n", PROG);
    printf(" * Version: 2017-02-24\n");
    printf(" * Licensed under the MIT license:\n   http://opensource.org/licenses/MIT\n");
    printf(" * Copyright (c) 2017 Przemysław Bruś\n");
}

void Help()
{
    printf("This program searches two databases with stars and makes a cross-corelation between them based on the XY coordinates. ");
    printf("Input lists of stars should have the following structure:\n\n");
    printf(" # ID_star Xpx Ypx Mag ...\n\n");
    printf("ID_star is converted into integer, the rest of columns is treated as float numbers. ");
    printf("Minimum three first columns are required.\n");
    printf("Usage:\n\n %s <list-1> <list-2> [-Mode] [-Offset] [-Radius] [-Head] [-Sort] [-reGion]\n\nOptions:\n\n", PROG);

    // options

    printf(" -m N   \tFive output formats are available (default: -m 0):\n");
    printf("        \t 0 - id1 x1 y1 id2 x2 y2 r \t// r = distance between stars (px)\n");
    printf("        \t 1 - id1 x1 y1 id2 x2 y2 r mag1-mag2\n");
    printf("        \t 2 - id1 x1 y1 mag1\n");
    printf("        \t 3 - id2 x2 y2 mag2\n");
    printf("        \t 4 - id1 id2\n");
    printf(" -o N M \tAdd an offset to the (x1,y1) coordinates before searching starts: (x1+N,y1+M).\n");
    printf("        \tN and M can be float number (default: -o 0.0 0.0).\n");
    printf(" -r N   \tSet the radius which defines a search area (default: -r 2.0).\n");
    printf(" -h N M \tIgnore the first N and M lines in list-1 and list-2, respectively.\n");
    printf("        \tIf both headers are the same size, only one value is necessary (default: -h 0).\n");
    printf(" -s N   \tSort the output data by 1-8 column (default: none = -s 0).\n");
    printf("        \tAmount of output columns depends on the -m option.\n");
    printf(" -g     \tGenerate an additional output %s.reg file to the ds9 program in the working directory.\n", PROG);
    printf("        \tThis file is generated based on the id1, x1, y1 values.\n");
    printf("        \tRegions are set as yellow circles with 10 px radius and are signed as id1 numbers.\n");
    printf("        \tIf the -s option is used, %s.reg inherits this feature (default: none).\n\n", PROG);

    printf("The sequence of the optional [] parameters can be random. On the contrary, names of the input lists must be located just behind the name of the program.\n");
}

void ToManyArg()
{
    printf("Error from %s: Too many arguments. Check '%s --help'.\n", PROG, PROG);
}

void ErrInFile()
{
    printf("Error from %s: Problem with the input lists. Check names or privileges of the input lists.\n", PROG);
}

void ErrOption()
{
    printf("Error from %s: Incorrect usage. Please, use '%s --help'.\n", PROG, PROG);
}

void EmptyFile(const char *arg)
{
    printf("Error from %s: File '%s' is empty.\n", PROG, arg);
}

void MemoryStar()
{
    printf("Error from %s: Cannot allocate memory for star's structures.\n", PROG);
}

void Err_H()
{
    printf("Error from %s: An argument of the -h option must be >= 0\n", PROG);
}

void TooBig_H(int h, const char *file)
{
    printf("Error from %s: -h = %d is equal or greater than the number of lines in '%s' file.\n", PROG, h, file);
}

void NotMemoBuff()
{
    printf("Error from %s: The malloc() function hasn't allocated memory in the ReadFile() function.\n", PROG);
}

void ErrReadHeader(const char *arg)
{
    printf("Error from %s: A problem has occurred while reading the header in '%s' file.\n", PROG, arg);
}

void ErrReadLine(const char *arg, int line)
{
    printf("Error from %s: A problem has occurred while reading the line %d in '%s' file.\n", PROG, line, arg);
}

void Err_R()
{
    printf("Error from %s: An argument of the -r option must be > 0\n", PROG);
}

void MemoryData()
{
    printf("Error from %s: Cannot allocate memory for data structure.\n", PROG);
}

short Correct_S_M(int m, int s)
{
    short int state = 0;

    switch (m)
    {
        case 0: break;
        case 1: break;
        case 2: break;
        case 3: break;
        case 4: break;
        default: printf("Error from %s: -m = 0,1,2,3,4 not %d\n", PROG, m);
                 state = 1;
    }

    switch (s)
    {
        case 0: break;
        case 1: break;
        case 2: break;
        case 3: break;
        case 4: break;
        case 5: break;
        case 6: break;
        case 7: break;
        case 8: break;
        default: printf("Error from %s: -s = 1,2,3,4,5,6,7,8 not %d\n", PROG, s);
                 state = 1;
    }

    if (!state)
    {
        if (!m && s < 8);
        else if (m == 1 && s < 9);
        else if ((m == 2 || m == 3) && s < 5);
        else if (m == 4 && s < 3);
        else state = 2;
    }

    if (state == 2)
        printf("Error from %s: An output format doesn't contain %d. column. Check the -m and -s options.\n", PROG, s);

    return state;
}
