#include "rangecoloreditcontrolpoint.h"
#include <QGraphicsScene>
#include <QPainter>
#include <QColorDialog>
#include <QGuiApplication>
#include "rangecoloredit.h"
#include "rangecolorcontrolpointconnection.h"
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include "trtparser.h"

RangeColorEditControlPoint::RangeColorEditControlPoint(QGraphicsScene* scene, const QPointF& pos, double size):\
        size_(size), currentPos_(pos)
{
    setFlags(ItemIgnoresTransformations | ItemIsFocusable | ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
    setZValue(defaultZValue_);
    setAcceptHoverEvents(true);

    if (scene)
    {
        auto rect = scene->sceneRect();
        colorData_.position_ = (pos.x() - rect.x())/rect.width();
        colorData_.alpha_ = 255*((pos.y() - rect.y())/rect.height());
        updatePosition(currentPos_);
        scene->addItem(this);
    }

    label_ = new QGraphicsSimpleTextItem(this);
    label_->setVisible(hovered_);
    QFont font(label_->font());
    font.setPixelSize(14);
    label_->setFont(font);
}

RangeColorEditControlPoint::RangeColorEditControlPoint(QGraphicsScene* scene, const RangeColorPoint& point, double size):size_(size)
{
    setFlags(ItemIgnoresTransformations | ItemIsFocusable | ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
    setZValue(defaultZValue_);
    setAcceptHoverEvents(true);

    colorData_ = point;

    if (scene)
    {
        auto rect = scene->sceneRect();
        QPointF pos;
        pos.setX(rect.left() + rect.width()*point.position_);
        pos.setY(rect.y() + rect.height()*point.alpha_/255.0);

        updatePosition(pos);
        scene->addItem(this);
    }

    label_ = new QGraphicsSimpleTextItem(this);
    label_->setVisible(hovered_);
    QFont font(label_->font());
    font.setPixelSize(14);
    label_->setFont(font);
}

RangeColorEditControlPoint::~RangeColorEditControlPoint()
{
    if(label_)
    {
        delete label_;
    }
}

void RangeColorEditControlPoint::updatePosition(const QPointF& pos)
{
    currentPos_ = pos;
    QGraphicsItem::setPos(pos);
}

const QPointF& RangeColorEditControlPoint::getCurrentPos() const
{
    return currentPos_;
}

double RangeColorEditControlPoint::getPosition() const
{
    return colorData_.position_;
}

QColor RangeColorEditControlPoint::getColor() const
{
    return colorData_.getRgbaColor();
}

void RangeColorEditControlPoint::setColor(const QColor& color)
{
    colorData_.setColor(color);
    auto rect = scene()->sceneRect();
    currentPos_.setY(rect.y() + rect.height()*colorData_.alpha_/255.0);
    updatePosition(currentPos_);
    update();
}

QVector<uchar> RangeColorEditControlPoint::getUcharColor() const
{
    QVector<uchar> ucharColor(4);
    memcpy(ucharColor.data(), &colorData_, sizeof(uchar)*4);
    return ucharColor;
}

void RangeColorEditControlPoint::beginMouseDrag()
{
    cachedPosition_ = currentPos_;
    mouseDrag_ = true;
}

void RangeColorEditControlPoint::stopMouseDrag()
{
    mouseDrag_ = false;
}

void RangeColorEditControlPoint::setHovered(bool hover)
{
    prepareGeometryChange();
    hovered_ = hover;

    label_->setVisible(hover);
    updateLabel();

    update();
}


QPointF RangeColorEditControlPoint::prepareItemPositionChange(const QPointF& pos)
{
    QPointF adjustedPos(pos);

    if (auto tfe = qobject_cast<RangeColorEdit*>(scene())) {
        QRectF rect = scene()->sceneRect();
        const double d = 2.0 * rect.width() * std::numeric_limits<float>::epsilon();

        const int moveMode = 0;//tfe->getMoveMode();

        // need to update position prior to updating connections
        currentPos_ = adjustedPos;

        if (left_) {
            if (left_->left_ && *(left_->left_) > *this)
            {
                switch (moveMode) {
                    case 0:  // Free
                        break;
                    case 1:  // Restrict
                        adjustedPos.setX(left_->left_->getCurrentPos().x() + d);
                        // need to update position prior to updating connections
                        currentPos_ = adjustedPos;
                        break;
                    case 2:  // Push
                        left_->left_->setPos(
                            QPointF(adjustedPos.x() - d, left_->left_->getCurrentPos().y()));
                        break;
                }

                tfe->updateConnections();
            } else {
                currentPos_ = adjustedPos;
                left_->updateShape();
            }
        }
        if (right_) {
            if (right_->right_ && *(right_->right_) < *this) {
                switch (moveMode) {
                    case 0:  // Free
                        break;
                    case 1:  // Restrict
                        adjustedPos.setX(right_->right_->getCurrentPos().x() - d);
                        // need to update position prior to updating connections
                        currentPos_ = adjustedPos;
                        break;
                    case 2:  // Push
                        right_->right_->setPos(
                            QPointF(adjustedPos.x() + d, right_->right_->getCurrentPos().y()));
                        break;
                }

                tfe->updateConnections();
            } else
            {
                right_->updateShape();
            }
        }
    }

    return adjustedPos;
}

QRectF RangeColorEditControlPoint::boundingRect() const {
    double bBoxSize = size_ + 5.0;  //<! consider size of pen
    auto bRect = QRectF(-bBoxSize / 2.0, -bBoxSize / 2.0, bBoxSize, bBoxSize);

    return bRect;
}

QPainterPath RangeColorEditControlPoint::shape() const {
    QPainterPath path;
    const auto radius = size_ * 0.5 + 1.5;  //<! consider size of pen
    path.addEllipse(QPointF(0.0, 0.0), radius, radius);
    return path;
}


void RangeColorEditControlPoint::paint(QPainter* painter, const QStyleOptionGraphicsItem* options,
                              QWidget* widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);

    // set up pen and brush for drawing the primitive
    QPen pen = QPen();
    pen.setCosmetic(true);
    pen.setCapStyle(Qt::RoundCap);
    pen.setStyle(Qt::SolidLine);
    pen.setWidthF(3.0);

    isSelected() ? pen.setColor(QColor(213, 79, 79)) : pen.setColor(QColor(66, 66, 66));

    QBrush brush = QBrush( colorData_.getRgbColor() );

    painter->setPen(pen);
    painter->setBrush(brush);

    const auto radius = size_ * 0.5;
    painter->drawEllipse(QPointF(0.0, 0.0), radius, radius);
}

void RangeColorEditControlPoint::contextMenuEvent(QGraphicsSceneContextMenuEvent* e)
{
    const QPointF pos(e->scenePos());

   QMenu menu;
   auto deletePrimitive = menu.addAction("Delete Point");
   if ( menu.exec(e->screenPos()) == deletePrimitive)
   {
        if (auto tfe = qobject_cast<RangeColorEdit*>(scene()))
        {
            tfe->deleteControlPoint(this);
        }
        e->accept();
   }
}

QVariant RangeColorEditControlPoint::itemChange(GraphicsItemChange change, const QVariant& value)
{
    // check for scene() here in order to avoid callbacks as long as item is not added to scene
    if ((change == QGraphicsItem::ItemPositionChange) && scene()) {
        // constrain positions to valid view positions
        auto newpos = value.toPointF();

        const bool shiftPressed =
            ((QGuiApplication::queryKeyboardModifiers() & Qt::ShiftModifier) == Qt::ShiftModifier);
        // restrict movement to either horizontal or vertical direction while shift is pressed
        if (mouseDrag_ && shiftPressed) {
            // adjust position of mouse event
            auto delta = newpos - cachedPosition_;
            if (std::abs(delta.x()) > std::abs(delta.y())) {
                // horizontal movement is dominating
                newpos.ry() = cachedPosition_.y();
            } else {
                // vertical movement is dominating
                newpos.rx() = cachedPosition_.x();
            }
        }

        QRectF rect = scene()->sceneRect();

        if (!rect.contains(newpos))
        {
            newpos.setX(qMin(rect.right(), qMax(newpos.x(), rect.left())));
            newpos.setY(qMin(rect.bottom(), qMax(newpos.y(), rect.top())));
        }

        // allow for adjusting the position in derived classes
        currentPos_ = prepareItemPositionChange(newpos);
        colorData_.position_ = (currentPos_.x() - rect.x())/rect.width();
        colorData_.alpha_ = (255 * (currentPos_.y() - rect.y())/rect.height() );

        if (!isEditingPoint_)
        {
            isEditingPoint_ = true;

            // update the associated transfer function primitive
            //onItemPositionChange(vec2(static_cast<float>(currentPos_.x() / rect.width()),
            //                          static_cast<float>(currentPos_.y() / rect.height())));
            // update label
            //updateLabel();
            updatePosition(newpos);
            isEditingPoint_ = false;
        }

        // return the constrained position
        return currentPos_;
    } else if (change == QGraphicsItem::ItemSceneHasChanged)
    {
        // inform primitive about scene change
        //onItemSceneHasChanged();
    }

    return QGraphicsItem::itemChange(change, value);
}

void RangeColorEditControlPoint::hoverEnterEvent(QGraphicsSceneHoverEvent*) {
    // ensure that this primitive is showing up on top for the duration of the hover
    setZValue(1000);
    setHovered(true);
}

void RangeColorEditControlPoint::hoverLeaveEvent(QGraphicsSceneHoverEvent*) {
    setHovered(false);
    setZValue(defaultZValue_);
}

void RangeColorEditControlPoint::updateLabel()
{
    if (!label_->isVisible())
    {
        return;
    }

    float min;
    float max;
    getTrtDataRange(min, max);
    QString label = QString("x=%1 alpha=%2").arg(min + colorData_.position_*(max-min), 0, 'g', 3).arg(colorData_.alpha_);


    label_->setText(label);

    const double distFromCenter = size_ * 0.7;
    QPointF pos(distFromCenter, distFromCenter);

    auto rect = label_->boundingRect();
    if (currentPos_.x() > scene()->sceneRect().width() * 0.5)
    {
        pos.rx() = -rect.width() - distFromCenter;
    }
    if (currentPos_.y() < scene()->sceneRect().height() * 0.5) {
        pos.ry() = -rect.height() - distFromCenter;
    }

    label_->setPos(pos);
}

bool operator==(const RangeColorEditControlPoint& lhs, const RangeColorEditControlPoint& rhs) {
    return (lhs.getCurrentPos() == rhs.getCurrentPos()) && (lhs.getColor() == rhs.getColor());
}

bool operator!=(const RangeColorEditControlPoint& lhs, const RangeColorEditControlPoint& rhs) {
    return !operator==(lhs, rhs);
}

bool operator<(const RangeColorEditControlPoint& lhs, const RangeColorEditControlPoint& rhs)
{
    return lhs.getCurrentPos().x() < rhs.getCurrentPos().x();
}

bool operator>(const RangeColorEditControlPoint& lhs, const RangeColorEditControlPoint& rhs) { return rhs < lhs; }

bool operator<=(const RangeColorEditControlPoint& lhs, const RangeColorEditControlPoint& rhs) { return !(rhs < lhs); }

bool operator>=(const RangeColorEditControlPoint& lhs, const RangeColorEditControlPoint& rhs) { return !(lhs < rhs); }

