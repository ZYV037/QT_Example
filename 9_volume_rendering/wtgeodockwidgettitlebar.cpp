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

#include "wtgeodockwidgettitlebar.h"

#include <QDockWidget>
#include <QEvent>
#include <QLabel>
#include <QIcon>
#include <QPixmap>
#include <QToolButton>
#include <QHBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include "unity.h"
#include <QPushButton>


WtgeoDockWidgetTitleBar::WtgeoDockWidgetTitleBar(QWidget *parent, size_t bar_height, const QString& custom_icon_file)
    : QWidget(parent)   \
    , parent_(dynamic_cast<QDockWidget *>(parent))  \
    , allowedDockAreas_(parent_->allowedAreas())    \
    , internalStickyFlagUpdate_(false)  \
    , bar_height_(bar_height) \
{
    label_ = new QLabel(parent->windowTitle(), this);
    label_->setObjectName("WtgeoDockWidgetTitleBarLabel");

    const auto iconsize = emToPx(this, QSizeF(iconSize_, iconSize_));

    if( custom_icon_file.isEmpty() == false )
    {
        customBtn_= new QToolButton();
        QIcon icon;
        icon.addFile(custom_icon_file, iconsize);
        customBtn_->setIcon(icon);
        customBtn_->setCheckable(true);
        customBtn_->setChecked(parent_->isFloating());
        customBtn_->setIconSize(iconsize);
    }

    {
        closeBtn_ = new QToolButton();
        QIcon icon;
        icon.addFile(":/svgicons/close.svg", iconsize);
        closeBtn_->setIcon(icon);
        closeBtn_->setIconSize(iconsize);
    }

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(label_);
    layout->addStretch(1);
    if(customBtn_)
    {
        layout->addWidget(customBtn_, 0, Qt::AlignmentFlag::AlignRight);
        layout->addSpacing(10);
    }

    layout->addWidget(closeBtn_, 0, Qt::AlignmentFlag::AlignRight);
    layout->setSpacing(0);
    layout->setMargin(emToPx(this, 0.2));

    setLayout(layout);

    QObject::connect(closeBtn_, &QToolButton::clicked, parent_, &QDockWidget::close);
}

void WtgeoDockWidgetTitleBar::paintEvent(QPaintEvent *) {
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void WtgeoDockWidgetTitleBar::stickyBtnToggled(bool toggle) {
    internalStickyFlagUpdate_ = true;
    if (toggle) {
        // docking allowed, restore docking areas
        parent_->setAllowedAreas(allowedDockAreas_);
    } else {
        // no docking, disable all areas
        parent_->setAllowedAreas(Qt::NoDockWidgetArea);
    }
    emit stickyFlagChanged(toggle);
    internalStickyFlagUpdate_ = false;
}


//void WtgeoDockWidgetTitleBar::floating(bool floating) { floatBtn_->setChecked(floating); }

void WtgeoDockWidgetTitleBar::setIconSize(double size) {
    iconSize_ = size;
    const auto iconsize = emToPx(this, QSizeF(iconSize_, iconSize_));

    for (auto tb : layout()->findChildren<QToolButton *>()) {
        tb->setIconSize(iconsize);
    }
}

QSize WtgeoDockWidgetTitleBar::sizeHint() const
{
    return QSize(bar_height_, bar_height_);
}

QSize WtgeoDockWidgetTitleBar::minimumSizeHint() const
{
    return QSize(10, 10);
}

QToolButton* WtgeoDockWidgetTitleBar::getCustomButton()
{
    return customBtn_;
}

