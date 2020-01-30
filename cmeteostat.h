#ifndef CMETEOSTAT_H
#define CMETEOSTAT_H


#include "cmeteodata.h"

#include <QDate>


class cMeteoStat
{
public:
	cMeteoStat(const QString& key);

	cMeteoDataList		getHistoricalData(const QDate& from, const QDate& to, const qint32& stationID);

private:
	QString				m_key;
};

#endif // CMETEOSTAT_H
