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
#include "wtgeodockwidget.h"
#include "wtgeodockwidgettitlebar.h"
#include <QKeyEvent>
#include <QSettings>
#include "unity.h"
#include <QScrollArea>

WtgeoDockWidget::WtgeoDockWidget(QString title, QWidget *parent, size_t bar_height, const QString& custom_icon_file)    \
    : QDockWidget(title, parent)
{
    setObjectName(title);

    // adding custom title bar to dock widget
    dockWidgetTitleBar_ = new WtgeoDockWidgetTitleBar(this, bar_height, custom_icon_file);
    setTitleBarWidget(dockWidgetTitleBar_);

    setAllowedAreas(Qt::AllDockWidgetAreas);

    const auto space = refSpacePx(this);
    scrollArea_ = new QScrollArea(this);
    scrollArea_->setWidgetResizable(true);
    scrollArea_->setMinimumWidth(emToPx(this, 30));
    scrollArea_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea_->setFrameShape(QFrame::NoFrame);
    scrollArea_->setContentsMargins(0, space, 0, space);
}

QToolButton* WtgeoDockWidget::getCustomButton()
{
    return dockWidgetTitleBar_->getCustomButton();
}

void WtgeoDockWidget::setContents(QWidget *widget) {
    QWidget *oldWidget = this->widget();
    if (oldWidget) {
        this->setWidget(nullptr);
        delete oldWidget;
    }

    this->setWidget(widget);
}

void WtgeoDockWidget::setContents(QLayout *layout) {
    QWidget *oldWidget = this->widget();
    if (oldWidget) {
        this->setWidget(nullptr);
        delete oldWidget;
    }

    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(layout);
    this->setWidget(centralWidget);
}

void WtgeoDockWidget::saveState() {
    QSettings settings;
    settings.beginGroup(objectName());
    settings.setValue("floating", isFloating());
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.setValue("visible", isVisible());
    settings.endGroup();
}

void WtgeoDockWidget::closeEvent(QCloseEvent *event)
{
    saveState();
    QDockWidget::closeEvent(event);
}
