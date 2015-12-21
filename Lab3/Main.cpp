#include <stdio.h>
#include <mpi.h>
#include <math.h> 
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	int rank, size, n;
	double t1, t2;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (rank == 0)
	{
		cout << "Enter elements amount: \n";
		cin >> n;
	}

	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		t1 = MPI_Wtime();
	}

	int residue = n % size;
	int k = n / size;
	if (residue > 0 && rank - residue < 0)
	{
		k++;
	}

	double pi = 0;
	for (int i = rank; i < n; i += size)
	{
		pi += pow(-1, i) / (2 * i + 1);
	}
	if (rank == 0)
	{
		for (int m = 1; m < size; m++)
		{
			MPI_Status status;
			double sumPi;
			MPI_Recv(&sumPi, 1, MPI_DOUBLE, m, m, MPI_COMM_WORLD, &status);
			pi += sumPi;
		}
	}
	else
	{
		MPI_Send(&pi, 1, MPI_DOUBLE, 0, rank, MPI_COMM_WORLD);
	}

	MPI_Barrier(MPI_COMM_WORLD);

	if (rank == 0)
	{
		t2 = MPI_Wtime();
		cout.precision(17);
		cout << "\nPi: " << pi*4;
		cout << "\nTime: " << (t2 - t1);
	}

	MPI_Finalize();
}