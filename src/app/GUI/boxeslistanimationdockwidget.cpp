// enve - 2D animations software
// Copyright (C) 2016-2019 Maurycy Liebner

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "boxeslistanimationdockwidget.h"
#include "mainwindow.h"
#include <QKeyEvent>
#include "canvaswindow.h"
#include "canvas.h"
#include "animationdockwidget.h"
#include <QScrollBar>
#include "GUI/BoxesList/boxscrollwidget.h"
#include "GUI/BoxesList/boxsinglewidget.h"
#include "widgetstack.h"
#include "actionbutton.h"
#include "GUI/RenderWidgets/renderwidget.h"
#include "boxeslistkeysviewwidget.h"
#include "animationwidgetscrollbar.h"
#include "GUI/global.h"
#include "renderinstancesettings.h"
#include "document.h"
#include "layouthandler.h"
#include "memoryhandler.h"

BoxesListAnimationDockWidget::BoxesListAnimationDockWidget(
        Document& document, LayoutHandler * const layoutH,
        MainWindow *parent) :
    QWidget(parent), mDocument(document),
    mTimelineLayout(layoutH->timelineLayout()) {
    connect(RenderHandler::sInstance, &RenderHandler::previewFinished,
            this, &BoxesListAnimationDockWidget::previewFinished);
    connect(RenderHandler::sInstance, &RenderHandler::previewBeingPlayed,
            this, &BoxesListAnimationDockWidget::previewBeingPlayed);
    connect(RenderHandler::sInstance, &RenderHandler::previewBeingRendered,
            this, &BoxesListAnimationDockWidget::previewBeingRendered);
    connect(RenderHandler::sInstance, &RenderHandler::previewPaused,
            this, &BoxesListAnimationDockWidget::previewPaused);

    setFocusPolicy(Qt::NoFocus);

    mMainWindow = parent;
    setMinimumSize(10*MIN_WIDGET_DIM, 12*MIN_WIDGET_DIM);
    mMainLayout = new QVBoxLayout(this);
    setLayout(mMainLayout);
    mMainLayout->setSpacing(0);
    mMainLayout->setMargin(0);

    mResolutionComboBox = new QComboBox(this);
    mResolutionComboBox->addItem("100 %");
    mResolutionComboBox->addItem("75 %");
    mResolutionComboBox->addItem("50 %");
    mResolutionComboBox->addItem("25 %");
    mResolutionComboBox->setEditable(true);
    mResolutionComboBox->lineEdit()->setInputMask("D00 %");
    mResolutionComboBox->setCurrentText("100 %");
    MainWindow::sGetInstance()->installNumericFilter(mResolutionComboBox);
    mResolutionComboBox->setInsertPolicy(QComboBox::NoInsert);
    mResolutionComboBox->setSizePolicy(QSizePolicy::Maximum,
                                       QSizePolicy::Maximum);
    connect(mResolutionComboBox, &QComboBox::currentTextChanged,
            this, &BoxesListAnimationDockWidget::setResolutionFractionText);

    const QString iconsDir = eSettings::sIconsDir() + "/toolbarButtons";

    mPlayButton = new ActionButton(iconsDir + "/play.png", "render preview", this);
    mStopButton = new ActionButton(iconsDir + "/stop.png", "stop preview", this);
    connect(mStopButton, &ActionButton::pressed,
            this, &BoxesListAnimationDockWidget::interruptPreview);

    mLocalPivot = new ActionButton(iconsDir + "/pivotGlobal.png",
                                  "pivot global/local", this);
    mLocalPivot->setToolTip("P");
    mLocalPivot->setCheckable(iconsDir + "/pivotLocal.png");
    mLocalPivot->setChecked(mDocument.fLocalPivot);
    connect(mLocalPivot, &ActionButton::toggled,
            this, &BoxesListAnimationDockWidget::setLocalPivot);

    mToolBar = new QToolBar(this);
    mToolBar->setMovable(false);

    mToolBar->setIconSize(QSize(5*MIN_WIDGET_DIM/4,
                                5*MIN_WIDGET_DIM/4));
    mToolBar->addSeparator();

//    mControlButtonsLayout->addWidget(mGoToPreviousKeyButton);
//    mGoToPreviousKeyButton->setFocusPolicy(Qt::NoFocus);
//    mControlButtonsLayout->addWidget(mGoToNextKeyButton);
//    mGoToNextKeyButton->setFocusPolicy(Qt::NoFocus);
    QAction *resA = mToolBar->addAction("Resolution:");
    mToolBar->widgetForAction(resA)->setObjectName("inactiveToolButton");

    mToolBar->addWidget(mResolutionComboBox);
    mToolBar->addSeparator();
    //mResolutionComboBox->setFocusPolicy(Qt::NoFocus);

    mToolBar->addWidget(mPlayButton);
    mToolBar->addWidget(mStopButton);

    mToolBar->addSeparator();
    mToolBar->addWidget(mLocalPivot);
    mLocalPivot->setFocusPolicy(Qt::NoFocus);
    mToolBar->addSeparator();

    QWidget * const spacerWidget = new QWidget(this);
    spacerWidget->setSizePolicy(QSizePolicy::Expanding,
                                QSizePolicy::Minimum);
    spacerWidget->setStyleSheet("QWidget {"
                                    "background-color: rgba(0, 0, 0, 0)"
                                "}");
    mToolBar->addWidget(spacerWidget);

    mToolBar->addSeparator();

    mTimelineAction = mToolBar->addAction("Timeline", this,
                                          &BoxesListAnimationDockWidget::setTimelineMode);
    mTimelineAction->setObjectName("customToolButton");
    mTimelineAction->setCheckable(true);
    mTimelineAction->setChecked(true);
    mRenderAction = mToolBar->addAction("Render", this,
                                        &BoxesListAnimationDockWidget::setRenderMode);
    mRenderAction->setObjectName("customToolButton");
    mRenderAction->setCheckable(true);

    mToolBar->addSeparator();

    mMainLayout->addWidget(mToolBar);

    mPlayButton->setEnabled(false);
    mStopButton->setEnabled(false);

    mRenderWidget = new RenderWidget(this);

    connect(&mDocument, &Document::activeSceneSet,
            this, [this](Canvas* const scene) {
        mPlayButton->setEnabled(scene);
        mStopButton->setEnabled(scene);
    });

    connect(mRenderWidget, &RenderWidget::renderFromSettings,
            this, [](RenderInstanceSettings* const settings) {
        RenderHandler::sInstance->renderFromSettings(settings);
    });

    mMainLayout->addWidget(mTimelineLayout);
    mMainLayout->addWidget(mRenderWidget);
    mRenderWidget->hide();

    previewFinished();
    //addNewBoxesListKeysViewWidget(1);
    //addNewBoxesListKeysViewWidget(0);

    connect(&mDocument, &Document::activeSceneSet,
            this, &BoxesListAnimationDockWidget::updateSettingsForCurrentCanvas);
}

void BoxesListAnimationDockWidget::setResolutionFractionText(QString text) {
    text = text.remove(" %");
    const qreal res = clamp(text.toDouble(), 1, 200)/100;
    mMainWindow->setResolutionFractionValue(res);
}

void BoxesListAnimationDockWidget::clearAll() {

}

RenderWidget *BoxesListAnimationDockWidget::getRenderWidget() {
    return mRenderWidget;
}

bool BoxesListAnimationDockWidget::processKeyPress(QKeyEvent *event) {
    const int key = event->key();
    const auto mods = event->modifiers();
    if(key == Qt::Key_Right && !(mods & Qt::ControlModifier)) {
        mDocument.incActiveSceneFrame();
    } else if(key == Qt::Key_Left && !(mods & Qt::ControlModifier)) {
        mDocument.decActiveSceneFrame();
    } else if(key == Qt::Key_Down && !(mods & Qt::ControlModifier)) {
        const auto scene = mDocument.fActiveScene;
        if(!scene) return false;
        int targetFrame;
        if(scene->anim_prevRelFrameWithKey(mDocument.getActiveSceneFrame(), targetFrame)) {
            mDocument.setActiveSceneFrame(targetFrame);
        }
    } else if(key == Qt::Key_Up && !(mods & Qt::ControlModifier)) {
        const auto scene = mDocument.fActiveScene;
        if(!scene) return false;
        int targetFrame;
        if(scene->anim_nextRelFrameWithKey(mDocument.getActiveSceneFrame(), targetFrame)) {
            mDocument.setActiveSceneFrame(targetFrame);
        }
    } else if(key == Qt::Key_P &&
              !(mods & Qt::ControlModifier) && !(mods & Qt::AltModifier)) {
        mLocalPivot->toggle();
    } else {
        return false;
    }
    return true;
}

void BoxesListAnimationDockWidget::previewFinished() {
    //setPlaying(false);
    mStopButton->setDisabled(true);
    const QString modeIconsDir = eSettings::sIconsDir() + "/toolbarButtons";
    mPlayButton->setIcon(modeIconsDir + "/play.png");
    mPlayButton->setToolTip("render preview");
    disconnect(mPlayButton, nullptr, this, nullptr);
    connect(mPlayButton, &ActionButton::pressed,
            this, &BoxesListAnimationDockWidget::renderPreview);
}

void BoxesListAnimationDockWidget::previewBeingPlayed() {
    mStopButton->setDisabled(false);
    const QString modeIconsDir = eSettings::sIconsDir() + "/toolbarButtons";
    mPlayButton->setIcon(modeIconsDir + "/pause.png");
    mPlayButton->setToolTip("pause preview");
    disconnect(mPlayButton, nullptr, this, nullptr);
    connect(mPlayButton, &ActionButton::pressed,
            this, &BoxesListAnimationDockWidget::pausePreview);
}

void BoxesListAnimationDockWidget::previewBeingRendered() {
    mStopButton->setDisabled(false);
    const QString modeIconsDir = eSettings::sIconsDir() + "/toolbarButtons";
    mPlayButton->setIcon(modeIconsDir + "/play.png");
    mPlayButton->setToolTip("play preview");
    disconnect(mPlayButton, nullptr, this, nullptr);
    connect(mPlayButton, &ActionButton::pressed,
            this, &BoxesListAnimationDockWidget::playPreview);
}

void BoxesListAnimationDockWidget::previewPaused() {
    mStopButton->setDisabled(false);
    const QString modeIconsDir = eSettings::sIconsDir() + "/toolbarButtons";
    mPlayButton->setIcon(modeIconsDir + "/play.png");
    mPlayButton->setToolTip("resume preview");
    disconnect(mPlayButton, nullptr, this, nullptr);
    connect(mPlayButton, &ActionButton::pressed,
            this, &BoxesListAnimationDockWidget::resumePreview);
}

void BoxesListAnimationDockWidget::resumePreview() {
    RenderHandler::sInstance->resumePreview();
}

void BoxesListAnimationDockWidget::pausePreview() {
    RenderHandler::sInstance->pausePreview();
}

void BoxesListAnimationDockWidget::playPreview() {
    RenderHandler::sInstance->playPreview();
}

void BoxesListAnimationDockWidget::renderPreview() {
    RenderHandler::sInstance->renderPreview();
}

void BoxesListAnimationDockWidget::interruptPreview() {
    RenderHandler::sInstance->interruptPreview();
}

void BoxesListAnimationDockWidget::setLocalPivot(const bool bT) {
    mDocument.fLocalPivot = bT;
    for(const auto& scene : mDocument.fScenes)
        scene->updatePivot();
    Document::sInstance->actionFinished();
}

void BoxesListAnimationDockWidget::setTimelineMode() {
    mTimelineAction->setDisabled(true);
    mRenderAction->setDisabled(false);

    mRenderAction->setChecked(false);
    mRenderWidget->hide();
    mTimelineLayout->show();
}

void BoxesListAnimationDockWidget::setRenderMode() {
    mTimelineAction->setDisabled(false);
    mRenderAction->setDisabled(true);

    mTimelineAction->setChecked(false);
    mTimelineLayout->hide();
    mRenderWidget->show();
}

void BoxesListAnimationDockWidget::updateSettingsForCurrentCanvas(
        Canvas* const canvas) {
    if(canvas) {
        disconnect(mResolutionComboBox, &QComboBox::currentTextChanged,
                   this, &BoxesListAnimationDockWidget::setResolutionFractionText);
        mResolutionComboBox->setCurrentText(
                    QString::number(canvas->getResolutionFraction()*100) + " %");
        connect(mResolutionComboBox, &QComboBox::currentTextChanged,
                this, &BoxesListAnimationDockWidget::setResolutionFractionText);
    }
}
