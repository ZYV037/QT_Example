#include "wttipsbox.h"
#include <QPainter>
#include <QTimeLine>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>

TipBox::TipBox(QWidget* pParent):m_pParentWidget(pParent)
{
    setMaximumHeight(0);
    setMargin(0);
    setStyleSheet("background-color:#00d8ff");

     m_pTimeLine = new QTimeLine(10000, this);
     connect(m_pTimeLine, SIGNAL(frameChanged(int)), this, SLOT(goTipBox(int)));
}

void TipBox::ShowMessage(const QString& msg)
{
    setText(msg);
    if( QTimeLine::Running == m_pTimeLine->state() )
    {
        m_pTimeLine->stop();
    }

    m_pTimeLine->setDuration(10000);
    m_pTimeLine->setFrameRange(0, 1000);
    m_pTimeLine->start();
}

void TipBox::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
}

void TipBox::goTipBox(int value)
{
    if(value<=300)
    {
        this->setMaximumHeight(value/2);
        m_pParentWidget->repaint();
        return;
    }
    else if(value > 700)
    {
        this->setMaximumHeight((1000 - value)/2);
        m_pParentWidget->repaint();
    }

    if(value==1000)
    {
        m_pTimeLine->stop();
        this->setMaximumHeight(0);
        m_pParentWidget->repaint();
    }
}
