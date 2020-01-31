#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H


#include "csplashscreen.h"
#include "common.h"

#include <QMainWindow>

#include <QStandardItemModel>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>


QT_BEGIN_NAMESPACE
namespace Ui { class cMainWindow; }
QT_END_NAMESPACE


QT_CHARTS_USE_NAMESPACE


class cMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	cMainWindow(cSplashScreen* lpSplashScreen, QWidget *parent = nullptr);
	~cMainWindow();

private:
	Ui::cMainWindow*				ui;
	cSplashScreen*					m_lpSplashScreen;

	QStandardItemModel*				m_lpListModel;

	QChart*							m_lpChart;
	QLineSeries*					m_lpSeriesTemperature;
	QLineSeries*					m_lpSeriesPressure;
	QLineSeries*					m_lpSeriesHumidity;
	QDateTimeAxis*					m_lpAxisX;
	QValueAxis*						m_lpAxisYTemperature;
	QValueAxis*						m_lpAxisYPressure;
	QValueAxis*						m_lpAxisYHumidity;

	void							initUI();
	void							createActions();
	void							createFileActions();
	void							createContextActions();

protected:
	void							closeEvent(QCloseEvent* event);

private slots:
	void							onGo();
	void							onCopy();
	void							onShowTemperature(int state);
	void							onShowPressure(int state);
	void							onShowHumidity(int state);
};
#endif // CMAINWINDOW_H
