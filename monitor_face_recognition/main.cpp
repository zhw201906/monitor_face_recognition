#include "Login.h"
#include <QtWidgets/QApplication>
#include <crtdbg.h>	

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Login w;
	w.show();
	return a.exec();

	_CrtDumpMemoryLeaks();
}
