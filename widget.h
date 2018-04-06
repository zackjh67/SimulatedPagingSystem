#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include "my_types.h"
#include <queue>

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
    //slots that are signaled
    void create_page_table(my_pid_t pid, std::vector<std::tuple<frame_t, frame_t, QString> > table);
    void add_frames(std::vector< std::tuple<frame_t, my_pid_t, frame_t, byte_t, QString> > frames);
    void remove_frames(std::vector<frame_t> indexes);
    void set_num_frames(frame_t num_frames);
    void new_process(my_pid_t pid);
    void halt_process(my_pid_t pid);
    void memory_full();
    void finished();
    void populate_trace(std::queue<QString> trace);


    //event slots
    void on_step_button_clicked();

    void on_start_button_clicked();

    void on_reset_button_clicked();

signals:
    void sig_set_memory(byte_t k_bytes);
    void sig_set_page_size(byte_t k_bytes);
    //TODO option input later http://www.cplusplus.com/forum/general/5098/
    void sig_read_trace(QString filename);
    void sig_step();
    void sig_start();
    void sig_reset();

private:
    Ui::Widget *ui;

    /* GUI's reference to pcb list */
    pcb_list pcbs;
};

#endif // WIDGET_H
