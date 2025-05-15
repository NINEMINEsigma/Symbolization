#include <iostream>

int main(int argv, char** argc)
{
	if (argv != 2)
	{
		std::cerr << "Usage: " << argc[0] << " <path to symbolization file>" << std::endl;
		return 1;
	}
	/*Symbolization symbolization(argc[1]);
	symbolization.symbolize();*/
	return 0;
}