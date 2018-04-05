#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_step_button_clicked()
{
    emit sig_step();
}

void Widget::add_frames(frame_t indexes[], kbyte_t vals[]){

}

void Widget::remove_frames(frame_t indexes[]){

}

void Widget::set_num_frames(frame_t num_frames){

}

void Widget::new_process(my_pid_t pid)
{

}

void Widget::halt_process(my_pid_t pid)
{

}

void Widget::get_pcb_list(pcb_list *pcbs)
{

}



void Widget::on_load_button_clicked()
{
    //read selected trace tape
    emit sig_read_trace(ui->trace_combo_box->currentText());
}
