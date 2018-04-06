#include "widget.h"
#include "controller.h"
#include <QApplication>
#include <my_types.h>
#include <iostream>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    Controller c;

    /* setup signals */
    //signals to GUI
    QObject::connect(&c, SIGNAL(sig_new_process(my_pid_t)), &w, SLOT(new_process(my_pid_t)));
    QObject::connect(&c, SIGNAL(sig_halt_process(my_pid_t)), &w, SLOT(halt_process(my_pid_t)));
    QObject::connect(&c, SIGNAL(sig_set_num_frames(frame_t)), &w, SLOT(set_num_frames(frame_t)));
    QObject::connect(&c, SIGNAL(sig_add_frames(std::vector<std::pair<frame_t,byte_t> >)), &w, SLOT(add_frames(std::vector<std::pair<frame_t,byte_t> >)));
    QObject::connect(&c, SIGNAL(sig_remove_frames(std::vector<frame_t>)), &w, SLOT(remove_frames(std::vector<frame_t>)));
    QObject::connect(&c, SIGNAL(sig_memory_full()), &w, SLOT(memory_full()));
    QObject::connect(&c, SIGNAL(sig_finished()), &w, SLOT(finished()));
    QObject::connect(&c,
                     SIGNAL(sig_create_page_table(my_pid_t,std::vector<std::tuple<frame_t,frame_t,QString> >)),
                     &w,
                     SLOT(create_page_table(my_pid_t,std::vector<std::tuple<frame_t,frame_t,QString> >)));


    //signals from GUI
    QObject::connect(&w, SIGNAL(sig_read_trace(QString)), &c, SLOT(read_trace(QString)));
    QObject::connect(&w, SIGNAL(sig_set_memory(byte_t)), &c, SLOT(set_memory(byte_t)));
    QObject::connect(&w, SIGNAL(sig_set_page_size(byte_t)), &c, SLOT(set_page_size(byte_t)));
    QObject::connect(&w, SIGNAL(sig_step()), &c, SLOT(step()));
    QObject::connect(&w, SIGNAL(sig_start()), &c, SLOT(start()));
    QObject::connect(&w, SIGNAL(sig_reset()), &c, SLOT(reset()));

    w.show();

    return a.exec();
    return 0;
}
