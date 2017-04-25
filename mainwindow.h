#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ImageList;
class QPlainTextEdit;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool eventFilter(QObject *watched, QEvent *e);
    void resizeEvent(QResizeEvent *e);
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);
    void paintEvent(QPaintEvent *e);

private:
    ImageList *imageList;
    Ui::MainWindow *ui;
    QMovie *movie;

public slots:
    void setFileName(const QString &);
    void inputAsText();
};

#endif // MAINWINDOW_H
