#ifndef _SDBXY_H_
#define _SDBXY_H_

struct star
{
    int id;
    double x;
    double y;
    double mag;
};

struct data
{
    int id1;
    int id2;
    double x1;
    double x2;
    double y1;
    double y2;
    double mag1;
    double mag2;
    double dmag;
    double r;
};

int NumLines(FILE *file, int *longline);
void ReadFile(FILE *file, struct star *s, int m, int h, int *longline, const char *arg);
int SearchByXY(const struct star *s1, const struct star *s2, struct data *d, int size1, int size2, double off1, double off2, double r_in);
void ShrinkData(struct data *p, const struct data *d, int n);
char WhichColumnSort(int m, int s);
void SortData(struct data *d, int n, char c);
void PrintData(const struct data *d, int n, int m);
void MakeReg(const struct data *d, int n);

#endif
