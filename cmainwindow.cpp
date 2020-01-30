#include "cmainwindow.h"
#include "ui_cmainwindow.h"

#include "cmeteostat.h"
#include "cmeteodata.h"

#include <QClipboard>

#include <QDebug>


cMainWindow::cMainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::cMainWindow)
{
	ui->setupUi(this);

	cMeteoStat		meteoStat("Q1H5Feca");
	cMeteoDataList	list		= meteoStat.getHistoricalData(QDate(2020, 1, 16), QDate(2020, 1, 16), 11035);
	QClipboard*		clipboard	= QGuiApplication::clipboard();
	QString			text		= "";

	for(int x = 0;x < list.count();x++)
	{
		cMeteoData*	lpData	= list[x];

		text.append(lpData->dateTime().toString("yyyy-MM-dd hh:mm:ss"));

		text.append("\t");
		text.append(lpData->dateTimeLocal().toString("yyyy-MM-dd hh:mm:ss"));

		text.append("\t");
		text.append(QString::number(lpData->temperature()));

		text.append("\t");
		text.append(QString::number(lpData->dewpoint()));

		text.append("\t");
		text.append(QString::number(lpData->humidity()));

		text.append("\t");
		text.append(QString::number(lpData->windspeed()));

		text.append("\t");
		text.append(QString::number(lpData->peakgust()));

		text.append("\t");
		text.append(QString::number(lpData->winddirection()));

		text.append("\t");
		text.append(QString::number(lpData->pressure()));

		text.append("\t");
		text.append(QString::number(lpData->condition()));

		text.append("\n");
	}

	clipboard->setText(text);
}

cMainWindow::~cMainWindow()
{
	delete ui;
}

