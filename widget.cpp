#include "widget.h"
#include "ui_widget.h"
#include <utility>

my_pid_t last_pid;

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

void Widget::add_frames(std::vector< std::pair< frame_t, byte_t > > frames){
    for(int i = 0; i < frames.size(); i++){
        //get pair from vector
        std::pair<frame_t, byte_t> pr = frames[i];

        //set labels accordingly
        //pid
        ui->memory_widget->setItem(pr.first, 0, new QTableWidgetItem(QString::number(last_pid)));
        //page#
        //TODO ui->memory_widget->setItem(pr.first, 1, new QTableWidgetItem("page#"));
        //value
        ui->memory_widget->setItem(pr.first, 2, new QTableWidgetItem(QString::number(pr.second)));
        //type
        //TODO ui->memory_widget->setItem(pr.first, 3, new QTableWidgetItem("Hello"));
    }
}

void Widget::remove_frames(std::vector<frame_t> indexes){

}

void Widget::set_num_frames(frame_t num_frames){
    //set proper row count
    ui->memory_widget->setRowCount(num_frames);

    //set row headers
    for(frame_t i = 0; i < num_frames; i++){
        QString f = "Frame ";
        //QString n = QString::number()
        f.append(QString::number(i));
        ui->memory_widget->setVerticalHeaderItem(i, new QTableWidgetItem(f));
    }
}

void Widget::new_process(my_pid_t pid)
{
    last_pid = pid;
    QString new_message = "New Process: ";
    QString pid_str = QString::number(pid);
    new_message.append(pid_str);
    ui->status_lbl->setText(new_message);
}

void Widget::halt_process(my_pid_t pid)
{
    last_pid = pid;
    QString halt_message = "Process ";
    QString pid_str = QString::number(pid);
    halt_message.append(pid_str);
    halt_message.append(" Halted");
    ui->status_lbl->setText(halt_message);
}

void Widget::get_pcb_list(pcb_list *pcbs)
{

}

void Widget::memory_full()
{
    //TODO make this better later
    ui->status_lbl->setText("Memory full!");
}

void Widget::finished()
{

}

void Widget::on_start_button_clicked()
{
    //convert kbytes to bytes and send memory size to controller
    emit sig_set_memory((ui->memory_size_box->currentText().toULong()) * 1024);

    //send page size to controller
    emit sig_set_page_size(ui->page_size_box->currentText().toULong());

    //read selected trace tape
    emit sig_read_trace(ui->trace_combo_box->currentText());

    //send start signal to start controller steps
    emit sig_start();
}

void Widget::on_reset_button_clicked()
{
    emit sig_reset();
}
