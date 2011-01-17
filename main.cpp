#include <iostream>

#include "a10_game.hpp"

using namespace std;

int main(int argc, char** argv)
{
	try
	{
		Kernel kernel(argc, argv);
		kernel.init();

		A10_Game game(kernel);

		kernel.run();

		return EXIT_SUCCESS;
	}
	catch(ticpp::Exception& e)
	{
		cerr << "TinyXML++ caused an EXCEPTION: " << e.what() << endl;
	}
	catch(CriticalError& e)
	{
		cerr << "CRITICAL EXCEPTION: " << e.what() << endl;
	}
	catch(std::exception& e)
	{
		cerr << "EXCEPTION: " << e.what() << endl;
	}
	catch(...)
	{
		cerr << "UNDEFINED ERROR" << endl;
	}

	cout << endl << "Press ENTER to quit" << endl;
	waitForEnter();

	return EXIT_FAILURE;
}
