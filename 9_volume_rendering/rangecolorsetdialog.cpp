#include "rangecolorsetdialog.h"
#include <QHBoxLayout>
#include "rangecoloreditview.h"
#include "rangecoloredit.h"
#include <QSplitter>
#include <QLayoutItem>
#include <QLabel>

RangeColorSetDialog::RangeColorSetDialog(QWidget* parent) : QColorDialog(parent)
{
    setOption(ColorDialogOption::ShowAlphaChannel, true);
    setOption(ColorDialogOption::NoButtons, true);

    edit = new RangeColorEdit(this);
    editView = new RangeColorEditView(edit);

    editView->setFocusPolicy(Qt::StrongFocus);
    editView->scale(1.0, -1.0);
    editView->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    editView->setMinimumSize(255, 100);
    editView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    editView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(edit, &RangeColorEdit::RangeColorChangeSignal, this, &RangeColorSetDialog::updateTFPreview);

    chkShowHistogram_ = new QComboBox(this);
    chkShowHistogram_->addItem("Histogram: Off");
    chkShowHistogram_->addItem("Histogram: 100%");
    chkShowHistogram_->addItem("Histogram: 99%");
    chkShowHistogram_->addItem("Histogram: 95%");
    chkShowHistogram_->addItem("Histogram: 90%");
    chkShowHistogram_->addItem("Histogram: Log");
    chkShowHistogram_->setCurrentIndex(static_cast<int>(editView->getHistogramMode()));
    connect(chkShowHistogram_, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            editView,&RangeColorEditView::showHistogram);

    //custom QColorDialog
    QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(layout());
    for(auto i = 0; i < mainLayout->count(); ++i)
    {
        QHBoxLayout* topLay =  dynamic_cast<QHBoxLayout*>( mainLayout->itemAt(i) );
        if(topLay)
        {
            QVBoxLayout* rightLayout = new QVBoxLayout;

            topLay->addLayout(rightLayout);
            tfPreview_ = new QLabel(this);
            tfPreview_->setMinimumSize(1, 20);
            QSizePolicy sliderPol = tfPreview_->sizePolicy();
            sliderPol.setHorizontalStretch(3);
            tfPreview_->setSizePolicy(sliderPol);

            rightLayout->addWidget(chkShowHistogram_);
            rightLayout->addWidget(editView);
            rightLayout->addWidget(tfPreview_);
        }
    }
}

QVector<uchar> RangeColorSetDialog::getColor(double y) const
{
    return edit->getColor(y);
}

void RangeColorSetDialog::addControlPoint(const RangeColorPoint& point)
{
    return edit->addControlPoint(point);
}

void RangeColorSetDialog::addControlPoints(const QList<RangeColorPoint>& points)
{
    foreach(auto point, points)
    {
        edit->addControlPoint(point);
    }
}

QList<RangeColorPoint> RangeColorSetDialog::getColorPoints()
{
    return edit->getColorPoints();
}

void RangeColorSetDialog::showEvent(QShowEvent *e)
{
    updateTFPreview();
    editView->update();
    QColorDialog::showEvent(e);
}

void RangeColorSetDialog::updateTFPreview()
{
    auto pixmap = edit->toQPixmap(QSize(tfPreview_->width(), 20));
    tfPreview_->setPixmap(pixmap);
}

QPixmap RangeColorSetDialog::toQPixmap(const QSize& size)
{
    return edit->toQPixmap(size);
}

