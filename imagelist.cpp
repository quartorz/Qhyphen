#include "imagelist.h"
#include "ui_imagelist.h"

#include <QMimeData>
#include <QCloseEvent>
#include <QFileInfo>

#include <algorithm>
#include <numeric>
#include <functional>
#include <random>

namespace {
    static const char *extensions[] = {
        "bmp", "gif", "png", "jpg", "jpeg"
    };
}

bool hasValidExtension(const QFileInfo &file)
{
    auto ext = file.suffix().toLower();
    auto iter = std::find(std::begin(extensions), std::end(extensions), ext);

    return iter != std::end(extensions);
}

ImageList::ImageList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageList())
{
    ui->setupUi(this);

    connect(ui->listWidget, &QListWidget::itemSelectionChanged, [&]() {
        int row = ui->listWidget->currentRow();

        if (row >= 0) {
            emit selectionChanged(items[ui->listWidget->currentRow()]);
        } else {
            emit selectionChanged("");
        }
    });
}

ImageList::~ImageList()
{
    delete ui;
}

void ImageList::closeEvent(QCloseEvent *)
{
    //e->ignore();
    //hide();
}

void ImageList::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->urls()[0].isLocalFile()) {
        e->acceptProposedAction();
    }
}

void ImageList::dropEvent(QDropEvent *e)
{
    e->accept();
    addItems(e->mimeData()->urls());
}

void ImageList::setListEventFilter(QObject *obj)
{
    ui->listWidget->installEventFilter(obj);
}

bool ImageList::addItem(const QString &item)
{
    QFileInfo file(item);

    if (!hasValidExtension(file) || !file.exists()) {
        return false;
    }

    auto iter = std::find(items.begin(), items.end(), item);

    if (iter != items.end()) {
        return false;
    }

    items.append(item);
    ui->listWidget->addItem(QFileInfo(item).fileName());

    return true;
}

bool ImageList::addItem(const QUrl &item)
{
    return addItem(item.toLocalFile());
}

bool ImageList::addItems(const QList<QUrl> &items)
{
    //auto f = std::accumulate(
    //            items.begin(), items.end(),
    //            false,
    //            [&](bool f, const QUrl &i) { return f || addItem(i); });

    bool f = false;

    for (auto &i : items) {
        f = f || addItem(i);
    }

    if (f) {
//        ui->listWidget->setCurrentRow(this->items.size() - 1);
    }

    return f;
}

void ImageList::shuffle()
{
    if (items.size() == 0) {
        return;
    }

    int currentIndex = ui->listWidget->currentRow();

    std::shuffle(items.begin(), items.end(), std::mt19937_64{std::random_device{}()});
    ui->listWidget->clear();

    for (auto &i : items) {
        ui->listWidget->addItem(QFileInfo(i).fileName());
    }

    ui->listWidget->setCurrentRow(currentIndex);
}

void ImageList::prev()
{
    if (items.size() == 0) {
        return;
    }

    auto idx = ui->listWidget->currentRow();

    if (idx == 0) {
        idx = items.size() - 1;
    } else {
        --idx;
    }

    ui->listWidget->setCurrentRow(idx);
}

void ImageList::next()
{
    if (items.size() == 0) {
        return;
    }

    auto idx = ui->listWidget->currentRow();

    if (idx == items.size() - 1) {
        idx = 0;
    } else {
        ++idx;
    }

    ui->listWidget->setCurrentRow(idx);
}

void ImageList::removeCurrent()
{
    if (items.size() == 0) {
        return;
    }

    auto idx = ui->listWidget->currentRow();

    items.removeAt(idx);
    ui->listWidget->takeItem(idx);
}
