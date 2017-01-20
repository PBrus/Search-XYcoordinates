#ifndef _COMM_H_
#define _COMM_H_

void Hello();
void Version();
void Help();
void ToManyArg();
void ErrInFile();
void ErrOption();
void Err_H();
void Err_R();
short Correct_S_M(int m, int s);
void TooBig_H(int h, const char *file);
void EmptyFile(const char *arg);
void ErrReadHeader(const char *arg);
void ErrReadLine(const char *arg, int line);
void MemoryStar();
void MemoryData();
void NotMemoBuff();

#endif
