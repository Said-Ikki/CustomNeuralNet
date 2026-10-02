#include <mpi.h>
#include <stdio.h>

int MANY_LOOPS = 0;
const int CHECK = 50;

int repeat(int world_rank) {
    for(int i = 0; i < world_rank+1; i++) {
        printf( "Proc %d ", world_rank );
        //fflush(stdout);
    }
    return 1;
}

int do_things_repeat_of_global(int world_rank) {
    for(;MANY_LOOPS < CHECK; MANY_LOOPS++) {
        printf( "Quiet %d with Check %d ", world_rank, MANY_LOOPS );
        fflush(stdout);
    }
    return 1; // the answer: no
}

int breakup_work(int proc_no, int proc_amount, int loop_count) {
    for(int i = proc_no; i < loop_count; i+=proc_amount) {
        printf("Proc %d/%d for %d/%d \n", proc_no+1, proc_amount, i+1, loop_count);
    }
    return 1;
}

int main(int argc, char **argv)
{
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
  
    // Get the number of processes ssociated with the communicator
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the calling process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    printf("Hello world from process %s with rank %d out of %d processors\n", processor_name, world_rank, world_size);
    fflush(stdout);

    //repeat(world_rank);
    //do_things_repeat_of_global(world_rank);
    breakup_work(world_rank, world_size, CHECK);


    // Finalize: Any resources allocated for MPI can be freed
    MPI_Finalize();
}

// compile
/* 
cl.exe /EHsc /MD "/IC:\Program Files (x86)\Microsoft SDKs\MPI\Include" main.c /Fe:main.exe /link "/LIBPATH:C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64" msmpi.lib

// run
mpiexec -n 4 main.exe
the 4 means the number of threads to create for the process

*/