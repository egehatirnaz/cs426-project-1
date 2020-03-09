#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    // Master tasks.
    double timeStart, timeEnd;
    int index;
    int len;    // No. of items on given input.
    int arr[255];
    int min_val = (int) INT16_MAX;

    FILE *in_txt;
    char *provided_path = argv[1];
    char *filepath;

    int curr_val = 0;
    asprintf(&filepath, "%s%s", provided_path, ".txt");
    in_txt = fopen(filepath, "r");
    if (in_txt == NULL) {
        printf("File can\'t be read!\n");
        fclose(in_txt);
        exit(0);
    }
    for (len = 0; fscanf(in_txt, "%d\n", &curr_val) != EOF; len++) {
        arr[len + 1] = curr_val;
    }
    fclose(in_txt);
    arr[0] = len;   // The length is being broadcast and placed on the first index.
    timeStart = MPI_Wtime();
    // Splitting.
    MPI_Status s;
    int size, rank, i, global_min;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);   // Size is no. of processes.  # size = 6
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Bcast ((void *)arr, 255, MPI_INT, 0, MPI_COMM_WORLD); // Broadcast the array.
    if (rank == 0) {   // Master process.
        MPI_Reduce(&global_min, &min_val, 1, MPI_INT,
                   MPI_MIN, 0, MPI_COMM_WORLD);
        timeEnd = MPI_Wtime();
        printf("min-mpi-v2: Min data is %d.\n", min_val);
        printf("Running time = %f seconds.\n", timeEnd - timeStart);
    } else {    // Child process.
        int start, end;
        start = 1;
        end = arr[0];
        global_min = (int) INT16_MAX; // Calculate the local min here.
        for (index = start; index <= end; index++){
            if(arr[index] < global_min){
                global_min = arr[index];
            }
        }
        printf("[%d] calculated %d as min.\n", rank, global_min);
        MPI_Reduce(&global_min, &min_val, 1, MPI_INT,
                   MPI_MIN, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}