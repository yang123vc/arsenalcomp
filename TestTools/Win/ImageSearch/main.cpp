#include "imagesearch.h"
#include <QtGui/QApplication>


int main(int argc, char *argv[])
{

		QApplication a(argc, argv);
		a.setQuitOnLastWindowClosed(false);

		QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

		QStringList lst = a.arguments();

		QString path_arg = "";
		for(size_t i = 0; i < lst.size(); ++i)
		{
				QString tmp = lst[i];

				if(tmp.startsWith("--path="))
				{
						int idx = tmp.indexOf("--path=");
						tmp.remove(0, strlen("--path="));
				}
		}

		const wchar_t *wcs = (const wchar_t*)path_arg.utf16();

		ImageSearch w;
		


		w.LoadSamples(wcs);
		w.show();

		return a.exec();
}

