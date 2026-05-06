#include "guiutil.h"

#include <QWidget>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QAbstractAnimation>

GUIUtil::GUIUtil() {}

void GUIUtil::applyWidgetFade(QWidget *widget, int duration){

    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(widget);
    widget->setGraphicsEffect(opacityEffect);

    QPropertyAnimation *fade = new QPropertyAnimation(opacityEffect, "opacity");
    fade->setDuration(duration);
    fade->setStartValue(0.0);
    fade->setEndValue(1.0);
    fade->start(QAbstractAnimation::DeleteWhenStopped);

    QAbstractAnimation::connect(fade, &QPropertyAnimation::finished,
            [=](){
                widget->setGraphicsEffect(nullptr);
            }
    );
}

