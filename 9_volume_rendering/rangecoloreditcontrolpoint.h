#ifndef RANGECOLOREDITCONTROLPOINT_H
#define RANGECOLOREDITCONTROLPOINT_H

#include <QGraphicsItem>
#include "rangecolordefine.h"

class RangeColorEditControlPointConnection;

class RangeColorEditControlPoint : public QGraphicsItem
{
public:
    RangeColorEditControlPoint(QGraphicsScene* scene = nullptr, const QPointF& pos = QPointF(0.0,0.0), double size = 14.0);
    RangeColorEditControlPoint(QGraphicsScene* scene = nullptr, const RangeColorPoint& point = RangeColorPoint(), double size = 14.0);

    virtual ~RangeColorEditControlPoint();

    void updatePosition(const QPointF& pos);
    virtual const QPointF& getCurrentPos() const;
    virtual double getPosition() const;
    QColor getColor() const;
    //QColor getRgbaColor() const;
    void   setColor(const QColor& color);
    QVector<uchar> getUcharColor() const;
    const RangeColorPoint& getColorPoint() const{return colorData_;};

    void beginMouseDrag();
    void stopMouseDrag();

    void setHovered(bool hover);

    virtual QPointF prepareItemPositionChange(const QPointF& pos);

protected:
    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* options,
                           QWidget* widget) override;
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* e) override;


    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
private:
    void updateLabel();

private:
    double size_;
    bool isEditingPoint_;
    QPointF currentPos_;


    QPointF cachedPosition_;  //!< used for restricting to horizontal/vertical movement
    bool mouseDrag_ = false;

    bool hovered_ = false;

    int defaultZValue_ = 10;

    RangeColorPoint colorData_;

public:
    RangeColorEditControlPointConnection* left_ = nullptr;   // Non-owning reference
    RangeColorEditControlPointConnection* right_= nullptr;
    QGraphicsSimpleTextItem*              label_= nullptr;
};

bool operator==(const RangeColorEditControlPoint& lhs,
                                         const RangeColorEditControlPoint& rhs);
bool operator!=(const RangeColorEditControlPoint& lhs,
                                         const RangeColorEditControlPoint& rhs);
bool operator<(const RangeColorEditControlPoint& lhs, const RangeColorEditControlPoint& rhs);
bool operator>(const RangeColorEditControlPoint& lhs, const RangeColorEditControlPoint& rhs);
bool operator<=(const RangeColorEditControlPoint& lhs,
                                         const RangeColorEditControlPoint& rhs);
bool operator>=(const RangeColorEditControlPoint& lhs,
                                         const RangeColorEditControlPoint& rhs);

#endif // RANGECOLOREDITCONTROLPOINT_H
