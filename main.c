#include <stdio.h>
#include <jmorecfg.h>
#include "mpi.h"

int array[4];
int PASSTEMP = 0;
int PASSSUM ;

boolean calculate(int x, int CalculateTime) {
    int i, j, temp, min, max;
    for (i = 0; i < 4; i++) {
        array[i] = x % 10;
        x /= 10;
    }
    for (j = 0; j < 3; j++) {
        for (i = 0; i < 3 - j; i++) {
            if (array[i] > array[i + 1]) {
                temp = array[i];
                array[i] = array[i + 1];
                array[i + 1] = temp;
            }
        }
    }
    min = array[0] * 1000 + array[1] * 100 + array[2] * 10 + array[3];
    max = array[3] * 1000 + array[2] * 100 + array[1] * 10 + array[0];
    x = max - min;
    if (x == 6174 && CalculateTime <= 7) {
        ++PASSTEMP;
        return TRUE;
    }
    return calculate(x, ++CalculateTime);
}

void run(int fromWhere, int endWhere, int rank) {
    for (int i = fromWhere; i <= endWhere; ++i) {
        if (i % 1111 == 0) {
            continue;
        }
        calculate(i, 1);
    }
    printf("第%d个进程成功的个数：%d\n", rank, PASSTEMP);

}

int main() {
    int rank;
    int size;
    MPI_Init(0, 0);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int counnt = 9999 - 1000 + 1;
    int eachCalucate = counnt / size;

    switch (size) {
        case 1:
            if (rank==0)
            run(1000, 9999, rank);
            break;
        case 2:
            if (rank==0){
                run(1000, 1000 + eachCalucate, rank);
            } else if (rank==1)
            run(1000 + eachCalucate + 1, 9999, rank);
            break;
        case 3:
            if (rank==0){
                run(1000, 1000 + eachCalucate, rank);
            } else if (rank==1){
                run(1000 + eachCalucate + 1, 1000 + eachCalucate * 2, rank);
            } else if (rank==2)
            run(1000 + eachCalucate * 2 +1, 9999, rank);
            break;
        case 4:
            if (rank==0){
                run(1000, 1000 + eachCalucate, rank);
            } else if (rank==1){
                run(1000 + eachCalucate + 1, 1000 + eachCalucate * 2, rank);
            } else if (rank==2){
                run(1000 + eachCalucate * 2 +1, 1000 + eachCalucate * 3, rank);
            } else if (rank==3){
                run(1000 + eachCalucate * 3 +1, 9999, rank);
            }




            break;
        default:
            break;

    }
    MPI_Reduce(&PASSTEMP, &PASSSUM, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank==0)
    printf("总共成功的个数%d\n",PASSSUM);
    MPI_Finalize();
    return 0;
}