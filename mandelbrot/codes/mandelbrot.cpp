#include <complex>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv)
{
	int max_row, max_column, max_n;

	max_row = atoi(argv[1]);
	max_column = atoi(argv[2]);
	max_n = atoi(argv[3]);

	char **mat = (char **)malloc(sizeof(char *) * max_row);

	for (int i = 0; i < max_row; i++)
		mat[i] = (char *)malloc(sizeof(char) * max_column);

	for (int r = 0; r < max_row; ++r)
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

	std::ofstream output("../outputs/outputsequential.txt");

	for (int r = 0; r < max_row; ++r)
	{
		for (int c = 0; c < max_column; ++c)
			output << mat[r][c];
		output << '\n';
	}

	output.close();
}
