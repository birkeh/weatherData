#include "cmainwindow.h"
#include "ui_cmainwindow.h"

#include "cmeteostat.h"
#include "cmeteodata.h"

#include <QSettings>
#include <QStandardItem>

#include <QCloseEvent>

#include <QClipboard>

#include <QShortcut>

#include <QDebug>


#define SEPARATOR	"\t"


cMainWindow::cMainWindow(cSplashScreen* lpSplashScreen, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::cMainWindow),
	m_lpSplashScreen(lpSplashScreen)
{
	initUI();
	createActions();

	m_lpSeriesTemperature	= new QLineSeries();
	m_lpSeriesPressure		= new QLineSeries();
	m_lpSeriesHumidity		= new QLineSeries();

	m_lpChart	= new QChart();

	m_lpAxisX	= new QDateTimeAxis;
	m_lpAxisX->setTickCount(10);
	m_lpAxisX->setFormat("dd.MM.yyyy hh:mm");
	m_lpAxisX->setTitleText("Date");
	m_lpChart->addAxis(m_lpAxisX, Qt::AlignBottom);

	m_lpChart->addSeries(m_lpSeriesTemperature);
	m_lpAxisYTemperature = new QValueAxis;
	m_lpAxisYTemperature->setLabelFormat("%i");
	m_lpAxisYTemperature->setTitleText("Temperature");
	m_lpChart->addAxis(m_lpAxisYTemperature, Qt::AlignLeft);
	m_lpSeriesTemperature->attachAxis(m_lpAxisYTemperature);
	m_lpSeriesTemperature->attachAxis(m_lpAxisX);
	m_lpSeriesTemperature->setName("Temperature");
	m_lpSeriesTemperature->setPointLabelsFormat("@yPoint");
	m_lpSeriesTemperature->setPointLabelsVisible(true);
	m_lpSeriesTemperature->setColor(Qt::blue);

	m_lpChart->addSeries(m_lpSeriesPressure);
	m_lpAxisYPressure = new QValueAxis;
	m_lpAxisYPressure->setLabelFormat("%i");
	m_lpAxisYPressure->setTitleText("Pressure");
	m_lpChart->addAxis(m_lpAxisYPressure, Qt::AlignLeft);
	m_lpSeriesPressure->attachAxis(m_lpAxisYPressure);
	m_lpSeriesPressure->attachAxis(m_lpAxisX);
	m_lpSeriesPressure->setName("Pressure");
	m_lpSeriesPressure->setPointLabelsFormat("@yPoint");
	m_lpSeriesPressure->setPointLabelsVisible(true);
	m_lpSeriesPressure->setColor(Qt::red);

	m_lpChart->addSeries(m_lpSeriesHumidity);
	m_lpAxisYHumidity = new QValueAxis;
	m_lpAxisYHumidity->setLabelFormat("%i");
	m_lpAxisYHumidity->setTitleText("Humidity");
	m_lpChart->addAxis(m_lpAxisYHumidity, Qt::AlignLeft);
	m_lpSeriesHumidity->attachAxis(m_lpAxisYHumidity);
	m_lpSeriesHumidity->attachAxis(m_lpAxisX);
	m_lpSeriesHumidity->setName("Humidity");
	m_lpSeriesHumidity->setPointLabelsFormat("@yPoint");
	m_lpSeriesHumidity->setPointLabelsVisible(true);
	m_lpSeriesHumidity->setColor(Qt::gray);

	m_lpChart->legend()->show();
	m_lpChart->setTitle("Weather");

	ui->m_lpChart->setChart(m_lpChart);
	ui->m_lpChart->setRenderHint(QPainter::Antialiasing);


	onShowTemperature(0);
	onShowPressure(0);
	onShowHumidity(0);
}

cMainWindow::~cMainWindow()
{
	delete ui;
}

void cMainWindow::closeEvent(QCloseEvent *event)
{
	QSettings	settings;
	settings.setValue("main/width", QVariant::fromValue(size().width()));
	settings.setValue("main/height", QVariant::fromValue(size().height()));
	settings.setValue("main/x", QVariant::fromValue(x()));
	settings.setValue("main/y", QVariant::fromValue(y()));
	if(this->isMaximized())
		settings.setValue("main/maximized", QVariant::fromValue(true));
	else
		settings.setValue("main/maximized", QVariant::fromValue(false));

	settings.setValue("show/temperature", QVariant::fromValue(ui->m_lpShowTemperature->isChecked()));
	settings.setValue("show/pressure", QVariant::fromValue(ui->m_lpShowPressure->isChecked()));
	settings.setValue("show/humidity", QVariant::fromValue(ui->m_lpShowHumidity->isChecked()));

	event->accept();
}

void cMainWindow::initUI()
{
	QSettings	settings;

	ui->setupUi(this);

	if(!settings.value("main/maximized").toBool())
	{
		qint32		iX		= settings.value("main/x", QVariant::fromValue(-1)).toInt();
		qint32		iY		= settings.value("main/y", QVariant::fromValue(-1)).toInt();
		qint32		iWidth	= settings.value("main/width", QVariant::fromValue(-1)).toInt();
		qint32		iHeight	= settings.value("main/height", QVariant::fromValue(-1)).toInt();

		if(iWidth != -1 && iHeight != -1)
			resize(iWidth, iHeight);
		if(iX != -1 && iY != -1)
			move(iX, iY);
	}

	ui->m_lpDateFrom->setDate(QDate::currentDate().addDays(-1));
	ui->m_lpDateTo->setDate(QDate::currentDate().addDays(-1));

	m_lpListModel	= new QStandardItemModel;
	ui->m_lpList->setModel(m_lpListModel);

	QStringList	headerLabels	= QStringList() << tr("Date") << tr("Temperature") << tr("Pressure") << tr("Humidity") << tr("Windspeed") << tr("Winddirection");
	m_lpListModel->setHorizontalHeaderLabels(headerLabels);

	ui->m_lpShowTemperature->setChecked(settings.value("show/temperature", QVariant::fromValue(true)).toBool());
	ui->m_lpShowPressure->setChecked(settings.value("show/pressure", QVariant::fromValue(true)).toBool());
	ui->m_lpShowHumidity->setChecked(settings.value("show/humidity", QVariant::fromValue(true)).toBool());
}

void cMainWindow::createActions()
{
//	setToolButtonStyle(Qt::ToolButtonFollowStyle);

//	createFileActions();
//	createContextActions();

//	connect(ui->m_lpFileList,		&cTreeView::deleteEntrys,	this,		&cMainWindow::onDeleteEntrys);
//	connect(ui->m_lpThumbnailSize,	&QSlider::valueChanged,		this,		&cMainWindow::onThumbnailSize);

	connect(ui->m_lpGo,					&QPushButton::clicked,		this,		&cMainWindow::onGo);
	connect(ui->m_lpCopy,				&QPushButton::clicked,		this,		&cMainWindow::onCopy);

	connect(ui->m_lpShowTemperature,	&QCheckBox::stateChanged,	this,		&cMainWindow::onShowTemperature);
	connect(ui->m_lpShowPressure,		&QCheckBox::stateChanged,	this,		&cMainWindow::onShowPressure);
	connect(ui->m_lpShowHumidity,		&QCheckBox::stateChanged,	this,		&cMainWindow::onShowHumidity);

	QShortcut * shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_C), this, SLOT(onCopyRow()));
	shortcut->setAutoRepeat(false);
}

void cMainWindow::createContextActions()
{
}

void cMainWindow::createFileActions()
{
//	m_lpFileToolBar	= addToolBar("file");

//	const QIcon	openIcon			= QIcon::fromTheme("document-open");
//	m_lpOpenFileAction				= m_lpFileToolBar->addAction(openIcon, tr("&Open..."), this, &cMainWindow::onAddFile);
//	m_lpOpenFileAction->setShortcut(QKeySequence::Open);

//	const QIcon	openDirectoryIcon	= QIcon::fromTheme("folder");
//	m_lpOpenDirectoryAction			= m_lpFileToolBar->addAction(openIcon, tr("&Open Folder..."), this, &cMainWindow::onAddFolder);


//	m_lpListToolBar	= addToolBar("list");

//	const QIcon	deleteIcon			= QIcon::fromTheme("edit-delete");
//	m_lpDeleteAction				= m_lpListToolBar->addAction(deleteIcon, tr("&Delete"), this, &cMainWindow::onDeleteEntrys);
//	m_lpDeleteAction->setShortcut(QKeySequence::Delete);

//	const QIcon	clearIcon			= QIcon::fromTheme("edit-clear");
//	m_lpClearAction					= m_lpListToolBar->addAction(clearIcon, tr("&Clear"), this, &cMainWindow::onClearList);


//	m_lpActionToolBar	= addToolBar("action");

//	const QIcon	convertIcon			= QIcon::fromTheme("system-run");
//	m_lpConvertAction				= m_lpActionToolBar->addAction(convertIcon, tr("&Convert"), this, &cMainWindow::onConvert);
//	m_lpConvertAction->setShortcut(QKeySequence::Delete);

//	const QIcon	stopIcon			= QIcon::fromTheme("process-stop");
//	m_lpStopAction					= m_lpActionToolBar->addAction(stopIcon, tr("&Stop"), this, &cMainWindow::onStop);
}

void cMainWindow::onGo()
{
	m_lpListModel->clear();

	QStringList	headerLabels	= QStringList() << tr("Date") << tr("Temperature") << tr("Pressure") << tr("Humidity") << tr("Windspeed") << tr("Winddirection");
	m_lpListModel->setHorizontalHeaderLabels(headerLabels);

	m_lpSeriesTemperature->clear();
	m_lpSeriesPressure->clear();
	m_lpSeriesHumidity->clear();

	cMeteoStat		meteoStat("Q1H5Feca");
	cMeteoDataList	list		= meteoStat.getHistoricalData(ui->m_lpDateFrom->date(), ui->m_lpDateTo->date(), 11035);

	if(!list.count())
		return;

	QDateTime		minDate			= list[0]->dateTime();
	QDateTime		maxDate			= list[0]->dateTime();
	qreal			minTemperature	= list[0]->temperature();
	qreal			maxTemperature	= list[0]->temperature();
	qreal			minPressure		= list[0]->pressure();
	qreal			maxPressure		= list[0]->pressure();
	qreal			minHumidity		= list[0]->humidity();
	qreal			maxHumidity		= list[0]->humidity();

	for(int x = 0;x < list.count();x++)
	{
		cMeteoData*				lpData		= list[x];
		QList<QStandardItem*>	itemList;

		itemList << new QStandardItem(lpData->dateTime().toString("yyyy-MM-dd hh:mm"))
				 << new QStandardItem(QString::number(lpData->temperature()) + " °C")
				 << new QStandardItem(QString::number(lpData->pressure()) + " hPa")
				 << new QStandardItem(QString::number(lpData->humidity()) + " %")
				 << new QStandardItem(QString::number(lpData->windspeed()) + " km/h")
				 << new QStandardItem(QString::number(lpData->winddirection()) + " °");

		itemList[1]->setTextAlignment(Qt::AlignRight);
		itemList[2]->setTextAlignment(Qt::AlignRight);
		itemList[3]->setTextAlignment(Qt::AlignRight);
		itemList[4]->setTextAlignment(Qt::AlignRight);

		itemList[0]->setData(QVariant::fromValue(lpData->dateTime()), Qt::UserRole);
		itemList[1]->setData(QVariant::fromValue(lpData->temperature()), Qt::UserRole);
		itemList[2]->setData(QVariant::fromValue(lpData->pressure()), Qt::UserRole);
		itemList[3]->setData(QVariant::fromValue(lpData->humidity()), Qt::UserRole);
		itemList[4]->setData(QVariant::fromValue(lpData->windspeed()), Qt::UserRole);
		itemList[5]->setData(QVariant::fromValue(lpData->winddirection()), Qt::UserRole);

		m_lpListModel->appendRow(itemList);

		m_lpSeriesTemperature->append(lpData->dateTime().toMSecsSinceEpoch(), lpData->temperature());
		m_lpSeriesPressure->append(lpData->dateTime().toMSecsSinceEpoch(), lpData->pressure());
		m_lpSeriesHumidity->append(lpData->dateTime().toMSecsSinceEpoch(), lpData->humidity());

		if(lpData->dateTime() < minDate)
			minDate	= lpData->dateTime();

		if(lpData->dateTime() > maxDate)
			maxDate	= lpData->dateTime();

		if(lpData->temperature() < minTemperature)
			minTemperature		= lpData->temperature();

		if(lpData->temperature() > maxTemperature)
			maxTemperature		= lpData->temperature();

		if(lpData->pressure() < minPressure)
			minPressure		= lpData->pressure();

		if(lpData->pressure() > maxPressure)
			maxPressure		= lpData->pressure();

		if(lpData->humidity() < minHumidity)
			minHumidity		= lpData->humidity();

		if(lpData->humidity() > maxHumidity)
			maxHumidity		= lpData->humidity();
	}

	qreal	diffTemperature	= maxTemperature-minTemperature;
	qreal	diffPRessure	= maxPressure-minPressure;
	qreal	diffHumidity	= maxHumidity-minHumidity;

	m_lpAxisX->setRange(minDate, maxDate);
	m_lpAxisYTemperature->setRange(minTemperature-diffTemperature/10, maxTemperature+diffTemperature/10);
	m_lpAxisYPressure->setRange(minPressure-diffPRessure/10, maxPressure+diffPRessure/10);
	m_lpAxisYHumidity->setRange(minHumidity-diffHumidity/10, maxHumidity+diffHumidity/10);

	ui->m_lpList->resizeColumnToContents(0);
	ui->m_lpList->resizeColumnToContents(1);
	ui->m_lpList->resizeColumnToContents(2);
	ui->m_lpList->resizeColumnToContents(3);
	ui->m_lpList->resizeColumnToContents(4);
	ui->m_lpList->resizeColumnToContents(5);
}

void cMainWindow::onCopy()
{
	QClipboard*		clipboard	= QGuiApplication::clipboard();
	QString			text		= tr("Date") + tr(SEPARATOR) + tr("Temperature") + tr(SEPARATOR) + tr("Pressure") + tr(SEPARATOR) + tr("Humidity") + tr(SEPARATOR) + tr("Windspeed") + tr(SEPARATOR) + tr("Winddirection\n");

	for(int x = 0;x < m_lpListModel->rowCount();x++)
	{
		QStandardItem*	lpDateTime		= m_lpListModel->item(x, 0);
		QStandardItem*	lpTemperature	= m_lpListModel->item(x, 1);
		QStandardItem*	lpPressure		= m_lpListModel->item(x, 2);
		QStandardItem*	lpHumidity		= m_lpListModel->item(x, 3);
		QStandardItem*	lpWindspeed		= m_lpListModel->item(x, 4);
		QStandardItem*	lpWinddirection	= m_lpListModel->item(x, 5);

		text.append(lpDateTime->data(Qt::UserRole).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));

		text.append(SEPARATOR);
		text.append(QString::number(lpTemperature->data(Qt::UserRole).toDouble()).replace(".", ","));

		text.append(SEPARATOR);
		text.append(QString::number(lpPressure->data(Qt::UserRole).toDouble()).replace(".", ","));

		text.append(SEPARATOR);
		text.append(QString::number(lpHumidity->data(Qt::UserRole).toInt()));

		text.append(SEPARATOR);
		text.append(QString::number(lpWindspeed->data(Qt::UserRole).toDouble()).replace(".", ","));

		text.append(SEPARATOR);
		text.append(QString::number(lpWinddirection->data(Qt::UserRole).toInt()));

		text.append("\n");
	}

	clipboard->setText(text);
}

void cMainWindow::onCopyRow()
{
	QModelIndex	index	= ui->m_lpList->currentIndex();
	if(!index.isValid())
		return;

	QClipboard*		clipboard	= QGuiApplication::clipboard();
	QString			text		= "";

//	QStandardItem*	lpDateTime		= m_lpListModel->item(index.row(), 0);
	QStandardItem*	lpTemperature	= m_lpListModel->item(index.row(), 1);
//	QStandardItem*	lpPressure		= m_lpListModel->item(index.row(), 2);
	QStandardItem*	lpHumidity		= m_lpListModel->item(index.row(), 3);
	QStandardItem*	lpWindspeed		= m_lpListModel->item(index.row(), 4);
//	QStandardItem*	lpWinddirection	= m_lpListModel->item(index.row(), 5);

//	text.append(lpDateTime->data(Qt::UserRole).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));

//	text.append(SEPARATOR);
	text.append(QString::number(lpTemperature->data(Qt::UserRole).toDouble()).replace(".", ","));

//	text.append(SEPARATOR);
//	text.append(QString::number(lpPressure->data(Qt::UserRole).toDouble()).replace(".", ","));

	text.append(SEPARATOR);
	text.append(QString::number(lpHumidity->data(Qt::UserRole).toInt()));

	text.append(SEPARATOR);
	text.append(QString::number(lpWindspeed->data(Qt::UserRole).toDouble()).replace(".", ","));

//	text.append(SEPARATOR);
//	text.append(QString::number(lpWinddirection->data(Qt::UserRole).toInt()));

	clipboard->setText(text);
}

void cMainWindow::onShowTemperature(int /*state*/)
{
	m_lpSeriesTemperature->setVisible(ui->m_lpShowTemperature->isChecked());
	m_lpAxisYTemperature->setVisible(ui->m_lpShowTemperature->isChecked());
}

void cMainWindow::onShowPressure(int /*state*/)
{
	m_lpSeriesPressure->setVisible(ui->m_lpShowPressure->isChecked());
	m_lpAxisYPressure->setVisible(ui->m_lpShowPressure->isChecked());
}

void cMainWindow::onShowHumidity(int /*state*/)
{
	m_lpSeriesHumidity->setVisible(ui->m_lpShowHumidity->isChecked());
	m_lpAxisYHumidity->setVisible(ui->m_lpShowHumidity->isChecked());
}
