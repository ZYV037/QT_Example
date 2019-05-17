#include "rangecoloreditview.h"
#include "volumeramhistogram.h"
#include "rangecolordefine.h"
#include "trtparser.h"

HistogramContainer container;

RangeColorEditView::RangeColorEditView(QGraphicsScene* scene, QWidget* parent)
    : QGraphicsView(scene, parent)
{
    setMouseTracking(true);
    setRenderHint(QPainter::Antialiasing, true);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

    this->setCacheMode(QGraphicsView::CacheBackground);
}

const HistogramContainer* RangeColorEditView::getNormalizedHistograms()
{
    float* data = nullptr;
    int xLength, yLength, zLength;
    float min, max;
    getTrtData(&data, xLength, yLength, zLength, min, max);

    container = calculateVolumeHistogram( data, size3_t(xLength,yLength,zLength), doubleVec2(min, max), false, 2048, size3_t(1,1,1) );
    return &container;
}

void RangeColorEditView::updateHistogram()
{
    histograms_.clear();
    QRectF sRect = sceneRect();

    const HistogramContainer* histCont = getNormalizedHistograms();

    if (!histCont) return;

    for (size_t channel = 0; channel < histCont->size(); ++channel) {
        histograms_.push_back(QPolygonF());
        const std::vector<double>* normHistogramData = (*histCont)[channel].getData();
        double histSize = static_cast<double>(normHistogramData->size());
        double stepSize = sRect.width() / histSize;

        double scale = 1.0;
        switch (histogramMode_) {
            case HistogramMode::Off:  // Don't show
                return;
            case HistogramMode::All:  // show all
                scale = 1.0;
                break;
            case HistogramMode::P99:  // show 99%
                scale = (*histCont)[channel].histStats_.percentiles[99];
                break;
            case HistogramMode::P95:  // show 95%
                scale = (*histCont)[channel].histStats_.percentiles[95];
                break;
            case HistogramMode::P90:  // show 90%
                scale = (*histCont)[channel].histStats_.percentiles[90];
                break;
            case HistogramMode::Log:  // show log%
                scale = 1.0;
                break;
        }
        double height;
        double maxCount = (*histCont)[channel].getMaximumBinValue();

        histograms_.back() << QPointF(0.0, 0.0);

        for (double i = 0; i < histSize; i++) {
            if (histogramMode_ == HistogramMode::Log) {
                height =
                    std::log10(1.0 + maxCount * normHistogramData->at(static_cast<size_t>(i))) /
                    std::log10(maxCount);

            } else {
                height = normHistogramData->at(static_cast<size_t>(i)) / scale;
                height = std::min(height, 1.0);
            }
            height *= sRect.height();
            histograms_.back() << QPointF(i * stepSize, height)
                               << QPointF((i + 1) * stepSize, height);
        }
        histograms_.back() << QPointF(sRect.width(), 0.0f) << QPointF(0.0f, 0.0f);
    }
}

void RangeColorEditView::showHistogram(int type)
{
    histogramMode_ = static_cast<HistogramMode>(type);
    updateHistogram();
    resetCachedContent();
    update();
}

void RangeColorEditView::showEvent(QShowEvent *event)
{
    updateHistogram();
    resetCachedContent();
    QGraphicsView::showEvent(event);
}

void RangeColorEditView::resizeEvent(QResizeEvent* event) {
    QGraphicsView::resizeEvent(event);
    resetCachedContent();
    //updateZoom();
}

void RangeColorEditView::drawForeground(QPainter* painter, const QRectF& rect) {
    QPen pen;
    pen.setCosmetic(true);
    pen.setWidthF(1.5);
    pen.setColor(Qt::lightGray);
    painter->setPen(pen);

    QRectF sRect = sceneRect();

//    if (maskHorizontal_.x > 0.0) {
//        double leftMaskBorder = maskHorizontal_.x * sRect.width();
//        QRectF r(0.0, rect.top(), leftMaskBorder, rect.height());
//        QLineF line(leftMaskBorder, rect.top(), leftMaskBorder, rect.bottom());
//        painter->fillRect(r, QColor(25, 25, 25, 100));
//        painter->drawLine(line);
//    }

//    if (maskHorizontal_.y < 1.0) {
//        double rightMaskBorder = maskHorizontal_.y * sRect.width();
//        QRectF r(rightMaskBorder, rect.top(), sRect.right() - rightMaskBorder, rect.height());
//        QLineF line(rightMaskBorder, rect.top(), rightMaskBorder, rect.bottom());
//        painter->fillRect(r, QColor(25, 25, 25, 100));
//        painter->drawLine(line);
//    }

    QGraphicsView::drawForeground(painter, rect);
}

void RangeColorEditView::drawBackground(QPainter* painter, const QRectF& rect) {
    painter->fillRect(rect, QColor(89, 89, 89));

    // overlay grid
    const QColor colorGrid(102, 102, 102);
    const QColor colorOrigin(102, 106, 115);
    const int gridSpacing = 50;

    QRectF sRect = sceneRect();
    QPen gridPen;
    gridPen.setCosmetic(true);

    double gridOrigin = sRect.left();  // horizontal origin of the grid
    // adjust grid origin if there is a data mapper available
//    if (volumeInport_ && volumeInport_->hasData()) {
//        auto& datamap = volumeInport_->getData()->dataMap_;
//        if ((datamap.valueRange.x < 0.0) && (datamap.valueRange.y > 0.0)) {
//            gridOrigin = datamap.mapFromValueToNormalized(0.0) * sRect.width() + sRect.left();

//            // draw line at zero
//            gridPen.setWidthF(3.0f);
//            gridPen.setColor(colorOrigin);
//            painter->setPen(gridPen);
//            painter->drawLine(
//                QLineF(QPointF(gridOrigin, sRect.bottom()), QPointF(gridOrigin, sRect.top())));
//        }
//    }

    QVector<QLineF> lines;

    // add grid lines left of origin
    double x = gridOrigin - gridSpacing;
    while (x > sRect.left()) {
        lines.push_back(QLineF(x, sRect.bottom(), x, sRect.top()));
        x -= gridSpacing;
    }
    // add grid lines right of origin
    x = gridOrigin + gridSpacing;
    while (x < sRect.right()) {
        lines.push_back(QLineF(x, sRect.bottom(), x, sRect.top()));
        x += gridSpacing;
    }

    // draw grid
    gridPen.setColor(colorGrid);
    gridPen.setWidthF(1.0);
    painter->setPen(gridPen);
    painter->drawLines(lines);

    //histogram
    if (histogramMode_ != HistogramMode::Off)
    {
        for (auto& elem : histograms_)
        {
            QPen pen;
            pen.setColor(QColor(68, 102, 170, 150));
            pen.setWidthF(2.0f);
            pen.setCosmetic(true);
            painter->setPen(pen);

            QBrush brush;
            brush.setColor(QColor(68, 102, 170, 100));
            brush.setStyle(Qt::SolidPattern);
            painter->setBrush(brush);

            painter->drawPolygon(elem);
        }
    }
}
