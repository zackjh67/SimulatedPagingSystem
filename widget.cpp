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

void Widget::on_stepButton_clicked()
{

}

void Widget::add_page(int num_pages){

}

void Widget::remove_page(int num_pages){

}

void Widget::set_frames(int num_frames){

}



void Widget::on_loadButton_clicked()
{
    //read selected trace tape
    emit sig_read_trace(ui->traceComboBox->currentText());
}
