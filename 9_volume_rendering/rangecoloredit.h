#pragma once

#include <QGraphicsScene>
#include "rangecolordefine.h"

class RangeColorEditControlPoint;
class RangeColorEditControlPointConnection;
class RangeColorSetDialog;

class RangeColorEdit : public QGraphicsScene
{
    Q_OBJECT
public:
    RangeColorEdit(RangeColorSetDialog* parent);
    ~RangeColorEdit();

    RangeColorEditControlPoint* getTFPrimitiveItemAt(const QPointF& pos) const;
    std::vector<RangeColorEditControlPoint*> getSelectedPrimitiveItems() const;

    QVector<uchar> getColor(double y) const;
    void updateConnections();
    void addControlPoint(const QPointF& pos);
    void addControlPoint(const RangeColorPoint& point);
    void deleteControlPoint(RangeColorEditControlPoint* point);
    QList<RangeColorPoint> getColorPoints();

    QPixmap toQPixmap(const QSize& size);

public slots:
    void onCurrentPointChanged();
    void onCurrentColorChanged();

signals:
    void RangeColorChangeSignal();

protected:
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* e) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* e) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* e) override;

private:

    std::vector<QPolygonF> histograms_;

    bool stopHistCalculation_ = false;

    QList<RangeColorEditControlPoint*> points_;
    QList<RangeColorEditControlPointConnection*> connections_;

    bool selectNewPrimitives_;
    bool mouseMovedSincePress_ = false;
    bool mouseDrag_ = false;
    bool mouseDoubleClick_ = false;

    RangeColorSetDialog* colorDialog_   = nullptr;
};
