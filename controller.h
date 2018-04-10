#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <unordered_map>
#include <queue>
#include <list>
#include <vector>
#include "my_types.h"

/*************************************************************************************
 * Controller program to manage memory and processes and update GUI accordingly.
 * @author Zachary Hern
 * @version 4/6/2018
 *
 *************************************************************************************/
class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);

private:
    /* total size of memory in KBytes */
    byte_t memory_size;

    /* total page size in KBytes */
    byte_t page_size;

    /* physical memory frames */
    std::vector<byte_t> frame_arr;

    /* queue of available frames in memory */
    std::queue<frame_t> free_frames;

    /* map of PCBs */
    pcb_list pcbs;

signals:
    /* create page table in GUI */
    void sig_create_page_table(my_pid_t, std::vector< std::tuple<frame_t, frame_t, QString > >);

    /* add pages to frames in memory in GUI */
    void sig_add_frames(std::vector< std::tuple<frame_t, my_pid_t, frame_t, byte_t, QString> > frames);

    /* remove frames from memory in GUI */
    void sig_remove_frames(std::vector<frame_t> indexes);

    /* set number of frames in GUI */
    void sig_set_num_frames(frame_t num_frames);

    /* show that new process has arrived and send pcb */
    void sig_new_process(my_pid_t pid);

    /* signal GUI to display out of memory popup */
    void sig_memory_full();

    /* signals GUI when trace tape has been fully stepped through */
    void sig_finished();

    /* halt process in GUI */
    void sig_halt_process(my_pid_t pid);

    /* Sends trace file to GUI */
    void sig_send_trace(std::queue<QString> trace);

public slots:
    /* sets memory size based on signal from GUI */
    void set_memory(byte_t mem_size);

    /* sets page size based on signal from GUI */
    void set_page_size(byte_t page_size);

    //TODO optional param later http://www.cplusplus.com/forum/general/5098/
    /* reads trace tape from a file */
    void read_trace(QString filename);

    /* starts mem operations */
    void start();

    /* resets program */
    void reset();

    /* steps through: process started -> page table created -> pags into memory
     *              or process ended -> pages out of memory */
    void step();
};

#endif // CONTROLLER_H
