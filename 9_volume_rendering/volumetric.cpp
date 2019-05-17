/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Data Visualization module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "volumetric.h"
#include <QtDataVisualization/qvalue3daxis.h>
#include <QtDataVisualization/q3dscene.h>
#include <QtDataVisualization/q3dcamera.h>
#include <QtDataVisualization/q3dtheme.h>
#include <QtDataVisualization/qcustom3dlabel.h>
#include <QtDataVisualization/q3dscatter.h>
#include <QtDataVisualization/q3dinputhandler.h>
#include <QtCore/qmath.h>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtCore/QDebug>
#include <QtGui/QOpenGLContext>
#include "trtparser.h"

using namespace QtDataVisualization;

static bool isOpenGLES()
{
#if defined(QT_OPENGL_ES_2)
    return true;
#elif (QT_VERSION < QT_VERSION_CHECK(5, 3, 0))
    return false;
#else
    return QOpenGLContext::currentContext()->isOpenGLES();
#endif
}

VolumetricModifier::VolumetricModifier(Q3DScatter *scatter)
    : m_graph(scatter),
      m_sliceIndexX(X_SIZE / 2),
      m_sliceIndexY(Z_SIZE / 2),
      m_sliceIndexZ(Y_SIZE / 2),
      m_slicingX(false),
      m_slicingY(false),
      m_slicingZ(false),
      m_mediumDetailIndex(0),
      m_highDetailIndex(0),
      m_mediumDetailShaftIndex(0),
      m_highDetailShaftIndex(0),
      m_sliceSliderX(0),
      m_sliceSliderY(0),
      m_sliceSliderZ(0),
      m_usingPrimaryTable(true),
      m_sliceLabelX(0),
      m_sliceLabelY(0),
      m_sliceLabelZ(0)
{
    m_graph->activeTheme()->setType(Q3DTheme::ThemeQt);
    m_graph->setShadowQuality(QAbstract3DGraph::ShadowQualityHigh);
    m_graph->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetIsometricRightHigh);
    //! [6]
    m_graph->setOrthoProjection(true);
    //! [6]
    m_graph->activeTheme()->setBackgroundEnabled(false);

    // Only allow zooming at the center and limit the zoom to 200% to avoid clipping issues
    static_cast<Q3DInputHandler *>(m_graph->activeInputHandler())->setZoomAtTargetEnabled(false);
    m_graph->scene()->activeCamera()->setMaxZoomLevel(200.0f);

    toggleAreaAll(true);

    if (!isOpenGLES())
    {
        m_graph->setHorizontalAspectRatio(0.25);
        m_graph->setAspectRatio(4.0);
        m_volumeItem = new QCustom3DVolume;
        m_volumeItem->setScaling(
                    QVector3D((m_graph->axisX()->max() - m_graph->axisX()->min()),
                              (m_graph->axisY()->max() - m_graph->axisY()->min()),
                              m_graph->axisZ()->max() - m_graph->axisZ()->min()));
        m_volumeItem->setPosition(
                    QVector3D((m_graph->axisX()->max() + m_graph->axisX()->min()) / 2.0f,
//                              -0.045f * (m_graph->axisY()->max() - m_graph->axisY()->min()) +
                              (m_graph->axisY()->max() + m_graph->axisY()->min()) / 2.0f,
                              (m_graph->axisZ()->max() + m_graph->axisZ()->min()) / 2.0f));
        m_volumeItem->setScalingAbsolute(false);

        m_volumeItem->setDrawSliceFrames(false);

        m_volumeItem->setTextureFormat(QImage::Format_ARGB32);

        handleSlicingChanges();

        m_graph->addCustomItem(m_volumeItem);
    }
    else
    {
        // OpenGL ES2 doesn't support 3D textures, so show a warning label instead
        QCustom3DLabel *warningLabel = new QCustom3DLabel(
                    "QCustom3DVolume is not supported with OpenGL ES2",
                    QFont(),
                    QVector3D(0.0f, 0.5f, 0.0f),
                    QVector3D(1.5f, 1.5f, 0.0f),
                    QQuaternion());
        warningLabel->setPositionAbsolute(true);
        warningLabel->setFacingCamera(true);
        m_graph->addCustomItem(warningLabel);
    }

    QObject::connect(m_graph, &QAbstract3DGraph::currentFpsChanged, this,
                     &VolumetricModifier::handleFpsChange);

}

VolumetricModifier::~VolumetricModifier()
{
    delete m_graph;
}

void VolumetricModifier::setFpsLabel(QLabel *fpsLabel)
{
    m_fpsLabel = fpsLabel;
}

void VolumetricModifier::setMediumDetailRB(QRadioButton *button)
{
    //m_mediumDetailRB = button;
}

void VolumetricModifier::setHighDetailRB(QRadioButton *button)
{
    //m_highDetailRB = button;
}

void VolumetricModifier::setSliceLabels(QLabel *xLabel, QLabel *yLabel, QLabel *zLabel)
{
    m_sliceLabelX = xLabel;
    m_sliceLabelY = yLabel;
    m_sliceLabelZ = zLabel;

    adjustSliceX(m_sliceSliderX->value());
    adjustSliceY(m_sliceSliderY->value());
    adjustSliceZ(m_sliceSliderZ->value());
}

void VolumetricModifier::setAlphaMultiplierLabel(QLabel *label)
{
    m_alphaMultiplierLabel = label;
}

void VolumetricModifier::sliceX(int enabled)
{
    m_slicingX = enabled;
    handleSlicingChanges();
}

void VolumetricModifier::sliceY(int enabled)
{
    m_slicingY = enabled;
    handleSlicingChanges();
}

void VolumetricModifier::sliceZ(int enabled)
{
    m_slicingZ = enabled;
    handleSlicingChanges();
}

void VolumetricModifier::adjustSliceX(int value)
{
    if (m_volumeItem) {
        m_sliceIndexX = value /*/ (1024 / m_volumeItem->textureWidth())*/;
        if (m_sliceIndexX == m_volumeItem->textureWidth())
            m_sliceIndexX--;
        if (m_volumeItem->sliceIndexX() != -1)
            //! [7]
            m_volumeItem->setSliceIndexX(m_sliceIndexX);
            //! [7]
        //! [9]
        m_sliceLabelX->setPixmap(
                    QPixmap::fromImage(m_volumeItem->renderSlice(Qt::XAxis, m_sliceIndexX)));
        //! [9]
    }
}

void VolumetricModifier::adjustSliceY(int value)
{
    if (m_volumeItem) {
        m_sliceIndexY = value /*/ (1024 / m_volumeItem->textureHeight())*/;
        if (m_sliceIndexY == m_volumeItem->textureHeight())
            m_sliceIndexY--;
        if (m_volumeItem->sliceIndexY() != -1)
            m_volumeItem->setSliceIndexY(m_sliceIndexY);
        QTransform transform;
        transform.rotate(90.0);
        //painter->drawPixmap(initialXp + frameCount * detaX[i],initialYp + frameCount * detaY[i],50,50,(*card).transformed(transform,Qt::SmoothTransformation)); }
        m_sliceLabelY->setPixmap(
                    QPixmap::fromImage(m_volumeItem->renderSlice(Qt::YAxis, m_sliceIndexY).transformed(transform)));
    }
}

void VolumetricModifier::adjustSliceZ(int value)
{
    if (m_volumeItem) {
        m_sliceIndexZ = value /*/ (1024 / m_volumeItem->textureDepth())*/;
        if (m_sliceIndexZ == m_volumeItem->textureDepth())
            m_sliceIndexZ--;
        if (m_volumeItem->sliceIndexZ() != -1)
            m_volumeItem->setSliceIndexZ(m_sliceIndexZ);
        m_sliceLabelZ->setPixmap(
                    QPixmap::fromImage(m_volumeItem->renderSlice(Qt::ZAxis, m_sliceIndexZ)));
    }
}

void VolumetricModifier::handleFpsChange(qreal fps)
{
    const QString fpsFormat = QStringLiteral("FPS: %1");
    int fps10 = int(fps * 10.0);
    m_fpsLabel->setText(fpsFormat.arg(qreal(fps10) / 10.0));
}

void VolumetricModifier::toggleCustomData(const QVector<uchar>& customData)
{
    m_graph->setHorizontalAspectRatio(0.25);
    m_graph->setAspectRatio(4.0);

    handleSlicingChanges();

    m_volumeItem->setTextureData(new QVector<uchar>(customData));

    m_volumeItem->setTextureWidth(X_SIZE);
    m_volumeItem->setTextureHeight(Z_SIZE);
    m_volumeItem->setTextureDepth(Y_SIZE);

    m_graph->axisX()->setRange(0.0f, X_SIZE);
    m_graph->axisY()->setRange(0.0f, Z_SIZE);
    m_graph->axisZ()->setRange(0.0f, Y_SIZE);

    adjustSliceX(m_sliceSliderX->value());
    adjustSliceY(m_sliceSliderY->value());
    adjustSliceZ(m_sliceSliderZ->value());

    setSliceRange();

    m_volumeItem->setScaling(
                QVector3D((m_graph->axisX()->max() - m_graph->axisX()->min()),
                          (m_graph->axisY()->max() - m_graph->axisY()->min()),
                          m_graph->axisZ()->max() - m_graph->axisZ()->min()));
    m_volumeItem->setPosition(
                QVector3D((m_graph->axisX()->max() + m_graph->axisX()->min()) / 2.0f,
//                              -0.045f * (m_graph->axisY()->max() - m_graph->axisY()->min()) +
                          (m_graph->axisY()->max() + m_graph->axisY()->min()) / 2.0f,
                          (m_graph->axisZ()->max() + m_graph->axisZ()->min()) / 2.0f));
}


void VolumetricModifier::setFpsMeasurement(bool enabled)
{
    m_graph->setMeasureFps(enabled);
    if (enabled)
        m_fpsLabel->setText(QStringLiteral("Measuring..."));
    else
        m_fpsLabel->setText(QString());
}

void VolumetricModifier::setSliceSliders(QSlider *sliderX, QSlider *sliderY, QSlider *sliderZ)
{
    m_sliceSliderX = sliderX;
    m_sliceSliderY = sliderY;
    m_sliceSliderZ = sliderZ;

    setSliceRange();
}

void VolumetricModifier::setSliceRange()
{
    m_sliceSliderX->setMinimum(0);
    m_sliceSliderX->setMaximum(X_SIZE);
    m_sliceSliderX->setValue(X_SIZE/2);
    m_sliceSliderX->setEnabled(true);

    m_sliceSliderY->setMinimum(0);
    m_sliceSliderY->setMaximum(Z_SIZE);
    m_sliceSliderY->setValue(Z_SIZE/2);
    m_sliceSliderY->setEnabled(true);

    m_sliceSliderZ->setMinimum(0);
    m_sliceSliderZ->setMaximum(Y_SIZE);
    m_sliceSliderZ->setValue(Y_SIZE/2);
    m_sliceSliderZ->setEnabled(true);
}

void VolumetricModifier::changeColorTable(int enabled)
{
    if (m_volumeItem) {
        if (enabled)
            m_volumeItem->setColorTable(m_colorTable2);
        else
            m_volumeItem->setColorTable(m_colorTable1);

        m_usingPrimaryTable = !enabled;

        // Rerender image labels
        adjustSliceX(m_sliceSliderX->value());
        adjustSliceY(m_sliceSliderY->value());
        adjustSliceZ(m_sliceSliderZ->value());
    }
}

void VolumetricModifier::setPreserveOpacity(bool enabled)
{
    if (m_volumeItem) {
        //! [10]
        m_volumeItem->setPreserveOpacity(enabled);
        //! [10]

        // Rerender image labels
        adjustSliceX(m_sliceSliderX->value());
        adjustSliceY(m_sliceSliderY->value());
        adjustSliceZ(m_sliceSliderZ->value());
    }
}

void VolumetricModifier::setUseHighDefShader(bool enabled)
{
    if (m_volumeItem) {
        //! [13]
        m_volumeItem->setUseHighDefShader(enabled);
        //! [13]
    }
}

void VolumetricModifier::adjustAlphaMultiplier(int value)
{
    if (m_volumeItem) {
        float mult;
        if (value > 100)
            mult = float(value - 99) / 2.0f;
        else
            mult = float(value) / float(500 - value * 4);
        //! [11]
        m_volumeItem->setAlphaMultiplier(mult);
        //! [11]
        QString labelFormat = QStringLiteral("Alpha multiplier: %1");
        m_alphaMultiplierLabel->setText(labelFormat.arg(
                                            QString::number(m_volumeItem->alphaMultiplier(), 'f', 3)));

        // Rerender image labels
        adjustSliceX(m_sliceSliderX->value());
        adjustSliceY(m_sliceSliderY->value());
        adjustSliceZ(m_sliceSliderZ->value());
    }
}

void VolumetricModifier::toggleAreaAll(bool enabled)
{
    if (enabled) {
        m_graph->axisX()->setSegmentCount(5);
        m_graph->axisY()->setSegmentCount(5);
        m_graph->axisZ()->setSegmentCount(10);

        m_graph->axisX()->setTitle("X");
        m_graph->axisX()->setTitleVisible(true);
        m_graph->axisY()->setTitle("Y");
        m_graph->axisY()->setTitleVisible(true);
        m_graph->axisZ()->setTitle("Z");
        m_graph->axisZ()->setTitleVisible(true);
    }
}

void VolumetricModifier::setDrawSliceFrames(int enabled)
{
    if (m_volumeItem)
        m_volumeItem->setDrawSliceFrames(enabled);
}

void VolumetricModifier::handleSlicingChanges()
{
    if (m_volumeItem) {
        if (m_slicingX || m_slicingY || m_slicingZ) {
            // Only show slices of selected dimensions
            //! [8]
            m_volumeItem->setDrawSlices(true);
            //! [8]
            m_volumeItem->setSliceIndexX(m_slicingX ? m_sliceIndexX : -1);
            m_volumeItem->setSliceIndexY(m_slicingY ? m_sliceIndexY : -1);
            m_volumeItem->setSliceIndexZ(m_slicingZ ? m_sliceIndexZ : -1);
        } else {
            // Show slice frames for all dimenstions when not actually slicing
            m_volumeItem->setDrawSlices(false);
            m_volumeItem->setSliceIndexX(m_sliceIndexX);
            m_volumeItem->setSliceIndexY(m_sliceIndexY);
            m_volumeItem->setSliceIndexZ(m_sliceIndexZ);
        }
    }
}
