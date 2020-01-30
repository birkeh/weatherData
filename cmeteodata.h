#ifndef CMETEODATA_H
#define CMETEODATA_H


#include <QMetaType>
#include <QDateTime>

#include <QList>


class cMeteoData
{
public:
	cMeteoData(const QDateTime& dateTime);

	QDateTime			dateTime();

	void				setDateTimeLocal(const QDateTime& dateTime);
	QDateTime			dateTimeLocal();

	void				setTemperature(const qreal& temperature);
	qreal				temperature();

	void				setDewpoint(const qreal& dewpoint);
	qreal				dewpoint();

	void				setHumidity(const qint8& humidity);
	qint8				humidity();

	void				setWindspeed(const qreal& windspeed);
	qreal				windspeed();

	void				setPeakgust(const qreal& peakgust);
	qreal				peakgust();

	void				setWinddirection(const qint16& winddirection);
	qint16				winddirection();

	void				setPressure(const qreal& pressure);
	qreal				pressure();

	void				setCondition(const qint8& condition);
	qint8				condition();

signals:

private:
	QDateTime			m_dateTime;
	QDateTime			m_dateTimeLocal;
	qreal				m_temperature;
	qreal				m_dewpoint;
	qint8				m_humidity;
	qreal				m_windspeed;
	qreal				m_peakgust;
	qint16				m_winddirection;
	qreal				m_pressure;
	qint8				m_condition;
};

Q_DECLARE_METATYPE(cMeteoData*)

class cMeteoDataList : public QList<cMeteoData*>
{
public:
	cMeteoDataList();

	cMeteoData*			add(const QDateTime& dateTime);
	cMeteoData*			find(const QDateTime& dateTime);
};

#endif // CMETEODATA_H
