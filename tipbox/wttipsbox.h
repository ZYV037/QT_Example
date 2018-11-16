#ifndef WTTIPSBOX_H
#define WTTIPSBOX_H

#include <QLabel>

class QTimeLine;

class TipBox: public QLabel
{
    Q_OBJECT
public:
    TipBox(QWidget* pParent);

    void ShowMessage(const QString& msg);
    void paintEvent(QPaintEvent *event);

private slots:
    void goTipBox(int value);

private:
    QTimeLine* m_pTimeLine;
    QString m_msg;
    QRect m_rect;
    QWidget* m_pParentWidget;
};

#endif // WTTIPSBOX_H
