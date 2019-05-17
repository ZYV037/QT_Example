#pragma once

#include <QGraphicsView>
#include <QGraphicsScene>
#include "rangecolordefine.h"

class HistogramContainer;

class RangeColorEditView : public QGraphicsView
{
    Q_OBJECT
public:
    RangeColorEditView( QGraphicsScene* scene = nullptr, QWidget* parent = nullptr);

    const HistogramContainer* getNormalizedHistograms();
    void updateHistogram();
    HistogramMode getHistogramMode() { return histogramMode_; }
    void showHistogram(int type);
protected:
    void showEvent(QShowEvent *event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void drawForeground(QPainter* painter, const QRectF& rect) override;
    virtual void drawBackground(QPainter* painter, const QRectF& rect) override;


private:
    std::vector<QPolygonF> histograms_;
    HistogramMode histogramMode_ = HistogramMode::P99;
};

