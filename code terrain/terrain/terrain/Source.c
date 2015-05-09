#include <stdlib.h>
#include <stdio.h>

int main(int argc, char ** argv)
{
	int i, j;
	double x = 110, y = 40;

	int _x, _y;
	_x =  (int) (x / 100.);
	_y =  (int) (y / 100.);

	printf(" _x : %d, _y : %d \n", _x, _y);
	printf("X ");
	for (i = 0; i < 60; i++)
		printf("~");
	printf("\n");

	for (i = 0; i < 20 ; i++)
	{
		printf(" |");
		for (j = 0; j < 30; j++)
		{
			if (_x == i  && _y == j )
				printf("O ");
			else
				printf("- ");
		}
		printf("|");
		if (i == 10)
			printf("X");
		printf("\n\r");
	}

	printf("X ");
	for (i = 0; i < 60; i++)
		printf("~");
	printf("\n");
	system("pause");
	return EXIT_SUCCESS;
}