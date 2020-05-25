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

#define SEND_BUFF_SIZE 100

int main(int argc, char *argv[])
{

	int npes;
	int myrank;
	double start, end;

    int struct_count = atoi(argv[1]); // ilość wiadomości do przesłania
    int pack_size = atoi(argv[2]);
    int iter_count = struct_count / pack_size; // ilość iteracji
    
    int position = 0;

    Bogata *buff = (Bogata *)malloc(sizeof(Bogata) * pack_size);
    Bogata *buff_for_send = (Bogata *)malloc(SEND_BUFF_SIZE * sizeof(Bogata) * pack_size), bbuf;

    

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
    
    int s, buf_size, bbuf_size;
    MPI_Pack_size(pack_size,komTYPE, MPI_COMM_WORLD,&s);
    buf_size = s + MPI_BSEND_OVERHEAD;
   

    MPI_Status status;
    start = MPI_Wtime();
    MPI_Buffer_attach(buff_for_send,SEND_BUFF_SIZE * buf_size);// buffor dla Bsend
    switch(myrank)
    {
        case 0:
        {
            Bogata * out_buff = (Bogata *)malloc(sizeof(Bogata) * pack_size);
            
            for(int j = 0; j < iter_count; j++)
            {
               
                MPI_Pack(out_buff, pack_size, komTYPE, buff,sizeof(Bogata) * pack_size ,&position, MPI_COMM_WORLD);
                MPI_Bsend(buff, position, MPI_PACKED, 1, 13, MPI_COMM_WORLD);
                if(!(j % SEND_BUFF_SIZE))//Upewnienie się że w buforze jest miejsce 
                {
                    MPI_Buffer_detach( &bbuf, &bbuf_size );
                    MPI_Buffer_attach(buff_for_send,SEND_BUFF_SIZE * buf_size);// buffor dla Bsend
                }
                position = 0;
            }
            break;
        }
        case 1:
        {
            Bogata * in_buff = (Bogata *)malloc(sizeof(Bogata) * pack_size);
            for(int j = 0; j < iter_count; j++)
            { 
                MPI_Recv(buff,sizeof(Bogata) * pack_size, MPI_PACKED, 0, 13, MPI_COMM_WORLD, &status);
                MPI_Unpack(buff,sizeof(Bogata) * pack_size, &position, in_buff, pack_size, komTYPE, MPI_COMM_WORLD);
                position = 0;
            }
            break;
        }
	}


	end = MPI_Wtime();
    
	MPI_Finalize();

	if (myrank == 1) {
		printf("%d po %d : %lf\n",struct_count , pack_size, end-start);
	}
	return 0;
}
