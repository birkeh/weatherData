#include "cmeteodata.h"

#include <QDebug>

cMeteoData::cMeteoData(const QDateTime& dateTime) :
	m_dateTime(dateTime),
	m_dateTimeLocal(QDateTime()),
	m_temperature(0.0),
	m_dewpoint(0.0),
	m_humidity(0),
	m_windspeed(0.0),
	m_peakgust(0.0),
	m_winddirection(0),
	m_pressure(0.0),
	m_condition(0)
{
}

QDateTime cMeteoData::dateTime()
{
	return(m_dateTime);
}

void cMeteoData::setDateTimeLocal(const QDateTime& dateTime)
{
	m_dateTimeLocal	= dateTime;
}

QDateTime cMeteoData::dateTimeLocal()
{
	return(m_dateTimeLocal);
}

void cMeteoData::setTemperature(const qreal& temperature)
{
	m_temperature	= temperature;
}

qreal cMeteoData::temperature()
{
	return(m_temperature);
}

void cMeteoData::setDewpoint(const qreal& dewpoint)
{
	m_dewpoint	= dewpoint;
}

qreal cMeteoData::dewpoint()
{
	return(m_dewpoint);
}

void cMeteoData::setHumidity(const qint8& humidity)
{
	m_humidity	= humidity;
}

qint8 cMeteoData::humidity()
{
	return(m_humidity);
}

void cMeteoData::setWindspeed(const qreal& windspeed)
{
	m_windspeed	= windspeed;
}

qreal cMeteoData::windspeed()
{
	return(m_windspeed);
}

void cMeteoData::setPeakgust(const qreal& peakgust)
{
	m_peakgust	= peakgust;
}

qreal cMeteoData::peakgust()
{
	return(m_peakgust);
}

void cMeteoData::setWinddirection(const qint16& winddirection)
{
	m_winddirection	= winddirection;
}

qint16 cMeteoData::winddirection()
{
	return(m_winddirection);
}

void cMeteoData::setPressure(const qreal& pressure)
{
	m_pressure	= pressure;
}

qreal cMeteoData::pressure()
{
	return(m_pressure);
}

void cMeteoData::setCondition(const qint8 &condition)
{
	m_condition	= condition;
}

qint8 cMeteoData::condition()
{
	return(m_condition);
}

cMeteoDataList::cMeteoDataList()
{
}

cMeteoData* cMeteoDataList::add(const QDateTime& dateTime)
{
	cMeteoData*	lpMeteoData	= find(dateTime);
	if(lpMeteoData)
		return(nullptr);

	lpMeteoData	= new cMeteoData(dateTime);
	append(lpMeteoData);
	return(lpMeteoData);
}

cMeteoData* cMeteoDataList::find(const QDateTime& dateTime)
{
	for(int x = 0;x < count();x++)
	{
		cMeteoData*	lpMeteoData	= at(x);
		if(lpMeteoData->dateTime() == dateTime)
			return(lpMeteoData);
	}
	return(nullptr);
}
