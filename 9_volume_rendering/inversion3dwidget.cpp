#include "inversion3dwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtGui/QScreen>
#include <QMessageBox>
#include "volumetric.h"
#include "inversion3drightdockwidget.h"
#include <QMainWindow>

Inversion3dWidget::Inversion3dWidget(QWidget* parent) : QWidget(parent)
{
    InitWidget();
}

bool Inversion3dWidget::saveColorPoints()
{
    return rightDockWidget_->saveColorPoints();
}

QDockWidget* Inversion3dWidget::getInversion3DRightDockWidget()
{
    return rightDockWidget_;
}

void Inversion3dWidget::setRightDockWidgetHide(bool hide)
{
    if(hide)
    {
        rightDockWidget_->close();
    }
    else
    {
        rightDockWidget_->show();
    }
}

void Inversion3dWidget::InitWidget()
{
    Q3DScatter *graph = new Q3DScatter();
    QWidget *container = QWidget::createWindowContainer(graph);

    if (!graph->hasContext())
    {
        QMessageBox msgBox;
        msgBox.setText("Couldn't initialize the OpenGL context.");
        msgBox.exec();
        return;
    }

    QSize screenSize = graph->screen()->size();
    container->setMinimumSize(QSize(screenSize.width() / 5, screenSize.height() / 3));
    container->setMaximumSize(screenSize);
    //container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);

    modifier_ = new VolumetricModifier(graph);

    QHBoxLayout *hLayout = new QHBoxLayout(this);

    hLayout->addWidget(container, 1);

    QMainWindow* mainWindow  = dynamic_cast<QMainWindow*>( parent() );
    if(mainWindow)
    {
        rightDockWidget_ = new Inversion3dRightDockWidget( this, modifier_);
        mainWindow->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, rightDockWidget_);
    }
}


