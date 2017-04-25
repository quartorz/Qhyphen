#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "imagelist.h"
#include "plaintextdialog.h"

#include <QMovie>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QPainter>
#include <QMimeData>
#include <QPlainTextEdit>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    imageList(new ImageList(this)),
    ui(new Ui::MainWindow),
    movie(new QMovie(this))
{
    ui->setupUi(this);
    ui->listContainer->addWidget(imageList);
    ui->splitter->setSizes(QList<int>{} << width() * 8 / 10 << width() * 2 / 10);

    installEventFilter(this);
    imageList->setListEventFilter(this);

    connect(movie, &QMovie::frameChanged,
            [&](int){ repaint(); });
    connect(imageList, &ImageList::selectionChanged,
            this, &MainWindow::setFileName);

    connect(ui->prevButton, &QPushButton::clicked, imageList, &ImageList::prev);
    connect(ui->nextButton, &QPushButton::clicked, imageList, &ImageList::next);
}

MainWindow::~MainWindow()
{
    delete imageList, ui, movie;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        auto e = static_cast<QKeyEvent*>(event);

        switch (e->key()) {
        case Qt::Key_S:
            imageList->shuffle();
            return true;
        case Qt::Key_L:
            imageList->show();
            return true;
        case Qt::Key_Up:
        case Qt::Key_Left:
        case Qt::Key_Comma:
            imageList->prev();
            return true;
        case Qt::Key_Down:
        case Qt::Key_Right:
        case Qt::Key_Period:
            imageList->next();
            return true;
        case Qt::Key_T:
            inputAsText();
            return true;
        case Qt::Key_Delete:
            imageList->removeCurrent();
            return true;
        }
    }

    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    repaint();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->urls()[0].isLocalFile()) {
        e->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *e)
{
    e->accept();
    imageList->addItems(e->mimeData()->urls());
}

void MainWindow::paintEvent(QPaintEvent *)
{
    if (!movie->isValid()) {
        return;
    }

    auto size = this->size();
    auto picSize = movie->currentPixmap().size();

    size.setWidth(ui->imageContainer->contentsRect().width());

    QRect rect;

    if (double(size.width()) / size.height() > double(picSize.width()) / picSize.height()) {
        double ratio = double(size.height()) / picSize.height();
        rect.setSize(QSize(picSize.width() * ratio, size.height()));
        rect.moveLeft((size.width() - rect.width()) / 2);
    } else {
        double ratio = double(size.width()) / picSize.width();
        rect.setSize(QSize(size.width(), picSize.height() * ratio));
        rect.moveTop((size.height() - rect.height()) / 2);
    }

    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform);
    painter.drawPixmap(rect, movie->currentPixmap());
}

void MainWindow::setFileName(const QString &name)
{
    if (movie->fileName() != name) {
        movie->stop();
        movie->setFileName(name);
        movie->start();
    }
}
#include <QDebug>
void MainWindow::inputAsText()
{
    PlainTextDialog dialog(this);
    dialog.exec();

    auto text = dialog.content();

    int size = text.size();
    int begin = 0;
    int end = text.indexOf('\n', 0);

    while (begin < size) {
        if (end == -1) {
            end = size;
        }

        auto line = text.midRef(begin, end - begin).trimmed();

        if (!line.isEmpty() && line[0] != '#') {
            imageList->addItem(line.toString());
        }

        begin = end + 1;
        end = text.indexOf('\n', begin);
    }
}
