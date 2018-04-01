#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QString>
#include <QStringList>


class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);
    void set_widget(QWidget *widget);

private:
    QWidget *widget;
    int memory_size;
    int page_size;
    std::vector frames;
    /* PCB struct for each process */
    struct pcb {
        //address of page table
        void* page_table;
        //page table length
        int table_length;
        //data segment length
        int data_seg_length;
        //text segment length
        int txt_seg_length;
    };
    /* process id type for convenience */
    typedef int pid;
    /* map of PCBs */
    std::map<pid, pcb> processes;
    /* vector to hold individual process traces */
    std::vector<QString> trace;

    void setup_signals();

signals:
    void sig_add_page(int index, QString val);
    void sig_remove_page(int index);
    void sig_set_frame_number(int num_frames);

public slots:
    void set_memory(int mem_size);
    void set_page_size(int page_size);
    //TODO option input later http://www.cplusplus.com/forum/general/5098/
    /* reads trace tape from a file */
    void read_trace(QString filename);
    void step();
};

#endif // CONTROLLER_H
