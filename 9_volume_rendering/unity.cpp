#include "unity.h"
#include <tgmath.h>
#include <QDateTime>
#include <codecvt>
#include <locale>

template<class Facet>
struct deletable_facet : Facet
{
    template<class ...Args>
    deletable_facet(Args&& ...args) : Facet(std::forward<Args>(args)...) {}
    ~deletable_facet() {}
};

int emToPx(const QFontMetrics& m, double em) {
    const auto pxPerEm = m.boundingRect(QString(100, 'M')).width() / 100.0;
    return static_cast<int>(std::round(pxPerEm * em));
}

int emToPx(const QWidget* w, double em) {
    w->ensurePolished();
    return emToPx(w->fontMetrics(), em);
}

QSize emToPx(const QWidget* w, QSizeF ems) {
    return QSize{emToPx(w, ems.width()), emToPx(w, ems.height())};
}

constexpr int refEm() { return 11; }
constexpr double refSpaceEm() { return 7.0 / refEm(); }

int refSpacePx(const QWidget* w)
{
    return emToPx(w, refSpaceEm());
}

QDateTime seperatorNumberToQDatetime(short year, short jday, short tenth_ms, unsigned char hour, unsigned char minute, unsigned char second)
{
    QDateTime dttime;
    QDate dt(year, 1, 1);
    dt = dt.addDays(jday-1);

    QTime tm(hour, minute, second, tenth_ms);
    dttime.setDate(dt);
    dttime.setTime(tm);

    return dttime;
}

void qDatetimeToseperatorNumber(const QDateTime& dt, short& year, short& jday, short& tenth_ms, unsigned char& hour, unsigned char& minute, unsigned char& second)
{
    year = static_cast<short>(dt.date().year());
    jday = static_cast<short>(dt.date().dayOfYear());
    hour = static_cast<unsigned char>(dt.time().hour());
    minute = static_cast<unsigned char>(dt.time().minute());
    second = static_cast<unsigned char>(dt.time().second());
    tenth_ms = static_cast<unsigned char>(dt.time().msec());
}

QString componentColor(QChar component)
{
    if(component== 'X')
    {
        return "red";
    }
    if(component== 'Y')
    {
        return "green";
    }
    if(component== 'Z')
    {
        return "blue";
    }

    return "";
}

bool IsNumber(QString str)
{
    if(str.isEmpty())
    {
        return false;
    }
    int point_num = 0;
    for(int i=0;i<str.length();i++)
    {
        QChar cell_char = str.at(i);
        if((!cell_char.isDigit())&&(cell_char!='.'))
        {
            return false;
        }else if(cell_char=='.')
        {
            if(i==0||i==str.length()-1)
            {
                return false;
            }
            point_num ++;
        }
    }
    if(point_num>1)
    {
        return false;
    }
    return true;
}

bool IsInt(QString str)
{
    if(str.isEmpty())
    {
        return false;
    }
    for(int i=0;i<str.length();i++)
    {
        QChar cell_character = str.at(i);
        if(!cell_character.isDigit())
        {
            return false;
        }
    }
    return true;
}

FILE* Wt_fopen(const std::string& fileName_, const char * mode_)
{
#ifdef _WIN32
    std::wstring_convert< deletable_facet<std::codecvt_utf8<wchar_t, 0x10ffff, std::codecvt_mode::little_endian>>, wchar_t> convCharToWchar;
    std::wstring w_strFile = convCharToWchar.from_bytes( fileName_.data() );
    std::wstring w_mode = convCharToWchar.from_bytes( mode_ );
    return _wfopen( w_strFile.data(), w_mode.data() );
#else
    return fopen( fileName_.data, mode );
#endif
}
