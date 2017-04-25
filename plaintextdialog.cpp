#include "plaintextdialog.h"
#include "ui_plaintextdialog.h"

PlainTextDialog::PlainTextDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlainTextDialog)
{
    ui->setupUi(this);
}

PlainTextDialog::~PlainTextDialog()
{
    delete ui;
}

QString PlainTextDialog::content() const
{
    return ui->plainTextEdit->document()->toPlainText();
}
