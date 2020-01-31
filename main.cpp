#include "cmainwindow.h"

#include <QApplication>

#include <QFile>
#include <QTextStream>
#include <QSettings>

#include "csplashscreen.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	a.setApplicationVersion(APP_VERSION);
	a.setApplicationDisplayName("weatherData");
	a.setOrganizationName("WIN-DESIGN");
	a.setOrganizationDomain("windesign.at");
	a.setApplicationName("weatherData");

	QSettings		settings;

	QPixmap			pixmap(":/images/splash.png");
	QFont			splashFont;
	cSplashScreen*	lpSplash	= new cSplashScreen(pixmap, splashFont);

	lpSplash->show();
	a.processEvents();

	lpSplash->showStatusMessage(QObject::tr("<center>initializing...</denter>"));

	cMainWindow w(lpSplash);


	if(settings.value("main/maximized").toBool())
		w.showMaximized();
	else
		w.show();

	lpSplash->finish(&w);
	delete lpSplash;

	return a.exec();
}
