#include "controller.h"
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QObject>
#include <utility>
#include <iostream>

/*************************************************************************************
 * Controller program to manage memory and processes and update GUI accordingly.
 * @author Zachary Hern
 * @version 4/6/2018
 *
 *************************************************************************************/

/* holds steps for trace line */
QStringList steps;

/* individual process trace lines */
std::queue<QString> trace;

//constructor
Controller::Controller(QObject *parent) : QObject(parent)
{

}

/*************************************************
 * Sets memory size in bytes.
 * @param mem_size : size of memory
 *************************************************/
void Controller::set_memory(byte_t mem_size)
{
    this->memory_size = mem_size;
}

/*************************************************
 * Sets page size in bytes.
 * @param page_size : size of memory
 *************************************************/
void Controller::set_page_size(byte_t page_size)
{
    this->page_size = page_size;
}

/*************************************************
 * Reads trace tape from a file.
 * @param filename : name of file to read
 *************************************************/
void Controller::read_trace(QString filename)
{
    //hardcoded for now :(
    QString path = "C:/Users/Zack/Documents/SimulatedPagingSystem/traces/";
    path.append(filename);

    QFileInfo check_file(path);
    // check if file exists and if yes: Is it really a file and no directory?
    if (check_file.exists() && check_file.isFile()) {

        //get file from path
        QFile file(path);


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
        //send trace to GUI
        emit sig_send_trace(trace);
    }
}

/*************************************************
 * Starts trace tape read process.
 * Signaled by GUI
 *************************************************/
void Controller::start()
{

    if(memory_size == 0 || page_size == 0){
        //TODO handle error
    } else {
        frame_t calc_frames = memory_size / page_size;
        //TODO handle destroying this later - delete [] frameArr
        //initialize frame array to fixed size
        frame_arr.reserve((calc_frames));

        //initialize frames to 0s
        for(frame_t i = 0; i < calc_frames; i++){
            frame_arr.push_back(0);

            //add free frames to free frame list
            free_frames.push(i);
        }
    }

    //signal GUI to create physical memory table
    emit sig_set_num_frames((frame_t)frame_arr.size());

    //perform first step
    this->step();
}

/*************************************************
 * Resets controller program.
 * Signaled by GUI
 *************************************************/
void Controller::reset()
{
    //reset sizes
    memory_size, page_size = 0;
    //empty frames
    frame_arr.clear();
    frame_arr.shrink_to_fit(); //C++11

    //empty free frame list
    if(!free_frames.empty()){
        std::queue<frame_t> empty;
        free_frames.swap(empty);
    }

    //empty pcb list
    pcbs.clear();

    //clear steps
    steps.clear();

    //empty trace Q
    if(!trace.empty()){
        std::queue<QString> empty;
        trace.swap(empty);
    }
}

/*************************************************
 * Steps through trace
 * Signaled by GUI
 *************************************************/
void Controller::step()
{
    if(trace.size() < 0){
        //TODO handle error
        //return -1;
    } else if(trace.size() == 0){
        //stepping finished
        emit sig_finished();
    }
    else {
        //perform differen't ops based on current_step and steps size

            //parse steps into list
            steps = trace.front().split(" ");
            trace.pop();

            //get pid
            my_pid_t pid = (my_pid_t)steps.front().toUInt();

            //pid, Halt
            if(steps.size() == 2){
                //send signal to GUI to display halted process
                emit sig_halt_process((my_pid_t)steps.front().toUInt());
                //remove pid segment
                steps.pop_front();

                //look up pcb by pid
                pcb_t pcb = *pcbs[pid];

                //change process state
                pcb.state = 4; //halted

                //get page table
                page_table_t table = *pcb.page_table;

                //indexes to pass to GUI
                std::vector<frame_t> indexes;

                for(frame_t i = 0; i < table.size(); i++){
                    //look up frames in table
                    frame_t frame = table.at(i);

                    //add frame to free frame list
                    free_frames.push(frame);

                    //add address to indexes[]
                    indexes.push_back(frame);
                }
                //clear addresses from page table
                table.clear();

                //send signal to remove frames from GUI
                emit sig_remove_frames(indexes);

                //clear steps
                steps.pop_front();
            }
            //pid, text seg, data seg
            else if(steps.size() == 3){
                //send signal to GUI to display new process
                emit sig_new_process((my_pid_t)steps.front().toUInt());
                //remove pid segment
                steps.pop_front();

                //create page table for process
                //ProcID, TextSize, DataSize (in Bytes!)

                //new page table on heap
                page_table_t* page_table = new page_table_t;

                //get text segment size
                byte_t txt_seg_length = (byte_t)steps.front().toULong();
                //calculate pages needed
                frame_t num_txt_pages = (txt_seg_length / page_size);
                if(txt_seg_length % page_size > 0){
                    num_txt_pages++;
                }
                //remove from queue
                steps.pop_front();

                //get text segment size
                byte_t data_seg_length = (byte_t)steps.front().toULong();
                //calculate pages needed
                frame_t num_data_pages = (data_seg_length / page_size);
                if(data_seg_length % page_size > 0){
                    num_data_pages++;
                }
                //remove from queue
                steps.pop_front();

                //create pcb for process on heap w/ some default values
                pcb_t* pcb = new pcb_t {
                    0,                                  //new state
                    page_table,                        //address of page table
                    num_txt_pages + num_data_pages,     //total pages needed
                    txt_seg_length + data_seg_length,   //total bytes of data
                    txt_seg_length,                     //size of txt segment
                    0,                                  //start of txt segment always 0 in this implementation
                    0,                                  //temp val txt end index
                    data_seg_length,                    //size of data segment
                    0,                                  //temp val data start index
                    0                                   //temp val data end index
                };

                /* populate the page table */
                if(num_txt_pages > 0 && num_data_pages > 0){
                    //if enough physical memory for process
                    if(pcb->num_pages <= free_frames.size()){

                        //vector to pass back to GUI for rendering page table
                        std::vector< std::tuple<frame_t, frame_t, QString > > gui_table;

                        //vector of frames to send to GUI
                        std::vector< std::tuple<frame_t, my_pid_t, frame_t, byte_t, QString> > added_frames;

                        /* text segment */
                        //loop through pages and add to memory and page table
                        byte_t remaining_bytes= txt_seg_length;
                        for(frame_t i = 0; i < num_txt_pages; i++){
                            //place page value (bytes) into physical memory
                            frame_t phys_address = free_frames.front();
                            if(remaining_bytes >= page_size){
                                frame_arr[phys_address] = page_size;
                                //add to passed frames
                                added_frames.push_back(std::make_tuple(phys_address, pid, i, page_size, "Text"));
                            } else {
                                frame_arr[phys_address] = remaining_bytes;
                                //add to passed frames
                                added_frames.push_back(std::make_tuple(phys_address, pid, i, remaining_bytes, "Text"));
                            }
                            remaining_bytes -= page_size;

                            //add memory address to page table
                            page_table->push_back(phys_address);

                            //add to GUI page table
                            std::tuple <frame_t, frame_t, QString> tup = std::make_tuple(i, phys_address, "Text");
                            gui_table.push_back(tup);

                            //remove page from free frame list
                            free_frames.pop();
                        }
                        //set txt segment end index
                        pcb->txt_end_idx = num_txt_pages - 1;

                        //set data segment start index
                        pcb->data_start_idx = num_txt_pages;

                        /* data segment */
                        //loop through pages and add to memory and page table
                        remaining_bytes= data_seg_length;
                        for(frame_t i = 0; i < num_data_pages; i++){
                            //place page value (bytes) into physical memory
                            frame_t phys_address = free_frames.front();
                            if(remaining_bytes >= page_size){
                                frame_arr[phys_address] = page_size;
                                //add to passed frames
                                added_frames.push_back(std::make_tuple(phys_address, pid, i, page_size, "Data"));
                            } else {
                                frame_arr[phys_address] = remaining_bytes;
                                //add to passed frames
                                added_frames.push_back(std::make_tuple(phys_address, pid, i, remaining_bytes, "Data"));
                            }
                            remaining_bytes -= page_size;

                            //add memory address to page table
                            page_table->push_back(phys_address);

                            //add to GUI page table
                            std::tuple <frame_t, frame_t, QString> tup = std::make_tuple(i, phys_address, "Data");
                            gui_table.push_back(tup);

                            //add to GUI frame table
                            emit sig_add_frames(added_frames);

                            //remove page from free frame list
                            free_frames.pop();
                        }

                        //set txt segment end index- also could be size()-1, but done this way for future implementation
                        pcb->data_end_idx = pcb->data_start_idx + num_data_pages - 1;

                        //process in memory so it is running
                        pcb->state = 3;

                        //render page table in GUI
                        emit sig_create_page_table(pid, gui_table);

                    } else {
                        //send out of memory error to GUI
                        emit sig_memory_full();
                    }
                } else {
                    //we have a problem
                }

                //add pcb to list
                pcbs.insert(std::make_pair(pid, pcb));
            }
            //stepping finished
            if(trace.size() == 0)
            emit sig_finished();
    }
}
