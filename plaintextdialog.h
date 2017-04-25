#ifndef PLAINTEXTDIALOG_H
#define PLAINTEXTDIALOG_H

#include <QDialog>

namespace Ui {
    class PlainTextDialog;
}

class PlainTextDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlainTextDialog(QWidget *parent = 0);
    ~PlainTextDialog();

    QString content() const;

private:
    Ui::PlainTextDialog *ui;
};

#endif // PLAINTEXTDIALOG_H
