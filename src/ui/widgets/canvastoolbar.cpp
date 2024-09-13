/*
#
# Friction - https://friction.graphics
#
# Copyright (c) Friction contributors
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# See 'README.md' for more information.
#
*/

#include "canvastoolbar.h"
#include "GUI/global.h"
#include "Private/document.h"
#include "widgets/editablecombobox.h"

#include <QLabel>
#include <QLineEdit>
#include <QDebug>

using namespace Friction::Ui;

CanvasToolBar::CanvasToolBar(QWidget *parent)
    : QToolBar(parent)
    , mSpinWidth(nullptr)
    , mSpinHeight(nullptr)
    , mComboResolution(nullptr)
{
    eSizesUI::widget.add(this, [this](const int size) {
        this->setIconSize({size, size});
    });

    setEnabled(false);
    setWindowTitle(tr("Canvas Toolbar"));
    setObjectName("CanvasToolBar");

    {
        const auto space = new QWidget(this);
        space->setSizePolicy(QSizePolicy::Expanding,
                             QSizePolicy::Expanding);
        addWidget(space);
    }

    setupDimensions();
    setupResolution();
}

void CanvasToolBar::setCurrentCanvas(Canvas * const target)
{
    mCanvas.assign(target);
    if (target) {
        mCanvas << connect(mComboResolution, &QComboBox::currentTextChanged,
                           this, [this, target](const QString &text) {
            setResolution(text, target);
        });
        mCanvas << connect(mSpinWidth,
                           QOverload<int>::of(&QSpinBox::valueChanged),
                           this, [this, target]() {
            setDimension({mSpinWidth->value(), mSpinHeight->value()},
                          target);
        });
        mCanvas << connect(mSpinHeight,
                           QOverload<int>::of(&QSpinBox::valueChanged),
                           this, [this, target]() {
            setDimension({mSpinWidth->value(), mSpinHeight->value()},
                         target);
        });
        mCanvas << connect(target, &Canvas::dimensionsChanged,
                           this, [this](int width, int height) {
            updateDimension({width, height});
        });
    }
    updateWidgets(target);
}

QComboBox *CanvasToolBar::getResolutionComboBox()
{
    return mComboResolution;
}

void CanvasToolBar::setupDimensions()
{
    mSpinWidth = new QSpinBox(this);
    mSpinWidth->setFocusPolicy(Qt::ClickFocus);
    mSpinWidth->setObjectName("ComboSpinBox");
    mSpinWidth->setMinimum(1);
    mSpinWidth->setMaximum(99999);
    mSpinWidth->setKeyboardTracking(false);

    mSpinHeight = new QSpinBox(this);
    mSpinWidth->setFocusPolicy(Qt::ClickFocus);
    mSpinHeight->setObjectName("ComboSpinBox");
    mSpinHeight->setMinimum(1);
    mSpinHeight->setMaximum(99999);
    mSpinHeight->setKeyboardTracking(false);

    addWidget(new QLabel(tr("Width"), this));
    addSeparator();

    addWidget(mSpinWidth);

    addSeparator();
    addWidget(new QLabel(tr("Height"), this));
    addSeparator();

    addWidget(mSpinHeight);
}

void CanvasToolBar::setupResolution()
{
    mComboResolution = new EditableComboBox(this);
    mComboResolution->setMinimumWidth(75);
    mComboResolution->setFocusPolicy(Qt::ClickFocus);
    mComboResolution->addItem("100 %");
    mComboResolution->addItem("75 %");
    mComboResolution->addItem("50 %");
    mComboResolution->addItem("25 %");
    mComboResolution->lineEdit()->setInputMask("D00 %");
    mComboResolution->setCurrentText("100 %");
    mComboResolution->setInsertPolicy(QComboBox::NoInsert);
    //mComboResolution->setSizePolicy(QSizePolicy::Preferred,
    //                                QSizePolicy::Preferred);

    addSeparator();
    addWidget(new QLabel(tr("Resolution"), this));
    addSeparator();

    addWidget(mComboResolution);
}

void CanvasToolBar::addSpacer()
{
    const auto space = new QWidget(this);
    space->setSizePolicy(QSizePolicy::Expanding,
                         QSizePolicy::Minimum);
    addWidget(space);
}

void CanvasToolBar::updateWidgets(Canvas * const target)
{
    if (!target) {
        setEnabled(false);
        return;
    }
    setEnabled(true);

    updateDimension(target->getCanvasSize());

    mComboResolution->blockSignals(true);
    mComboResolution->setCurrentText(QString("%1 %")
                                         .arg(target->getResolution() * 100));
    mComboResolution->blockSignals(false);
}

void CanvasToolBar::updateDimension(const QSize dim)
{
    mSpinWidth->blockSignals(true);
    mSpinWidth->setValue(dim.width());
    mSpinWidth->blockSignals(false);

    mSpinHeight->blockSignals(true);
    mSpinHeight->setValue(dim.height());
    mSpinHeight->blockSignals(false);
}

void CanvasToolBar::setResolution(QString text,
                                  Canvas * const target)
{
    if (!target) { return; }

    const qreal percent = clamp(text.remove("%").simplified().toDouble(),
                                1, 200) / 100;

    target->setResolution(percent);
    Document::sInstance->actionFinished();
}

void CanvasToolBar::setDimension(const QSize dim,
                                 Canvas * const target)
{
    if (!target) { return; }

    target->setCanvasSize(dim.width(),
                          dim.height());
    Document::sInstance->actionFinished();
}
