#include <stdio.h>
#include <string.h>

// Calculates the Disk number and Sector number for RAID-0
void RAIDZero(int Query, int N, int C, int S) {
  int Disk = (Query / C) % N;
  int Sector = (Query % C) + (Query / (N * C)) * C;
  printf("%d %d\n", Disk, Sector);
}

// Calculates the Disk numbers and Sector number for RAID-1
void RAIDOne(int Query, int N, int C, int S) {
  int FirstDisk = 0;
  int SecondDisk = 1;
  int Sector = Query;
  printf("%d %d %d\n", FirstDisk, Sector, SecondDisk);
}

// Calculates the Disk numbers and Sector number for RAID-01
void RAIDZeroOne(int Query, int N, int C, int S) {
  int FirstDisk = (Query & 1);
  int SecondDisk = (Query & 1) + 2;
  int Sector = Query / 2;
  printf("%d %d %d\n", FirstDisk, Sector, SecondDisk);
}

// Calculates the Disk numbers and Sector number for RAID-10
void RAIDOneZero(int Query, int N, int C, int S) {
  int FirstDisk = (Query & 1) * 2;
  int SecondDisk = FirstDisk + 1;
  int Sector = Query / 2;
  printf("%d %d %d\n", FirstDisk, Sector, SecondDisk);
}

// Calculates the Disk, Sector, and Parity number for RAID-4
void RAIDFour(int Query, int N, int C, int S) {
  int Sector = Query / (N - 1);
  int Disk = Query % (N - 1);
  int ParityDisk = N - 1;
  printf("%d %d %d\n", Disk, Sector, ParityDisk);
}

// Calculates the Disk, Sector, and Parity number for RAID-5
void RAIDFive(int Query, int N, int C, int S) {
  int Sector = Query / (N - 1);
  int ParityDisk = (N - (Sector % N) - 1);
  int Disk = Query % N;
  printf("%d %d %d\n", Disk, Sector, ParityDisk);
}

int main(int argc, char const *argv[]) {
  char level[10];
  int N, C, S, Q;
  scanf("%s%d%d%d%d", level, &N, &C, &S, &Q);
  while (Q-- > 0) {
    int Query;
    scanf("%d", &Query);
    if (strcmp(level, "01") == 0)
      RAIDZeroOne(Query, N, C, S);
    else if (strcmp(level, "10") == 0)
      RAIDOneZero(Query, N, C, S);
    else if (strcmp(level, "0") == 0)
      RAIDZero(Query, N, C, S);
    else if (strcmp(level, "1") == 0)
      RAIDOne(Query, N, C, S);
    else if (strcmp(level, "4") == 0)
      RAIDFour(Query, N, C, S);
    else
      RAIDFive(Query, N, C, S);
  }
  return 0;
}