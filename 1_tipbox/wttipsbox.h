#ifndef WTTIPSBOX_H
#define WTTIPSBOX_H

#include <QLabel>

class QTimeLine;

class TipBox: public QLabel
{
public:
    enum TipType
    {
        Info,
        Warning,
        Error,
        Fatal
    };

    Q_OBJECT

    TipBox(QWidget* pParent);

    void ShowMessage(const QString& msg, TipType type = Info);
    void setTextColor(TipType type);
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    
private slots:
    void goTipBox(int value);
    
private:
    QTimeLine* m_pTimeLine;
    QWidget* m_pParentWidget;
    bool m_bMouseIn;
};

#endif // WTTIPSBOX_H
