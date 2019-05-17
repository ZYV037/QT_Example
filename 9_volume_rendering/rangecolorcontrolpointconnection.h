#pragma once

#include <QGraphicsItem>

class RangeColorEditControlPoint;

class RangeColorEditControlPointConnection : public QGraphicsItem
{
public:
    RangeColorEditControlPointConnection();
    virtual ~RangeColorEditControlPointConnection();

    void updateShape();

    friend bool operator==(const RangeColorEditControlPointConnection& lhs,
                                                    const RangeColorEditControlPointConnection& rhs);

    // Compare points by their "x" value
    friend bool operator<(const RangeColorEditControlPointConnection& lhs,
                                                   const RangeColorEditControlPointConnection& rhs);

    RangeColorEditControlPoint* left_;   // Non-owning reference
    RangeColorEditControlPoint* right_;  // Non-owning reference

    QPointF getStart() const;
    QPointF getStop() const;

protected:
    // Overload
    void paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget);
    QRectF boundingRect() const;
    QPainterPath shape() const;

private:
    QPainterPath path_;
    QPainterPath shape_;
    QRectF rect_;
};

bool operator==(const RangeColorEditControlPointConnection& lhs,
                                         const RangeColorEditControlPointConnection& rhs);
bool operator!=(const RangeColorEditControlPointConnection& lhs,
                                         const RangeColorEditControlPointConnection& rhs);
bool operator<(const RangeColorEditControlPointConnection& lhs,
                                        const RangeColorEditControlPointConnection& rhs);
bool operator>(const RangeColorEditControlPointConnection& lhs,
                                        const RangeColorEditControlPointConnection& rhs);
bool operator<=(const RangeColorEditControlPointConnection& lhs,
                                         const RangeColorEditControlPointConnection& rhs);
bool operator>=(const RangeColorEditControlPointConnection& lhs,
                                         const RangeColorEditControlPointConnection& rhs);
