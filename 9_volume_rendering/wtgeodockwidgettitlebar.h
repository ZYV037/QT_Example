/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2012-2019 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************************/
#pragma once

#include <QWidget>

class QPaintEvent;
class QDockWidget;
class QToolButton;
class QLabel;
class QPushButton;


/*! \class WtgeoDockWidgetTitleBar
\brief Custom title bar widget for QDockWidget derived from QWidget.

The title bar widget contains a label showing the window title of the
parent QDockWidget and buttons for the sticky mode, dock/undock, and close.
The sticky mode determines whether the floating dock widget can be docked
to prevent involuntary docking when moving the dock widget. Docking via the
dock button or double clicking the title bar is still possible.

Note: adding a custom title bar to a QDockWidget removes the window decoration!
*/
class  WtgeoDockWidgetTitleBar : public QWidget {
    Q_OBJECT
    Q_PROPERTY(double iconSize READ getIconSize WRITE setIconSize DESIGNABLE true SCRIPTABLE true)

public:
    WtgeoDockWidgetTitleBar(QWidget* parent = nullptr, size_t bar_height = 33, const QString& custom_icon_file = "");
    virtual ~WtgeoDockWidgetTitleBar() = default;

    virtual void paintEvent(QPaintEvent*) override;

    //void floating(bool floating);

    double getIconSize() const { return iconSize_; }
    void setIconSize(double size);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    QToolButton* getCustomButton();
signals:
    void stickyFlagChanged(bool sticky);

private:
    void stickyBtnToggled(bool toggle);

    QDockWidget* parent_;

    QLabel* label_;
    //QToolButton* stickyBtn_;
    //QToolButton* floatBtn_;
    QToolButton* closeBtn_;
    Qt::DockWidgetAreas allowedDockAreas_;

    bool internalStickyFlagUpdate_;
    double iconSize_ = 2.0;
    QToolButton* customBtn_ = nullptr;
    size_t bar_height_ = 100;
};

