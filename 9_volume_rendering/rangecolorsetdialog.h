#pragma once

#include <QColorDialog>
#include <QComboBox>
#include "rangecolordefine.h"

class RangeColorEditView;
class RangeColorEdit;
class QSplitter;
class QColorDialog;
class QLabel;

class RangeColorSetDialog : public QColorDialog
{
public:
    RangeColorSetDialog(QWidget* parent = nullptr);

    QVector<uchar> getColor(double y) const;

    void addControlPoint(const RangeColorPoint& point);
    void addControlPoints(const QList<RangeColorPoint>& points);
    QList<RangeColorPoint> getColorPoints();

    QPixmap toQPixmap(const QSize& size);

protected:
    void showEvent(QShowEvent *e) override;

public slots:
    void updateTFPreview();

private:
    RangeColorEditView* editView;
    RangeColorEdit*     edit;
    QLabel*             tfPreview_ = nullptr;
    QSplitter*          zoomVSlider_ = nullptr;
    QSplitter*          zoomHSlider_ = nullptr;
    QComboBox*          chkShowHistogram_ = nullptr;
};
