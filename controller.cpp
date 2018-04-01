#include "controller.h"
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

//constructor
Controller::Controller(QObject *parent) : QObject(parent)
{
    setup_signals();
}

void Controller::set_widget(QWidget *widget)
{
    this->widget = widget;
}

void Controller::setup_signals()
{
    //signals to GUI
    connect(this, SIGNAL(sig_add_page(int)), widget, SLOT(add_page(int)));
    connect(this, SIGNAL(sig_remove_page(int)), widget, SLOT(remove_page(int)));

    //signals from GUI
    connect(widget, SIGNAL(sig_read_trace(QString)), this, SLOT(read_trace(QString)));
    connect(widget, SIGNAL(sig_set_memory(int)), this, SLOT(set_memory(int));
    connect(widget, SIGNAL(sig_set_page_size(int)), this, SLOT(set_page_size(int)));
    connect(widget, SIGNAL(sig_step()), this, SLOT(step()));
}

void Controller::set_memory(int mem_size)
{
    this->memory_size = mem_size;
}

void Controller::set_page_size(int page_size)
{
    this->page_size = page_size;
}

void Controller::read_trace(QString filename)
{
    QString path = "./traces/";
    path.append(filename);

    QFileInfo check_file(path);
    // check if file exists and if yes: Is it really a file and no directory?
    if (check_file.exists() && check_file.isFile()) {

        //get file from path
        QFile file(path);

        //TODO error check later
        if(!file.open(QIODevice::ReadOnly)) {
            //QMessageBox::information(0, "error", file.errorString());
        }

        QTextStream in(&file);

        while(!in.atEnd()) {
            QString line = in.readLine();
            //data seperated by spaces
            //use this later DO NOT DELETE QStringList fields = line.split(" ");
            //add invidual process traces to vector
            trace.push_back(line);
        }

        file.close();
    }
}

void Controller::step()
{
    if(trace.size() < 0){
        //TODO handle error
        return -1;
    }
    else {

    }
}
