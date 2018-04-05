#include "controller.h"
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QObject>

/* counter for steps */
int step_ctr;

/* holds steps for trace line */
QStringList steps;

/* individual process trace lines */
std::queue<QString> trace;

//constructor
Controller::Controller(QObject *parent) : QObject(parent)
{

}

//std::unordered_map<my_pid_t, pcb_t> Controller::getProcesses() const
//{
//    return processes;
//}

void Controller::set_memory(kbyte_t mem_size)
{
    this->memory_size = mem_size;
}

void Controller::set_page_size(kbyte_t page_size)
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
            trace.push(line);
        }

        file.close();
    }
}

void Controller::step()
{
    if(trace.size() < 0){
        //TODO handle error
        //return -1;
    }
    else {
        //perform different steps based on size of step list
        switch(steps.size()){
        case 0:
            //parse steps into list
            steps = trace.front().split(" ");
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            //new process has arrived
            my_pid_t pid = (my_pid_t)steps.front().toInt();
            steps.pop_front();

            //create page table for process


            //create pcb for process
//            pcb_t pcb_table = {
//                0, //new state

//            };

//            //process state
//            /* 0->new 1->ready 2->running 3->waiting 4->halted */
//            ushort state;

//            //address of page table
//            page_table_t* page_table;

//        //    size_t get_tbl_len(page_table_t* table){
//        //        return table->size();
//        //    }

//            //length of page table
//            kbyte_t table_length;

//            //page table length
//            //kbyte_t table_length = page_table->size();


//            /* text segment */
//            //length of text segment
//            kbyte_t txt_seg_length;
//            //start of text segment
//            frame_t txt_start_idx;
//            //end of text segment
//            frame_t txt_end_idx;

//            /* data segment */
//            //length of data segment
//            kbyte_t data_seg_length;
//            //start of text segment
//            frame_t data_start_idx;
//            //end of text segment
//            frame_t data_end_idx;

            break;
        default:
            break;
        }







    }
}
