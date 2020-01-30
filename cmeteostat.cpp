#include "cmeteostat.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QDebug>


cMeteoStat::cMeteoStat(const QString& key) :
	m_key(key)
{
}

cMeteoDataList cMeteoStat::getHistoricalData(const QDate& from, const QDate& to, const qint32 &stationID)
{
	cMeteoDataList			meteoDataList;
	QNetworkAccessManager	networkManager;
	QString					szRequest	= QString("https://api.meteostat.net/v1/history/hourly?station=%1&start=%2&end=%3&time_zone=Europe/London&time_format=Y-m-d\%20H:i&key=%4").arg(stationID).arg(from.toString("yyyy-MM-dd")).arg(to.toString("yyyy-MM-dd")).arg(m_key);

	QNetworkRequest			request;
	request.setUrl(QUrl(szRequest));

	QNetworkReply*			reply   = networkManager.get(request);
	QEventLoop				loop;

	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	if(reply->error() == QNetworkReply::NoError)
	{
		QString			strReply		= QString(reply->readAll());
		QJsonDocument	jsonResponse	= QJsonDocument::fromJson(strReply.toUtf8());
		QJsonObject		jsonObj			= jsonResponse.object();
		QJsonArray		jsonArray		= jsonObj["data"].toArray();

		for(int z = 0;z < jsonArray.count();z++)
		{
			QJsonObject	entry			= jsonArray[z].toObject();

			cMeteoData*	lpMeteoData		= meteoDataList.add(entry["time"].toVariant().toDateTime());

			if(lpMeteoData)
			{
				lpMeteoData->setDateTimeLocal(entry["time_local"].toVariant().toDateTime());
				lpMeteoData->setTemperature(entry["temperature"].toDouble());
				lpMeteoData->setDewpoint(entry["dewpoint"].toDouble());
				lpMeteoData->setHumidity(entry["humidity"].toInt());
				lpMeteoData->setWindspeed(entry["windspeed"].toDouble());
				lpMeteoData->setPeakgust(entry["peakgust"].toDouble());
				lpMeteoData->setWinddirection(entry["winddirection"].toInt());
				lpMeteoData->setPressure(entry["pressure"].toDouble());
				lpMeteoData->setCondition(entry["condition"].toInt());
			}
		}
	}
	else
	{
		qDebug() << reply->errorString();
		delete reply;
	}

	delete reply;
	return(meteoDataList);
}
