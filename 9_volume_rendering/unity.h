#pragma once

#include <QFont>
#include <QWidget>

int emToPx(const QFontMetrics& m, double em);

int emToPx(const QWidget* w, double em);

QSize emToPx(const QWidget* w, QSizeF ems);
int refSpacePx(const QWidget* w);

QDateTime seperatorNumberToQDatetime(short year, short jday, short  tenth_ms, unsigned char hour, unsigned char minute, unsigned char second);

void qDatetimeToseperatorNumber(const QDateTime& dt, short& year, short& jday, short& tenth_ms, unsigned char& hour, unsigned char& minute, unsigned char& second);

QString componentColor(QChar component);


bool IsNumber(QString str);

bool IsInt(QString str);

FILE* Wt_fopen(const std::string& fileName_, const char * mode_);
