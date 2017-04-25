#include "invisiblebutton.h"

#include <QPaintEvent>

InvisibleButton::InvisibleButton(QWidget *parent) :
    QPushButton(parent)
{
}

void InvisibleButton::paintEvent(QPaintEvent *e)
{
    e->accept();
}
