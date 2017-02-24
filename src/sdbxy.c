#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../lib/comm.h"
#include "../lib/sdbxy.h"

int NumLines(FILE *file, int *longline)
{
    int i = 0, j = 0;
    char c;
    *longline = 0;

        do
        {
            c = fgetc(file);
            j++;

            if (c == '\n')
            {
                i++;
                if (j > *longline)
                    *longline = j;
                j = 0;
            }

        }
        while (c != EOF);

    *longline += 1;     // for new line '\n' sign
    rewind(file);

    return i;
}

void ReadFile(FILE *file, struct star *s, int m, int h, int *longline, const char *arg)
{
    int i = 0, j = 0, n;
    char *buff = malloc(*longline * sizeof(char));

        if (buff == NULL)
        {
            NotMemoBuff();
            exit(0);
        }

    while (j < h)
        if (fgets(buff, *longline, file) != NULL) j++;
        else ErrReadHeader(arg);

    if (m == 0 || m == 4) n = 3; else n = 4;

    while (fgets(buff, *longline, file) != NULL)
    {
        if (n == 3 && sscanf(buff, "%d %lf %lf", &((s+i)->id), &((s+i)->x), &((s+i)->y)) != n)
        {
            ErrReadLine(arg, i+1+h);
            exit(0);
        }
        else if (n == 4 && sscanf(buff, "%d %lf %lf %lf", &((s+i)->id), &((s+i)->x), &((s+i)->y), &((s+i)->mag)) != n)
        {
            ErrReadLine(arg, i+1+h);
            exit(0);
        }
        i++;
    }

    free(buff);
}

int SearchByXY(const struct star *s1, const struct star *s2, struct data *d, int size1, int size2, double off1, double off2, double r_in)
{
    int i, j, n = 0, size;
    double idx1, idx2, x1, y1, x2, y2, x_off, y_off, r, r_tmp;

    if (size1 < size2)
        size = size1;
    else
        size = size2;

    for (i = 0; i < size; i++)
        (d+i)->r = -1.0;

    for (i = 0; i < size1; i++)
    {
        x1 = (s1+i)->x;
        y1 = (s1+i)->y;
        x_off = x1 + off1;
        y_off = y1 + off2;
        r = r_in;

            for (j = 0; j < size2; j++)
            {
                x2 = (s2+j)->x;
                y2 = (s2+j)->y;
                r_tmp = sqrt(pow((x2 - x_off), 2) + pow((y2 - y_off), 2));

                if (r_tmp < r)
                {
                    idx1 = (s1+i)->id;
                    idx2 = (s2+j)->id;
                    r = r_tmp;

                        if (size == size1)
                        {
                            (d+i)->id1 = idx1;
                            (d+i)->id2 = idx2;
                            (d+i)->x1 = x1;
                            (d+i)->x2 = x2;
                            (d+i)->y1 = y1;
                            (d+i)->y2 = y2;
                            (d+i)->mag1 = (s1+i)->mag;
                            (d+i)->mag2 = (s2+j)->mag;
                            (d+i)->dmag = (d+i)->mag1 - (d+i)->mag2;
                            (d+i)->r = r_tmp;
                        }
                        else
                        {
                            (d+j)->id1 = idx1;
                            (d+j)->id2 = idx2;
                            (d+j)->x1 = x1;
                            (d+j)->x2 = x2;
                            (d+j)->y1 = y1;
                            (d+j)->y2 = y2;
                            (d+j)->mag1 = (s1+i)->mag;
                            (d+j)->mag2 = (s2+j)->mag;
                            (d+j)->dmag = (d+j)->mag1 - (d+j)->mag2;
                            (d+j)->r = r_tmp;
                        }
                }
            }
    }

    for (i = 0; i < size; i++)
        if ((d+i)->r != -1.0)
            n++;                    // number of found pairs of stars

    return n;
}

void ShrinkData(struct data *p, const struct data *d, int n)
{
    int i = 0, j;

    for (j = 0; j < n; j++)
        if ((d+j)->r != -1.0)
        {
            (p+i)->id1 = (d+j)->id1;
            (p+i)->id2 = (d+j)->id2;
            (p+i)->x1 = (d+j)->x1;
            (p+i)->x2 = (d+j)->x2;
            (p+i)->y1 = (d+j)->y1;
            (p+i)->y2 = (d+j)->y2;
            (p+i)->mag1 = (d+j)->mag1;
            (p+i)->mag2 = (d+j)->mag2;
            (p+i)->dmag = (d+j)->dmag;
            (p+i)->r = (d+j)->r;
            i++;
        }
}

char WhichColumnSort(int m, int s)
{
    int sum = m + s;
    char c = 'x';

    /*
        c = 0 -> id1
        c = 1 -> id2
        c = 2 -> x1
        c = 3 -> x2
        c = 4 -> y1
        c = 5 -> y2
        c = 6 -> r
        c = 7 -> mag1
        c = 8 -> mag2
        c = 9 -> dmag
    */

    if (m == 0)
        switch (sum)
        {
            case 1: c = '0'; break;
            case 2: c = '2'; break;
            case 3: c = '4'; break;
            case 4: c = '1'; break;
            case 5: c = '3'; break;
            case 6: c = '5'; break;
            case 7: c = '6';
        }
    else if (m == 1)
        switch (sum)
        {
            case 2: c = '0'; break;
            case 3: c = '2'; break;
            case 4: c = '4'; break;
            case 5: c = '1'; break;
            case 6: c = '3'; break;
            case 7: c = '5'; break;
            case 8: c = '6'; break;
            case 9: c = '9';
        }
    else if (m == 2)
        switch (sum)
        {
            case 3: c = '0'; break;
            case 4: c = '2'; break;
            case 5: c = '4'; break;
            case 6: c = '7';
        }
    else if (m == 3)
        switch (sum)
        {
            case 4: c = '1'; break;
            case 5: c = '3'; break;
            case 6: c = '5'; break;
            case 7: c = '8';
        }
    else if (m == 4)
        switch (sum)
        {
           case 5: c = '0'; break;
           case 6: c = '1';
        }


    return c;
}

void SortData(struct data *d, int n, char c)
{
    short s = 0;
    int i, j;
    int id1, id2;
    double x1, x2, y1, y2, r, mag1, mag2, dmag;


    for (i = 0; i < n; i++)
        for (j = 0; j < n-i-1; j++)
        {
            s = 0;
            switch (c)
            {
                case '0': if ((d+j)->id1 > (d+j+1)->id1) s = 1;
                          break;
                case '1': if ((d+j)->id2 > (d+j+1)->id2) s = 1;
                          break;
                case '2': if ((d+j)->x1 > (d+j+1)->x1) s = 1;
                          break;
                case '3': if ((d+j)->x2 > (d+j+1)->x2) s = 1;
                          break;
                case '4': if ((d+j)->y1 > (d+j+1)->y1) s = 1;
                          break;
                case '5': if ((d+j)->y2 > (d+j+1)->y2) s = 1;
                          break;
                case '6': if ((d+j)->r > (d+j+1)->r) s = 1;
                          break;
                case '7': if ((d+j)->mag1 > (d+j+1)->mag1) s = 1;
                          break;
                case '8': if ((d+j)->mag2 > (d+j+1)->mag2) s = 1;
                          break;
                case '9': if ((d+j)->dmag > (d+j+1)->dmag) s = 1;
            }

                if (s)
                {
                    id1 = (d+j)->id1;
                    (d+j)->id1 = (d+j+1)->id1;
                    (d+j+1)->id1 = id1;

                    id2 = (d+j)->id2;
                    (d+j)->id2 = (d+j+1)->id2;
                    (d+j+1)->id2 = id2;

                    x1 = (d+j)->x1;
                    (d+j)->x1 = (d+j+1)->x1;
                    (d+j+1)->x1 = x1;

                    x2 = (d+j)->x2;
                    (d+j)->x2 = (d+j+1)->x2;
                    (d+j+1)->x2 = x2;

                    y1 = (d+j)->y1;
                    (d+j)->y1 = (d+j+1)->y1;
                    (d+j+1)->y1 = y1;

                    y2 = (d+j)->y2;
                    (d+j)->y2 = (d+j+1)->y2;
                    (d+j+1)->y2 = y2;

                    r = (d+j)->r;
                    (d+j)->r = (d+j+1)->r;
                    (d+j+1)->r = r;

                    mag1 = (d+j)->mag1;
                    (d+j)->mag1 = (d+j+1)->mag1;
                    (d+j+1)->mag1 = mag1;

                    mag2 = (d+j)->mag2;
                    (d+j)->mag2 = (d+j+1)->mag2;
                    (d+j+1)->mag2 = mag2;

                    dmag = (d+j)->dmag;
                    (d+j)->dmag = (d+j+1)->dmag;
                    (d+j+1)->dmag = dmag;
                }
        }
}

void PrintData(const struct data *d, int n, int m)
{
    int i;

    for (i = 0; i < n; i++)
        switch (m)
        {
            case 0: printf("%7d %9.3lf %9.3lf %7d %9.3lf %9.3lf %8.4lf\n", (d+i)->id1, (d+i)->x1, (d+i)->y1, (d+i)->id2, (d+i)->x2, (d+i)->y2, (d+i)->r);
                break;
            case 1: printf("%7d %9.3lf %9.3lf %7d %9.3lf %9.3lf %8.4lf %8.4lf\n", (d+i)->id1, (d+i)->x1, (d+i)->y1, (d+i)->id2, (d+i)->x2, (d+i)->y2, (d+i)->r, (d+i)->dmag);
                break;
            case 2: printf("%7d %9.3lf %9.3lf %8.4lf\n", (d+i)->id1, (d+i)->x1, (d+i)->y1, (d+i)->mag1);
                break;
            case 3: printf("%7d %9.3lf %9.3lf %8.4lf\n", (d+i)->id2, (d+i)->x2, (d+i)->y2, (d+i)->mag2);
                break;
            case 4: printf("%7d %7d\n", (d+i)->id1, (d+i)->id2);
        }
}

void MakeReg(const struct data *d, int n)
{
    int i;
    FILE *out = fopen("sdb_xy.reg", "w");

    for (i = 0; i < n; i++)
        fprintf(out, "circle(%.3lf,%.3lf,10) # text={%d} color=yellow\n", (d+i)->x1, (d+i)->y1, (d+i)->id1);

    fclose(out);
}
