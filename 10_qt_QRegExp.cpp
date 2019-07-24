#include <QCoreApplication>
#include <QRegExp>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString ip = "1(127.0.0.1)";

    QRegExp reg( "^([0-9a-z]+)\\((([0-9a-z]+.){3,5}[0-9a-z]+)\\)$");

    QString company, web, country;

    if ( reg.indexIn(ip) != -1) {
        company = reg.cap(0);
        web = reg.cap(1);
        country = reg.cap(2);
    }

    return a.exec();
}
