#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/comm.h"
#include "../lib/sdbxy.h"


int CheckOptArg(const char *arg, const char opt);

int main(int argc, char *argv[])
{
    FILE *in1, *in2;

    // idx[]: table of indexes {-m N, -o N M, -r N, -h N M, -s N, -g}
    // idx[]: m, o1, o2, r, h1, h2, s, g

    short int idx[8] = {0, 0, 0, 0, 0, 0, 0, 0};    // initial values
    short int state[6] = {0, 0, 0, 0, 0, 0};        // initial states of options. The following sequence is used: {-m, -r, -s, -o, -h, -g}
                                                    // this table allows to check which options are used when the program is called
    // error handling

    if (argc < 3)
    {
        if (argc == 2)
        {
            if (!strcmp(argv[1], (const char *)"--help"))
                Help();     // display the help screen
            else if (!strcmp(argv[1], (const char *)"-v"))
                Version();  // display the version screen
            else
                Hello();    // display the welcome screen
            exit(0);
        }
        Hello();
        exit(0);
    }
    else if (argc > 16)
    {
        ToManyArg();    // display an error: to many arguments
        exit(0);
    }
    else if (argc >= 3)
    {
        in1 = fopen(argv[1], "r");
        in2 = fopen(argv[2], "r");

        if (in1 == NULL || in2 == NULL)
        {
            ErrInFile();
            exit(0);
        }

        if (argc >= 4)
        {
            int i, j = 0, sum = 0;                              // local variables
            char c;

            if (CheckOptArg(argv[argc-1], '2'))                 // if the last argument equals -m, -o, -r, -h or -s stop the program => parameters without arguments
            {
                ErrOption();
                exit(0);
            }

            for (i = 3; i < argc; i++)
            {
                // if the argv[i] argument equals -m, -r or -s and the next argument is different from a parameter value, then go inside if()

                if (CheckOptArg(argv[i], '3') && !CheckOptArg(argv[i+1], '1'))
                {
                    c = *(argv[i]+1);                           // the second sign in a chosen parameter option

                    if (i < argc-1 && !state[0] && c == 'm')
                    {
                        idx[0] = i+1;                           // get an index of argv[] table
                        state[0] = 2;                           // this option was used earlier, switch flag = 2 because two values: -m N
                    }
                    else if (i < argc-1 && !state[1] && c == 'r')
                    {
                        idx[3] = i+1;
                        state[1] = 2;
                    }
                    else if (i < argc-1 && !state[2] && c == 's')
                    {
                        idx[6] = i+1;
                        state[2] = 2;
                    }
                }
                else if (i < argc-2 && !state[3] && CheckOptArg(argv[i], '4') && !CheckOptArg(argv[i+1], '1') && !CheckOptArg(argv[i+2], '1'))  // -o option
                {
                    idx[1] = i+1;
                    idx[2] = i+2;
                    state[3] = 3;                               // this option was used earlier, switch flag = 3 because three values: -o N M
                }
                else if (i < argc-1 && !state[4] && CheckOptArg(argv[i], '5'))  // -h option
                {
                    if (i == argc-2 && !CheckOptArg(argv[i+1], '1'))
                    {
                        idx[4] = i+1;
                        state[4] = 2;
                    }
                    else if (i <= argc-3)
                    {
                        if (!CheckOptArg(argv[i+1], '1') && CheckOptArg(argv[i+2], '1'))
                        {
                            idx[4] = i+1;
                            state[4] = 2;
                        }
                        else if (!CheckOptArg(argv[i+1], '1') && !CheckOptArg(argv[i+2], '1'))
                        {
                            idx[4] = i+1;
                            idx[5] = i+2;
                            state[4] = 3;
                        }
                    }
                }
                else if (!state[5] && CheckOptArg(argv[i], '6'))    // -g option
                {
                    idx[7] = -1;
                    state[5] = 1;
                }
            }

            while (j < 6)
                sum += state[j++];      // sum of flag values

            if (argc != sum + 3)        // compare flag values with an amount of input arguments
            {
                ErrOption();
                exit(0);
            }
        }
    }

    int m, h1, h2, s, n;
    double o1, o2, r;
    int size1, size2, size, line1, line2;
    char c;

    // get values of the mode, sort and header options

    m = atoi(argv[idx[0]]);
    s = atoi(argv[idx[6]]);

        if (Correct_S_M(m, s))
            exit(0);

    h1 = atoi(argv[idx[4]]);
    h2 = atoi(argv[idx[5]]);

        if (h1 < 0 || h2 < 0)
        {
            Err_H();
            exit(0);
        }

    if (state[4] != 3) h2 = h1;   // because options -h N and -h N M are possible, state[4] contains info about it

    // get value of the offset option

    o1 = atof(argv[idx[1]]);
    o2 = atof(argv[idx[2]]);

        // set value of the r option

        if (state[1])
            r = atof(argv[idx[3]]);
        else
            r = 2.0;                // default value of the radius

        if (r <= 0.0)
        {
            Err_R();
            exit(0);
        }

    // reading files
    // get info about a number of lines in file and a size of the longest line

    size1 = NumLines(in1, &line1);
    size2 = NumLines(in2, &line2);

    size1 -= h1;
    size2 -= h2;

        if (size1 <= 0)
        {
            TooBig_H(h1, argv[1]);
            exit(0);
        }
        else if (size2 <= 0)
        {
            TooBig_H(h2, argv[2]);
            exit(0);
        }

        // check if files are empty

        if (!size1)
        {
            EmptyFile(argv[1]);
            exit(0);
        }
        else if (!size2)
        {
            EmptyFile(argv[2]);
            exit(0);
        }

    if (size1 < size2)
        size = size1;
    else
        size = size2;

    struct star *s1, *s2;
    s1 = malloc(size1 * sizeof(struct star));
    s2 = malloc(size2 * sizeof(struct star));

        if (s1 == NULL || s2 == NULL)
        {
            MemoryStar();
            exit(0);
        }

    ReadFile(in1, s1, m, h1, &line1, argv[1]);
    ReadFile(in2, s2, m, h2, &line2, argv[2]);

    fclose(in1);
    fclose(in2);

    // main calculations

    struct data *d;

    d = malloc(size * sizeof(struct data));

        if (d == NULL)
        {
            MemoryData();
            exit(0);
        }

    n = SearchByXY(s1, s2, d, size1, size2, o1, o2, r);

    free(s1);
    free(s2);

        if (!n) exit(0);

    // sorting and writing

    struct data *fin;

    fin = malloc(n * sizeof(struct data));

        if (fin == NULL)
        {
            MemoryData();
            exit(0);
        }

    ShrinkData(fin, d, size);
    free(d);

    if (s)
    {
        c = WhichColumnSort(m ,s);
        SortData(fin, n, c);
    }

    if (state[5])
        MakeReg(fin, n);

    PrintData(fin, n, m);
    free(fin);

    return 0;
}

int CheckOptArg(const char *arg, const char opt)
{
    int state = 0;

    switch (opt)
    {
        case '1': state = (!strcmp(arg, (const char *)"-m") || !strcmp(arg, (const char *)"-o") || !strcmp(arg, (const char *)"-r") || !strcmp(arg, (const char *)"-h") || !strcmp(arg, (const char *)"-s") || !strcmp(arg, (const char *)"-g"));
                break;
        case '2': state = (!strcmp(arg, (const char *)"-m") || !strcmp(arg, (const char *)"-o") || !strcmp(arg, (const char *)"-r") || !strcmp(arg, (const char *)"-h") || !strcmp(arg, (const char *)"-s"));
                break;
        case '3': state = (!strcmp(arg, (const char *)"-m") || !strcmp(arg, (const char *)"-r") || !strcmp(arg, (const char *)"-s"));
                break;
        case '4': state = (!strcmp(arg, (const char *)"-o"));
                break;
        case '5': state = (!strcmp(arg, (const char *)"-h"));
                break;
        case '6': state = (!strcmp(arg, (const char *)"-g"));
                break;
    }
    return state;
}
