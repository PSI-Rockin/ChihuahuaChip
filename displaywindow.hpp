#ifndef DISPLAYWINDOW_HPP
#define DISPLAYWINDOW_HPP

#include <QMainWindow>
#include <QImage>

class DisplayWindow : public QMainWindow
{
    Q_OBJECT
    private:
        bool is_running;
        QImage image;
    public:
        explicit DisplayWindow(QWidget *parent = nullptr);

        bool running();

        void paintEvent(QPaintEvent *event) override;
        void closeEvent(QCloseEvent *event) override;
        void render(uint8_t* framebuffer);
    signals:

    public slots:
};

#endif // DISPLAYWINDOW_HPP
