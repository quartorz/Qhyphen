#ifndef INVISIBLEBUTTON_H
#define INVISIBLEBUTTON_H

#include <QPushButton>

class InvisibleButton : public QPushButton
{
public:
    InvisibleButton(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *e);
};

#endif // INVISIBLEBUTTON_H
