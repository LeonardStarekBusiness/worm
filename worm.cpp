#include "worm.hpp"
#include <stdexcept>

int main(int ac, char **av)
{
	if (!(ac == 2 || (ac == 3 && av[2] == std::string("--prep"))))
		return 1;
	try
	{
		Program baal(av[1], ac == 3);
		baal.preprocess();
		baal.execute();
	}
	catch (std::runtime_error & e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}
}