#include <iostream>
#include <vector>
#include <fstream>
#include <pthread.h>

typedef unsigned int uint;

struct thread_data
{
	int start;
	int end;
    int thread_id;
};

std::ofstream outfile("problem_output_pthread");
uint prefix = 0;
std::vector<uint> values;
int current_thread = 0;


void * prefix_sum(void *threadarg)
{
    struct thread_data *my_data;
    my_data = (struct thread_data *)threadarg;
    int start = my_data->start;
    int end = my_data->end;

    while (current_thread != my_data->thread_id)
    {
        // Waiting for the previous thread to finish
        //std::cout << "Waiting for thread " << current_thread << " to finish\n";
    }

    for (int i = start; i < end; ++i)
    {
        prefix += values[i];
        outfile << prefix << std::endl;
    }

    current_thread++;

    pthread_exit(NULL);
}

// Gets the input value from the input file
void get_values(char *filename, std::vector<uint> &values)
{
    std::fstream infile(filename);
    uint val;
    // The first value is the number of elements
    infile >> val;
    values.reserve(val);
    values.clear();
    // Read the elements
    while (infile >> val)
    {
        values.push_back(val);
    }
    infile.close();
}

int main(int argc, char *argv[])
{
    if (argc == 3)
    {
        //std::vector<uint> values;
        int NUM_THREADS = atoi(argv[1]);
        get_values(argv[2], values);
        // Open the output file
        //std::ofstream outfile("problem_output");
        // Start processing
        uint n = values.size();
        //uint prefix = 0;
        pthread_t threads[NUM_THREADS];

	    struct thread_data td[NUM_THREADS];
        for (int i = 0; i < NUM_THREADS; i++)
        {
            td[i].start = i * (n / NUM_THREADS);
            td[i].end = (i + 1) * (n / NUM_THREADS);
            if (i == NUM_THREADS - 1)
            {
                td[i].end = n;
            }
            td[i].thread_id = i;
        }

        for (int i = 0; i < NUM_THREADS; i++)
        {
            pthread_create(&threads[i], NULL, prefix_sum, (void *)&td[i]);
        }

        for (int i = 0; i < NUM_THREADS; i++)
        {
            pthread_join(threads[i], NULL);
        }


        /* for (uint i = 0; i < n; ++i)
        {
            prefix += values[i];
            outfile << prefix << std::endl;
        } */
        outfile.close();
    }
    else
    {
        std::cout << "Usage: ./prefix-sum N_THREADS problem_input \n";
    }
}
