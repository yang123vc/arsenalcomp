#include "imagesearch.h"
#include <QtGui/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ImageSearch w;
	w.show();
	return a.exec();
}

