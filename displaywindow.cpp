#include <QCloseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include "displaywindow.hpp"

DisplayWindow::DisplayWindow(QWidget *parent) : QMainWindow(parent)
{
    is_running = true;
    resize(64 * 8, 32 * 8);
    show();
}

bool DisplayWindow::running()
{
    return is_running;
}

void DisplayWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
    is_running = false;
}

void DisplayWindow::paintEvent(QPaintEvent *event)
{
    event->accept();
    QPainter painter(this);

    if (image.isNull())
        return;
    painter.drawPixmap(0, 0, QPixmap::fromImage(image.scaled(64 * 8, 32 * 8)));
}

void DisplayWindow::render(uint8_t *framebuffer)
{
    image = QImage(framebuffer, 64, 32, QImage::Format_RGBA8888);
    update();
}
