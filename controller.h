#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <unordered_map>
#include <queue>
#include <list>
#include "my_types.h"


class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);

    //std::unordered_map<my_pid_t, pcb_t> getProcesses() const;

private:
    /* total size of memory in KBytes */
    kbyte_t memory_size;

    /* total page size in KBytes */
    kbyte_t page_size;

    /* array of physical memory frames */
    frame_t * frameArr;

    /* queue of available frames in memory */
    std::queue<frame_t> availableFrames;

    /* map of PCBs */
    pcb_list pcbs;

signals:
    //TODO make sent pcb_map (processes), page table, & frameArr read only
    //http://www.cplusplus.com/forum/articles/36872/

    /* pass process data to GUI */
    void sig_pcb_list(pcb_list* pcbs);

    /* add pages to frames in memory in GUI */
    void sig_add_frames(frame_t index[], kbyte_t vals[]);

    /* remove frames from memory in GUI */
    void sig_remove_frames(frame_t index[]);

    /* set number of frames in GUI */
    void sig_set_num_frames(frame_t num_frames);

    /* show that new process has arrived and send pcb */
    void sig_new_process(my_pid_t pid);


    //not needed prolly cuz pcb has address void sig_add_table(my_pid_t pid, page_table_t page_table);

    /* halt process in GUI */
    void sig_halt_process(my_pid_t pid);

public slots:
    /* sets memory size based on signal from GUI */
    void set_memory(kbyte_t mem_size);

    /* sets page size based on signal from GUI */
    void set_page_size(kbyte_t page_size);

    //TODO option input later http://www.cplusplus.com/forum/general/5098/
    /* reads trace tape from a file */
    void read_trace(QString filename);

    /* steps through: process started -> page table created -> pags into memory
     *              or process ended -> pages out of memory */
    void step();
};

#endif // CONTROLLER_H
