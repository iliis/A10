#include <iostream>

#include "a10_game.hpp"

using namespace std;

void close_on_esc(KEY key, bool state, Kernel* k)
{
	assert(k);

	if(key == KEY_ESCAPE and state)
		k->stop();
}

int main(int argc, char** argv)
{
	try
	{
		Kernel kernel(argc, argv);
		kernel.init();

		kernel.inputMgr->addKeyListener(boost::bind(close_on_esc, _1, _2, &kernel));

		WidgetPtr skedit = kernel.guiMgr->createWidget<SkeletonEditorWidget>("sk_editor");
		skedit->setSize(kernel.graphicsMgr->getScreenSize().cast<double>());

		//boost::shared_ptr<A10_Game> game = boost::shared_ptr<A10_Game>(new A10_Game(&kernel));
		//game->init();

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
