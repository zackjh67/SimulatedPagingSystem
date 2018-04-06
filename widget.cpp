#include "widget.h"
#include "ui_widget.h"
#include <utility>
#include <tuple>

my_pid_t last_pid;
int step_ctr = 1;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //hide status label
    ui->status_lbl->hide();
    ui->reset_button->hide();//doesn't quite work yet
    ui->reset_button->setEnabled(false); //disabled unil finished
    ui->step_button->setEnabled(false); //disabled until started
}

Widget::~Widget()
{
    delete ui;
}

void Widget::create_page_table(my_pid_t pid, std::vector<std::tuple<frame_t, frame_t, QString> > table)
{
    //set pid label accordingly
    ui->page_tbl_lbl->setText("Page Table PID: " + QString::number(pid));

    //set page table rows
    ui->page_tbl_widget->setRowCount((int)table.size());

    for(frame_t i = 0; i < table.size(); i++){
        //get tuple
        std::tuple<frame_t, frame_t, QString> tup = table[i];

        //set labels
        ui->page_tbl_widget->setItem(i, 0, new QTableWidgetItem(QString::number(std::get<0>(tup))));
        ui->page_tbl_widget->setItem(i, 1, new QTableWidgetItem(QString::number(std::get<1>(tup))));
        ui->page_tbl_widget->setItem(i, 2, new QTableWidgetItem(std::get<2>(tup)));
    }
}

void Widget::on_step_button_clicked()
{
    //delect any existing frames
    ui->memory_widget->clearSelection();
    ui->page_tbl_widget->clearFocus();

    emit sig_step();

    //highlight current step
    if (ui->trace_list->count() > 0) {
      ui->trace_list->item(step_ctr)->setSelected(true);
    }
    ui->trace_list->setFocus();


    step_ctr++;
}

void Widget::add_frames(std::vector< std::tuple<frame_t, my_pid_t, frame_t, byte_t, QString> > frames){
    for(int i = 0; i < frames.size(); i++){
        //set labels accordingly
        //pid
        ui->memory_widget->setItem(std::get<0>(frames[i]), 0, new QTableWidgetItem(QString::number(std::get<1>(frames[i]))));
        //page#
        ui->memory_widget->setItem(std::get<0>(frames[i]), 1, new QTableWidgetItem(QString::number(std::get<2>(frames[i]))));
        //value
        ui->memory_widget->setItem(std::get<0>(frames[i]), 2, new QTableWidgetItem(QString::number(std::get<3>(frames[i]))));
        //type
        ui->memory_widget->setItem(std::get<0>(frames[i]), 3, new QTableWidgetItem(std::get<4>(frames[i])));

        //highlight frame accordingly
        ui->memory_widget->item(std::get<0>(frames[i]), 0)->setSelected(true);
        ui->memory_widget->item(std::get<0>(frames[i]), 1)->setSelected(true);
        ui->memory_widget->item(std::get<0>(frames[i]), 2)->setSelected(true);
        ui->memory_widget->item(std::get<0>(frames[i]), 3)->setSelected(true);

    }
            ui->memory_widget->setFocus();
}

void Widget::remove_frames(std::vector<frame_t> indexes){
    for(frame_t i = 0; i < indexes.size(); i++){
        ui->memory_widget->setItem(indexes[i], 0, new QTableWidgetItem(""));
        ui->memory_widget->setItem(indexes[i], 1, new QTableWidgetItem(""));
        ui->memory_widget->setItem(indexes[i], 2, new QTableWidgetItem(""));
        ui->memory_widget->setItem(indexes[i], 3, new QTableWidgetItem(""));
    }
}

void Widget::set_num_frames(frame_t num_frames){
    //set proper row count
    ui->memory_widget->setRowCount(num_frames);

    //set row headers
    for(frame_t i = 0; i < num_frames; i++){
        QString f = "Frame ";
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
    ui->status_lbl->show();
}

void Widget::halt_process(my_pid_t pid)
{
    last_pid = pid;
    QString halt_message = "Process ";
    QString pid_str = QString::number(pid);
    halt_message.append(pid_str);
    halt_message.append(" Halted");
    ui->status_lbl->setText(halt_message);
    ui->status_lbl->show();
}

void Widget::memory_full()
{
    //TODO make this better later
    ui->status_lbl->setText("Memory full!");
    ui->status_lbl->show();
}

void Widget::finished()
{
    ui->status_lbl->setText("Trace Finished");
    ui->start_button->setEnabled(false);
    ui->step_button->setEnabled(false);
    ui->reset_button->setEnabled(true);
}

void Widget::populate_trace(std::queue<QString> trace)
{
    //populate trace tape list
    while(trace.size() != 0){
        ui->trace_list->addItem(trace.front());
        trace.pop();
    }

    //highlight first step
    if (ui->trace_list->count() > 0) {
      ui->trace_list->item(0)->setSelected(true);
    }
    ui->trace_list->setFocus();
}

void Widget::on_start_button_clicked()
{
    //disable start and reset button
    ui->start_button->setEnabled(false);
    ui->reset_button->setEnabled(false);
    //enable step button
    ui->step_button->setEnabled(true);
    //disable filename, memory, and page size changes
    ui->trace_combo_box->setEnabled(false);
    ui->memory_size_box->setEnabled(false);
    ui->page_size_box->setEnabled(false);

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
    ui->reset_button->setEnabled(false);
    ui->memory_size_box->setEnabled(true);
    ui->page_size_box->setEnabled(true);
    ui->trace_combo_box->setEnabled(true);
    ui->start_button->setEnabled(true);

    //clear contents
    ui->trace_list->clear();
    ui->memory_widget->clearContents();
    ui->page_tbl_widget->clearContents();

    emit sig_reset();
}
