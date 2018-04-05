#include "widget.h"
#include "controller.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    Controller c;

    /* setup signals */
    //signals to GUI
    QObject::connect(&c, SIGNAL(sig_pcb_list(pcb_list*)), &w, SLOT(get_pcb_list(pcb_list*)));
    QObject::connect(&c, SIGNAL(sig_new_process(my_pid_t)), &w, SLOT(new_process(my_pid_t)));
    QObject::connect(&c, SIGNAL(sig_halt_process(my_pid_t)), &w, SLOT(halt_process(my_pid_t)));
    QObject::connect(&c, SIGNAL(sig_set_num_frames(frame_t)), &w, SLOT(set_num_frames(frame_t)));
    QObject::connect(&c, SIGNAL(sig_add_frames(frame_t[],kbyte_t[])), &w, SLOT(add_frames(frame_t[],kbyte_t[])));
    QObject::connect(&c, SIGNAL(sig_remove_frames(frame_t[])), &w, SLOT(remove_frames(frame_t[])));


    //signals from GUI
    QObject::connect(&w, SIGNAL(sig_read_trace(QString)), &c, SLOT(read_trace(QString)));
    QObject::connect(&w, SIGNAL(sig_set_memory(kbyte_t)), &c, SLOT(set_memory(kbyte_t)));
    QObject::connect(&w, SIGNAL(sig_set_page_size(kbyte_t)), &c, SLOT(set_page_size(kbyte_t)));
    QObject::connect(&w, SIGNAL(sig_step()), &c, SLOT(step()));

    w.show();

    return a.exec();
}
