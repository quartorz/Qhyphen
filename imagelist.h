#ifndef IMAGELIST_H
#define IMAGELIST_H

#include <QWidget>
#include <QUrl>

namespace Ui {
    class ImageList;
}

class ImageList : public QWidget
{
    Q_OBJECT
public:
    explicit ImageList(QWidget *parent = 0);
    ~ImageList();

    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);
    void closeEvent(QCloseEvent *e);

    void setListEventFilter(QObject *);

private:
    Ui::ImageList *ui;
    QList<QString> items;

signals:
    void selectionChanged(const QString &);

public slots:
    bool addItem(const QString &);
    bool addItem(const QUrl &);
    bool addItems(const QList<QUrl> &);

    void shuffle();
    void prev();
    void next();
    void removeCurrent();
};

#endif // IMAGELIST_H
