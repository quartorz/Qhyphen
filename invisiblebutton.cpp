#include "invisiblebutton.h"

#include <QPaintEvent>
#include <QPainter>

InvisibleButton::InvisibleButton(QWidget *parent) :
    QPushButton(parent)
{
}

void InvisibleButton::paintEvent(QPaintEvent *e)
{
    e->accept();

    if (underMouse()) {
        QPainter painter(this);
        painter.fillRect(rect(), QColor(255, 255, 255, 80));
    }
}
