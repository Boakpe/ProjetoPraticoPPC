#include <iostream>
#include <vector>
#include <fstream>
#include <omp.h>

typedef unsigned int uint;

struct thread_data
{
	int start;
	int end;
};

// Gets the input value from the input file
void get_values(char* filename, std::vector<uint>& values) {
     std::fstream infile(filename);
     uint val;
     // The first value is the number of elements
     infile >> val;
     values.reserve(val);
     values.clear();
     // Read the elements
     while (infile >> val) {
          values.push_back(val);
     }
     infile.close();
}

int main (int argc, char* argv[]){
     if (argc == 3) {\
          int NUM_THREADS = atoi(argv[1]);
          std::vector<uint> values;
          get_values(argv[2], values);
          // Open the output file
          std::ofstream outfile("problem_output_omp");
          // Start processing
          uint n = values.size();
          uint prefix = 0;

          struct thread_data td[NUM_THREADS];
          for (int i = 0; i < NUM_THREADS; i++)
          {
               td[i].start = i * (n / NUM_THREADS);
               td[i].end = (i + 1) * (n / NUM_THREADS);
               if (i == NUM_THREADS - 1)
               {
                    td[i].end = n;
               }
          }
          #pragma omp parallel for ordered num_threads(NUM_THREADS)
          for (int i = 0; i < NUM_THREADS; i++)
          {
               int thread_id = omp_get_thread_num();
               #pragma omp ordered
               {
                    for (int j = td[thread_id].start; j < td[thread_id].end; j++)
                    {
                         prefix += values[j];
                         values[j] = prefix;
                         outfile << prefix << std::endl;
                    }
               }
          }
          outfile.close();
     } else {
          std::cout << "Usage: ./prefix-sum problem_input \n";
     }
}
