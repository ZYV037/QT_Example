#include "rangecoloredit.h"
#include <QGraphicsSceneMouseEvent>
#include "rangecoloreditcontrolpoint.h"
#include "rangecolorcontrolpointconnection.h"
#include <QGuiApplication>
#include <QGraphicsView>
#include "rangecolorsetdialog.h"
#include <cmath>

class  KeepTrueWhileInScope {
public:
    KeepTrueWhileInScope(bool* b) : variable_(b), prevValue_(b ? *b : false) {
        if (variable_) {
            (*variable_) = true;
        }
    }
    ~KeepTrueWhileInScope() {
        if (variable_) (*variable_) = prevValue_;
    }

private:
    bool* variable_;
    bool prevValue_;
};

struct comparePtr {
    template <typename T>
    bool operator()(const T& a, const T& b) {
        //static_assert(util::is_dereferenceable<T>::value, "T has to be dereferenceable");
        return (*a < *b);
    }
};

template<typename genType>
 genType clamp(genType x, genType minVal, genType maxVal)
{
    //GLM_STATIC_ASSERT(std::numeric_limits<genType>::is_iec559 || std::numeric_limits<genType>::is_integer, "'clamp' only accept floating-point or integer inputs");
    return std::min(std::max(x, minVal), maxVal);
}

RangeColorEdit::RangeColorEdit(RangeColorSetDialog* parent):QGraphicsScene(parent)
{
    colorDialog_ = parent;

    setSceneRect(0.0, 0.0, 512.0, 512.0);
    setItemIndexMethod(QGraphicsScene::NoIndex);

    connect(this, &RangeColorEdit::selectionChanged, this, &RangeColorEdit::onCurrentPointChanged);
    connect(this, &RangeColorEdit::changed, this, &RangeColorEdit::onCurrentPointChanged);
    connect(colorDialog_, &RangeColorSetDialog::currentColorChanged, this, &RangeColorEdit::onCurrentColorChanged);
}


RangeColorEdit::~RangeColorEdit()
{
    disconnect(this, &RangeColorEdit::selectionChanged, this, &RangeColorEdit::onCurrentPointChanged);
    disconnect(this, &RangeColorEdit::changed, this, &RangeColorEdit::onCurrentPointChanged);
    disconnect(colorDialog_, &RangeColorSetDialog::currentColorChanged, this, &RangeColorEdit::onCurrentColorChanged);
}

RangeColorEditControlPoint* RangeColorEdit::getTFPrimitiveItemAt(const QPointF& pos) const
{
    QList<QGraphicsItem*> graphicsItems = items(pos);

    for (auto& graphicsItem : graphicsItems)
    {
        if (auto item = qgraphicsitem_cast<RangeColorEditControlPoint*>(graphicsItem))
        {
            return item;
        }
    }

    return nullptr;
}

inline int mix(unsigned char left, unsigned char right, double percent)
{
    return left + (right - left)*percent;
}

QVector<uchar> RangeColorEdit::getColor(double y) const //0.0-1.0
{
    static QVector<uchar> colorVec(4);

    if (points_.isEmpty()) return colorVec;

    auto it = std::upper_bound(points_.begin(), points_.end(), y, [](double val, const auto& p) { return val < p->getPosition(); });

    if (it == points_.begin())
    {
        return points_.front()->getUcharColor();
    }
    else if (it == points_.end())
    {
        return points_.back()->getUcharColor();
    }

    auto next = it--;
    const auto x = static_cast<double>((y - (*it)->getPosition() ) / ((*next)->getPosition() - (*it)->getPosition()));

    const QColor& colorLeft = (*it)->getColor();
    const QColor& colorRight = (*next)->getColor();

    colorVec[0] = mix( (*it)->getColor().blue(), (*next)->getColor().blue(), x );
    colorVec[1] = mix( (*it)->getColor().green(), (*next)->getColor().green(), x );
    colorVec[2] = mix( (*it)->getColor().red(), (*next)->getColor().red(), x );
    colorVec[3] = mix( (*it)->getColor().alpha(), (*next)->getColor().alpha(), x );
    return colorVec;
}

std::vector<RangeColorEditControlPoint*> RangeColorEdit::getSelectedPrimitiveItems() const
{
    std::vector<RangeColorEditControlPoint*> selection;
    for (auto& elem : selectedItems())
    {
        if (auto p = qgraphicsitem_cast<RangeColorEditControlPoint*>(elem))
        {
            selection.push_back(p);
        }
    }

    return selection;
}

void RangeColorEdit::addControlPoint(const QPointF& pos)
{
    RangeColorEditControlPoint* newpoint = new RangeColorEditControlPoint(this, pos, 14);
    if (selectNewPrimitives_)
    {
        newpoint->setSelected(true);
    }

    auto it = std::upper_bound(points_.begin(), points_.end(), newpoint, comparePtr{});
    it = points_.insert(it, newpoint);
    updateConnections();
    emit RangeColorChangeSignal();
}

void RangeColorEdit::addControlPoint(const RangeColorPoint& point)
{
    RangeColorEditControlPoint* newpoint = new RangeColorEditControlPoint(this, point, 14);
    if (selectNewPrimitives_)
    {
        newpoint->setSelected(true);
    }

    auto it = std::upper_bound(points_.begin(), points_.end(), newpoint, comparePtr{});
    it = points_.insert(it, newpoint);
    updateConnections();
    emit RangeColorChangeSignal();
}

void RangeColorEdit::deleteControlPoint(RangeColorEditControlPoint* pointDel)
{
    foreach(auto point, points_)
    {
        if(pointDel == point)
        {
            points_.removeOne(pointDel);
            removeItem(pointDel);
            updateConnections();
            delete pointDel;
            emit RangeColorChangeSignal();
            return;
        }
    }
}

QList<RangeColorPoint> RangeColorEdit::getColorPoints()
{
    QList<RangeColorPoint> colorPoints;
    foreach(auto point, points_)
    {
        colorPoints.push_back( point->getColorPoint() );
    }

    return colorPoints;
}

QPixmap RangeColorEdit::toQPixmap(const QSize& size)
{
    QPixmap tfPixmap(size);
    QPainter painter(&tfPixmap);

    QPixmap checkerBoard(10, 10);
    QPainter checkerBoardPainter(&checkerBoard);
    checkerBoardPainter.fillRect(0, 0, 5, 5, Qt::lightGray);
    checkerBoardPainter.fillRect(5, 0, 5, 5, Qt::darkGray);
    checkerBoardPainter.fillRect(0, 5, 5, 5, Qt::darkGray);
    checkerBoardPainter.fillRect(5, 5, 5, 5, Qt::lightGray);
    checkerBoardPainter.end();

    QRect r(QPoint(0, 0), size);
    // fill pixmap background with a checkerboard
    painter.fillRect(r, QBrush(checkerBoard));

    if (points_.size() > 0) {
        // draw TF gradient on top

        QVector<QGradientStop> gradientStops;
        for (const auto& tfpoint : points_)
        {
            auto curColor = tfpoint->getColor();
            // increase alpha to allow better visibility by 1 - (1 - a)^4
            auto aPercent = 1.0f - std::pow(1.0f - curColor.alphaF(), 4.0f);
            curColor.setAlpha(255*aPercent);
            gradientStops.append(QGradientStop(tfpoint->getPosition(), curColor));
        }

        // set bounds of the gradient
        QLinearGradient gradient;
        gradient.setStops(gradientStops);

        // gradient should stretch entire pixmap from left to right
        gradient.setStart(QPointF(0.0, 0.0));
        gradient.setFinalStop(QPointF(size.width(), 0.0));
        painter.fillRect(r, gradient);
    }

//        if (propertyConcept.hasIsovalues()) {
//            // draw a small hour glass for each isovalue
//            const double halfWidth = 6.0;
//            painter.save();
//            painter.setRenderHint(QPainter::Antialiasing);

//            const auto tfRange = propertyConcept.getIsovalues()->getRange();
//            auto normalize = [tfRange](double pos) {
//                return (pos - tfRange.x) / (tfRange.y - tfRange.x);
//            };

//            painter.setPen(QPen(Qt::black, 1.0, Qt::SolidLine));
//            // add vertical lines for each isovalue
//            for (const auto& isovalue : *propertyConcept.getIsovalues()) {
//                vec4 curColor = isovalue.getColor();
//                // increase alpha to allow better visibility by 1 - (1 - a)^4
//                curColor.a = 1.0f - std::pow(1.0f - curColor.a, 4.0f);
//                double pos = normalize(isovalue.getPosition()) * size.width();

//                painter.setBrush(utilqt::toQColor(curColor));
//                painter.drawPolygon(QPolygonF(
//                    {QPointF(pos - halfWidth, size.height()), QPointF(pos + halfWidth, size.height()),
//                     QPointF(pos - halfWidth, 0.0), QPointF(pos + halfWidth, 0.0)}));
//            }

//            painter.restore();
//        }

//        if (propertyConcept.supportsMask()) {
//            const auto tfRange = propertyConcept.getTransferFunction()->getRange();
//            // draw masking indicators
//            const QColor maskColor(25, 25, 25, 150);
//            if (propertyConcept.getMask().x > tfRange.x) {
//                // normalize mask position with respect to TF range
//                const double maskPos =
//                    (propertyConcept.getMask().x - tfRange.x) / (tfRange.y - tfRange.x);
//                QRect rectMask(QPoint(0, 0),
//                               QSize(static_cast<int>(maskPos * size.width()), size.height()));
//                painter.fillRect(rectMask, maskColor);
//                painter.drawLine(rectMask.bottomRight(), rectMask.topRight());
//            }

//            if (propertyConcept.getMask().y < tfRange.y) {
//                // normalize mask position with respect to TF range
//                const double maskPos =
//                    (propertyConcept.getMask().y - tfRange.x) / (tfRange.y - tfRange.x);
//                QRect rectMask(QPoint(static_cast<int>(maskPos * size.width()), 0),
//                               QPoint(size.width(), size.height()));
//                painter.fillRect(rectMask, maskColor);
//                painter.drawLine(rectMask.bottomLeft(), rectMask.topLeft());
//            }
//        }

    return tfPixmap;
}

void RangeColorEdit::onCurrentPointChanged()
{
    foreach( auto item, selectedItems() )
    {
        RangeColorEditControlPoint* point = qgraphicsitem_cast<RangeColorEditControlPoint*>(item);
        if(point)
        {
            if(point->getColor() != colorDialog_->currentColor())
            {
                colorDialog_->setCurrentColor(point->getColor());
                update();
                emit RangeColorChangeSignal();
            }
            return;
        }
    }
}

void RangeColorEdit::onCurrentColorChanged()
{
    foreach( auto item, selectedItems() )
    {
        RangeColorEditControlPoint* point = qgraphicsitem_cast<RangeColorEditControlPoint*>(item);
        if(point)
        {
            if(point->getColor() != colorDialog_->currentColor())
            {
                point->setColor(colorDialog_->currentColor());
                update();
                emit RangeColorChangeSignal();
            }
            return;
        }
    }
}

void RangeColorEdit::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    mouseDoubleClick_ = true;
    if(event->button() == Qt::MouseButton::LeftButton )
    {
        if (nullptr == getTFPrimitiveItemAt(event->scenePos()))
        {
            addControlPoint( event->scenePos() );
        }
    }


    QGraphicsScene::mouseDoubleClickEvent(event);
}

void RangeColorEdit::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
    switch (e->button())
    {
        case Qt::LeftButton:
           if (getTFPrimitiveItemAt(e->scenePos()))
           {
               mouseDrag_ = true;
               // inform all selected TF primitives about imminent move (need to cache position)
               for (auto& item : getSelectedPrimitiveItems())
               {
                   item->beginMouseDrag();
               }

           }
           else
           {
               views().front()->setDragMode(QGraphicsView::RubberBandDrag);
           }

           break;
        case Qt::RightButton:
           break;
        default:
           break;
       }

    mouseMovedSincePress_ = false;
    QGraphicsScene::mousePressEvent(e);
}

void RangeColorEdit::mouseMoveEvent(QGraphicsSceneMouseEvent* e)
{
    mouseMovedSincePress_ = true;
    if (mouseDrag_ && ((e->buttons() & Qt::LeftButton) == Qt::LeftButton))
    {
        // Prevent network evaluations while moving control point
        //NetworkLock lock;
        QGraphicsScene::mouseMoveEvent(e);
    } else {
        QGraphicsScene::mouseMoveEvent(e);
    }
}

void RangeColorEdit::mouseReleaseEvent(QGraphicsSceneMouseEvent* e)
{
    const bool controlPressed =
            ((QGuiApplication::queryKeyboardModifiers() & Qt::ControlModifier) == Qt::ControlModifier);

        switch (e->button())
        {
            case Qt::LeftButton:
            {
                if (!mouseMovedSincePress_)
                {
                    // add new TF primitive when control is pressed and nothing is below the cursor
                    if (controlPressed && !mouseDoubleClick_ && !mouseDrag_)
                    {
                        this->clearSelection();

                        KeepTrueWhileInScope k(&selectNewPrimitives_);
                        addControlPoint(e->scenePos());
                        e->accept();
                        }
                }
                if (mouseDrag_)
                {
                    for (auto& item : getSelectedPrimitiveItems()) {
                        item->stopMouseDrag();
                    }
                } else
                {
                    // disable rubber band selection
                    views().front()->setDragMode(QGraphicsView::NoDrag);
                }
            }
            case Qt::RightButton:
                break;
            default:
                break;
        }

        mouseDrag_ = false;
        mouseDoubleClick_ = false;
        mouseMovedSincePress_ = false;

        if (!e->isAccepted())
        {
            QGraphicsScene::mouseReleaseEvent(e);
        }
}

void RangeColorEdit::updateConnections()
{
    std::stable_sort(points_.begin(), points_.end(), comparePtr{});

    while (connections_.size() < points_.size() + 1) {
        auto c = new RangeColorEditControlPointConnection();
        connections_.push_back(c);
        addItem(c);
    }
    while (connections_.size() > points_.size() + 1) {
        auto c = connections_.back();
        removeItem(c);
        delete c;
        connections_.pop_back();
    }

    connections_[0]->left_ = nullptr;
    connections_[connections_.size() - 1]->right_ = nullptr;

    for (int i = 0; i < static_cast<int>(points_.size()); ++i)
    {
        points_[i]->left_ = connections_[i];
        points_[i]->right_ = connections_[i + 1];
        connections_[i]->right_ = points_[i];
        connections_[i + 1]->left_ = points_[i];
    }

    for (auto& elem : connections_)
    {
        elem->updateShape();
    }
}
