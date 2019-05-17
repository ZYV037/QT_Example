#pragma once

#include "wtgeodockwidget.h"

class RangeColorSetDialog;
class QLineEdit;
class QPushButton;
class VolumetricModifier;

class Inversion3dRightDockWidget: public WtgeoDockWidget
{
public:
    Inversion3dRightDockWidget(QWidget* parent = nullptr, VolumetricModifier* modifier=nullptr);

    bool OpenTrtFile(const QString& trtFile);
    bool saveColorPoints();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void updateColorsetButton();

private:
    RangeColorSetDialog* rangeColorDialog_ = nullptr;
    QLineEdit* trtFolder_;
    QPushButton* colorSetButton_;
    VolumetricModifier *modifier_;
    QVector<uchar> customData_;
};
