#ifndef MY_TYPES_H
#define MY_TYPES_H
#include <list>
#include <unordered_map>

/* process id type for convenience and just cause */
typedef uint my_pid_t;

/* type for kilobytes */
typedef ulong kbyte_t;

/* type for frame index and counting */
typedef ulong frame_t;

/* type for page table */
typedef std::list<kbyte_t> page_table_t;

typedef ulong table_ptr;

/* PCB struct for each process */
typedef struct {
    //process state
    /* 0->new 1->ready 2->running 3->waiting 4->halted */
    ushort state;

    //address of page table
    page_table_t* page_table;

//    size_t get_tbl_len(page_table_t* table){
//        return table->size();
//    }

    //length of page table
    kbyte_t table_length;

    //page table length
    //kbyte_t table_length = page_table->size();


    /* text segment */
    //length of text segment
    kbyte_t txt_seg_length;
    //start of text segment
    frame_t txt_start_idx;
    //end of text segment
    frame_t txt_end_idx;

    /* data segment */
    //length of data segment
    kbyte_t data_seg_length;
    //start of text segment
    frame_t data_start_idx;
    //end of text segment
    frame_t data_end_idx;

} pcb_t;

/* list of pcb's */
typedef std::unordered_map<my_pid_t, pcb_t>* pcb_list;

#endif // MY_TYPES_H
