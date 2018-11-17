#include "wttipsbox.h"
#include <QTimeLine>
#include <QWidget>

TipBox::TipBox(QWidget* pParent):m_pParentWidget(pParent),m_bMouseIn(false)
{
    setMaximumHeight(0);
    setMargin(0);

    m_pTimeLine = new QTimeLine(10000, this);
    m_pTimeLine->setFrameRange(0, 1000);
    connect(m_pTimeLine, SIGNAL(frameChanged(int)), this, SLOT(goTipBox(int)));
}

void TipBox::ShowMessage(const QString& msg, TipType type)
{
    setText(msg);
    setTextColor(type);

    if( QTimeLine::NotRunning != m_pTimeLine->state() )
    {
        m_pTimeLine->stop();
    }


    m_pTimeLine->setDuration(10000);
    m_pTimeLine->setStartFrame(0);
    m_pTimeLine->start();
}


void TipBox::enterEvent(QEvent *event)
{
    m_bMouseIn = true;
}

void TipBox::leaveEvent(QEvent *event)
{
    m_pTimeLine->setDirection(QTimeLine::Direction::Forward);
    m_bMouseIn = false;
    if(m_pTimeLine->state() == QTimeLine::Paused )
    {
         m_pTimeLine->setPaused(false);
    }
}

void TipBox::goTipBox(int value)
{
    if(value==1000)
    {
        m_pTimeLine->stop();
        this->setMaximumHeight(0);
    }
    else if(value <= 300)
    {
        this->setMaximumHeight(value/2);
        m_pParentWidget->repaint();
    }
    else if(value >= 700 )
    {
        if(m_bMouseIn)
        {
            m_pTimeLine->setDirection(QTimeLine::Direction::Backward);
        }

        this->setMaximumHeight((1000 - value)/2);
        m_pParentWidget->repaint();
    }
    else if(m_bMouseIn && value == 500 )
    {
        m_pTimeLine->setDirection(QTimeLine::Direction::Forward);
        m_pTimeLine->setPaused(true);
    }

    m_pParentWidget->repaint();
    return;

}

void TipBox::setTextColor(TipType type)
{
    switch(type)
    {
        case Info:
            setStyleSheet("color:black; background-color:#00d8ff");
            break;
        case Warning:
            setStyleSheet("color:red; background-color:#00d8ff");
            break;
        case Error:
            setStyleSheet("color:red; background-color:#00d8ff");
            break;
        case Fatal:
            setStyleSheet("color:red; background-color:#00d8ff");
            break;
    }
}
