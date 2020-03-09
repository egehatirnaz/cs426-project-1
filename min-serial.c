#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main(int argc, char *argv[]){
    FILE *in_txt;
    char* filepath;
    asprintf(&filepath, "%s%s", argv[1], ".txt");
    in_txt = fopen(filepath, "r");
    if (in_txt == NULL){
        printf("File can\'t be read!\n");
        fclose(in_txt);
        exit (0);
    }
    int arr[255];
    int index;
    int min_val = (int) INT16_MAX;
    int curr_val = 0;
    int len = 0;    // No. of items on given input.
    for (len = 0; fscanf(in_txt, "%d\n", &curr_val) != EOF; len++) {
        arr[len] = curr_val;
    }
    fclose(in_txt);
    // Analysis begins.
    clock_t time_start = clock();
    for (index = 0; index < len; index++){
        if(arr[index] < min_val){
            min_val = arr[index];
        }
    }
    clock_t time_end = clock();
    printf("min-serial.c : Min value is: %d\n", min_val);
    printf("Running time = %f seconds.\n", (double) (time_end - time_start) / CLOCKS_PER_SEC);
    return 0;
}