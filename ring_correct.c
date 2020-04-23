/*
 * Copyright (c) 2004-2006 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2006      Cisco Systems, Inc.  All rights reserved.
 *
 * Simple ring test program in C.
 */

#include <stdio.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
    int rank, size, next, prev, tag = 201;
	int i, j;
	const long int num_elements=1024*1024*1024;
	double* message;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    next = (rank + 1) % size;
    prev = (rank + size - 1) % size;
	message = malloc(num_elements*sizeof(double));

	if (0 == rank) {
		
		srand48(0);
		
		for(i=0;i<num_elements;i++){ message[i]=drand48(); }

        MPI_Send(message, num_elements, MPI_DOUBLE, next, tag, MPI_COMM_WORLD);
    }

	for(j=0;j<10;j++){
 
        MPI_Recv(message, num_elements, MPI_DOUBLE, prev, tag, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);

        for(i=0;i<num_elements-1;i++){
			message[i]+= rank*message[i+1];
			
		}
		if(j==9 && rank ==0){
			break;
		}
		else{	
        	MPI_Send(message, num_elements, MPI_DOUBLE, next, tag, MPI_COMM_WORLD);
		}
	}

    if (rank == 0) {
		double value =0;
		for(i=0;i<num_elements;i++){
			value+=message[i];
		}

		free(message);

		printf("The final value is %f\n",value);

	}else{free(message);}
	
    MPI_Finalize();
   return 0;
}
