#include <complex>
#include <iostream>
#include <fstream>
#include <pthread.h>

using namespace std;

char **mat;
int max_row, max_column, max_n;

struct thread_data
{
	int start;
	int end;
};

void *draw(void *threadarg)
{
	struct thread_data *my_data;
	my_data = (struct thread_data *)threadarg;
	int start = my_data->start;
	int end = my_data->end;

	for (int r = start; r < end; ++r)
	{
		for (int c = 0; c < max_column; ++c)
		{
			complex<float> z = 0;
			int n = 0;
			while (abs(z) < 2.0 && ++n < max_n)
				z = z * z + complex<float>(
								(float)c * 2 / max_column - 1.5,
								(float)r * 2 / max_row - 1);
			if (n == max_n)
				mat[r][c] = '#';
			else
				mat[r][c] = '.';
		}
	}
	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	int NUM_THREADS = atoi(argv[1]);
	max_row = atoi(argv[2]);
	max_column = atoi(argv[3]);
	max_n = atoi(argv[4]);

    pthread_t threads[NUM_THREADS];

    mat = (char **)malloc(sizeof(char *) * max_row);


	for (int i = 0; i < max_row; i++)
		mat[i] = (char *)malloc(sizeof(char) * max_column);


	struct thread_data td[NUM_THREADS];
	for (int i = 0; i < NUM_THREADS; i++)
	{
		td[i].start = i * (max_row / NUM_THREADS);
		td[i].end = (i + 1) * (max_row / NUM_THREADS);
		if (i == NUM_THREADS - 1)
			td[i].end = max_row;
	}

	for (int i = 0; i < NUM_THREADS; i++)
	{
		pthread_create(&threads[i], NULL, draw, (void *)&td[i]);
	}

	for (int i = 0; i < NUM_THREADS; i++)
	{
		pthread_join(threads[i], NULL);
	}

	std::ofstream output("../outputs/outputpthread.txt");

	for (int r = 0; r < max_row; ++r)
	{
		for (int c = 0; c < max_column; ++c)
			output << mat[r][c];
		output << '\n';
	}

	output.close();
}
