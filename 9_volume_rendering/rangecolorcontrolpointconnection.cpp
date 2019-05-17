#include <QGraphicsScene>
#include <QPainter>
#include "rangecolorcontrolpointconnection.h"
#include "rangecoloreditcontrolpoint.h"

RangeColorEditControlPointConnection::RangeColorEditControlPointConnection()
    : QGraphicsItem(), left_(nullptr), right_(nullptr), path_(), shape_(), rect_() {
    setZValue(8.0);
    updateShape();
}

RangeColorEditControlPointConnection::~RangeColorEditControlPointConnection() {}

void RangeColorEditControlPointConnection::paint(QPainter* p, const QStyleOptionGraphicsItem* options,
                                     QWidget* widget) {
    p->save();
    p->setRenderHint(QPainter::Antialiasing, true);

    QPen pathPen(QColor(66, 66, 66));
    pathPen.setWidth(3);
    pathPen.setCosmetic(true);

    p->setPen(pathPen);
    p->drawPath(path_);
    p->restore();
}

void RangeColorEditControlPointConnection::updateShape() {
    if (left_ == nullptr && right_ == nullptr) {
        path_ = QPainterPath();
    }

    path_ = QPainterPath(getStart());
    path_.lineTo(getStop());

    rect_ = path_.boundingRect();

    QPainterPathStroker pathStrocker;
    pathStrocker.setWidth(10.0);
    shape_ = pathStrocker.createStroke(path_);

    prepareGeometryChange();
    update();
}

QPointF RangeColorEditControlPointConnection::getStart() const {
    QPointF start;
    if (left_) {
        start = left_->getCurrentPos();
    }
    else if (right_ && scene()) {
        start = QPointF(scene()->sceneRect().left(), right_->getCurrentPos().y());
    }
    return start;
}

QPointF RangeColorEditControlPointConnection::getStop() const {
    QPointF stop;
    if (right_) {
        stop = right_->getCurrentPos();
    } else if (left_ && scene()) {
        stop = QPointF(scene()->sceneRect().right(), left_->getCurrentPos().y());
    }
    return stop;
}

QRectF RangeColorEditControlPointConnection::boundingRect() const { return rect_; }

QPainterPath RangeColorEditControlPointConnection::shape() const { return shape_; }

bool operator==(const RangeColorEditControlPointConnection& lhs, const RangeColorEditControlPointConnection& rhs) {
    return lhs.getStart() == rhs.getStart() && lhs.getStop() == rhs.getStop();
}

bool operator!=(const RangeColorEditControlPointConnection& lhs, const RangeColorEditControlPointConnection& rhs) {
    return !operator==(lhs, rhs);
}

bool operator<(const RangeColorEditControlPointConnection& lhs, const RangeColorEditControlPointConnection& rhs) {
    return 0.5f * (lhs.getStart().x() + lhs.getStop().x()) <
           0.5f * (rhs.getStart().x() + rhs.getStop().x());
}

bool operator>(const RangeColorEditControlPointConnection& lhs, const RangeColorEditControlPointConnection& rhs) {
    return rhs < lhs;
}

bool operator<=(const RangeColorEditControlPointConnection& lhs, const RangeColorEditControlPointConnection& rhs) {
    return !(rhs < lhs);
}

bool operator>=(const RangeColorEditControlPointConnection& lhs, const RangeColorEditControlPointConnection& rhs) {
    return !(lhs < rhs);
}
