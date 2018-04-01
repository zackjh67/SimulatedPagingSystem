#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_stepButton_clicked();
    void add_page(int);
    void remove_page(int);
    void set_frames(int);

    void on_loadButton_clicked();

signals:
    void sig_set_memory(int bytes);
    void sig_set_page_size(int bytes);
    //TODO option input later http://www.cplusplus.com/forum/general/5098/
    void sig_read_trace(QString filename);
    void sig_step();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
