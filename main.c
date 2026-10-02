#include <stdio.h>
#include <limits.h>
#include <mpi.h>
#include <windows.h>

const int IS_PRIME = 0;
const int NOT_PRIME = 1;
const unsigned long long PRIME_LIMIT = 1000000;

int is_prime( int val ) {
    for(int k = 2; k < val; k++) {
        if( val % k == 0 ) {
            return NOT_PRIME;
        }
    }
    return IS_PRIME;
}

int maxim(int a, int b) {

    if(a > b) {
        return a;
    }
    else {
        return b;
    }

}


int main(int argc, char **argv) {
    //printf("Hi there, world");
    
    int number_of_prime = 2;

    MPI_Init(&argc, &argv);

    int core_no;
    int core_count;

    MPI_Comm_size(MPI_COMM_WORLD, &core_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &core_no);

    int start   = (PRIME_LIMIT / core_count) * core_no;
    int end     = (PRIME_LIMIT / core_count) * (core_no + 1) - 1;

    int prev_prime = 2;
    int curr_prime = 2;
    int max_prime_diff = 0;

    int time = -1 * MPI_Wtime();

    if(start < 2) {
        start = 2;
    }
    while( start <= end || prev_prime < end) {

        int is_it_prime = is_prime(start);

        if(is_it_prime == IS_PRIME) {
            if(prev_prime == 2 && curr_prime == 2) {
                prev_prime = start;
                curr_prime = start;
            }
            max_prime_diff = maxim( max_prime_diff, start - curr_prime );
            prev_prime = curr_prime;
            curr_prime = start;
        }

        
        start++;
    }

    int final_value;
    int count = 1;
    int core_to_hold = 0;
    MPI_Reduce( &max_prime_diff, &final_value, count, MPI_INT, MPI_MAX, core_to_hold, MPI_COMM_WORLD );

    //printf("Local Maximum: %d\n", max_prime_diff);
    //printf("Current Prime: %d \nPrevious Prime: %d \n", curr_prime, prev_prime);
    //printf("Difference Between Current and Previous: %d \n", curr_prime - prev_prime);


    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    time = time + MPI_Wtime();

    if(core_no == 0) {
        printf("Maximum Prime Difference: %d \n", final_value);
        printf("Time to Complete: %d seconds", time);
    } 

    MPI_Finalize();
    

    return 0;
}

/*

cl.exe /EHsc /MD "/IC:\Program Files (x86)\Microsoft SDKs\MPI\Include" main.c /Fe:main.exe /link "/LIBPATH:C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64" msmpi.lib

mpiexec -n 4 main.exe


*/