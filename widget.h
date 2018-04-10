#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include "my_types.h"
#include <queue>
/*
 * Qt Widget class
 * */
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
    /* signaled by controller to create page table */
    void create_page_table(my_pid_t pid, std::vector<std::tuple<frame_t, frame_t, QString> > table);

    /* signaled by controller to add frames to memory widget */
    void add_frames(std::vector< std::tuple<frame_t, my_pid_t, frame_t, byte_t, QString> > frames);

    /* signaled by controller to remove frames from memory widget */
    void remove_frames(std::vector<frame_t> indexes);

    /* signaled by controller to set number of frames in memory */
    void set_num_frames(frame_t num_frames);

    /* signaled by controller that a new process has started */
    void new_process(my_pid_t pid);

    /* signaled by controller that a process has been stopped */
    void halt_process(my_pid_t pid);

    /* signaled by controller that memory is full */
    void memory_full();

    /* signaled by controller that program is finished */
    void finished();

    /* signaled by controller to populate trace widget */
    void populate_trace(std::queue<QString> trace);


    //event slots
    void on_step_button_clicked();

    void on_start_button_clicked();

    void on_reset_button_clicked();

signals:
    /* set memory in controller */
    void sig_set_memory(byte_t k_bytes);

    /* set page size in controller */
    void sig_set_page_size(byte_t k_bytes);

    //TODO option input later http://www.cplusplus.com/forum/general/5098/
    /* tell controller to read trace tape */
    void sig_read_trace(QString filename);

    /* step through trace */
    void sig_step();

    /* start controller */
    void sig_start();

    /* reset controller */
    void sig_reset();

private:
    Ui::Widget *ui;

    /* GUI's reference to pcb list */
    pcb_list pcbs;
};

#endif // WIDGET_H
