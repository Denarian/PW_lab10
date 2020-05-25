#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <mpi.h>

typedef struct 
{
    char name[50];
    char surname[50];
    char data[5000];
}Bogata;



int main(int argc, char *argv[])
{

	int npes;
	int myrank;
	double start, end;

    int struct_count = atoi(argv[1]); // ilość wiadomości do przesłania
    int pack_size = atoi(argv[2]);
    int iter_count = struct_count / pack_size; // ilość iteracji

    MPI_Datatype komTYPE;
    MPI_Datatype type[] = {MPI_CHAR, MPI_CHAR, MPI_CHAR};
    int blocklen[] = {50,50,5000};

    MPI_Aint disp[3];

    disp[0] = offsetof(Bogata,name);
    disp[1] = offsetof(Bogata,surname);
    disp[2] = offsetof(Bogata,data);

	MPI_Init(&argc, &argv);
	MPI_Type_create_struct(3,blocklen,disp, type, &komTYPE);
    MPI_Type_commit(&komTYPE);
    

	
	MPI_Comm_size(MPI_COMM_WORLD, &npes);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    MPI_Status status;

    start = MPI_Wtime();

    switch(myrank)
    {
        case 0:
        {
            Bogata * x = (Bogata *)malloc(sizeof(Bogata) * pack_size);
            for(int j = 0; j < iter_count ; j++)
            {
                MPI_Send(x, pack_size, komTYPE, 1, 13, MPI_COMM_WORLD);
            }
            break;
        }
        case 1:
        {
            Bogata * y = (Bogata *)malloc(sizeof(Bogata) * pack_size);
            for(int j = 0; j < iter_count; j++)
            { 
                MPI_Recv(y, pack_size, komTYPE, 0, 13, MPI_COMM_WORLD, &status);
            }
        }
	}

	end = MPI_Wtime();
	MPI_Finalize();

	if (myrank == 1) {
		printf("%d po %d : %lf\n",struct_count , pack_size, end-start);
	}
	return 0;
}
