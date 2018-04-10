#ifndef MY_TYPES_H
#define MY_TYPES_H
#include <vector>
#include <unordered_map>

/*************************************************************************************
 * Custom types used by simulated paging system
 * @author Zachary Hern
 * @version 4/6/2018
 *
 *************************************************************************************/

/* process id type for convenience and just cause */
typedef unsigned int my_pid_t;

/* type for bytes */
typedef unsigned long long byte_t;

/* type for frame index and counting */
typedef unsigned long frame_t;

/* type for page table */
typedef std::vector<frame_t> page_table_t;

/* pointer to segment end and start points in page table */
typedef unsigned long table_ptr;

/* PCB struct for each process */
typedef struct {
    //process state
    /* 0->new 1->ready 2->running 3->waiting 4->halted */
    unsigned short state;

    //pointer to a page table
    page_table_t* page_table;

    //total number of pages
    frame_t num_pages;

    //length of page table
    byte_t table_length;

    /* text segment */
    //length of text segment
    byte_t txt_seg_length;
    //start of text segment
    table_ptr txt_start_idx;
    //end of text segment
    table_ptr txt_end_idx;

    /* data segment */
    //length of data segment
    byte_t data_seg_length;
    //start of text segment
    table_ptr data_start_idx;
    //end of text segment
    table_ptr data_end_idx;

} pcb_t;

/* list of pcb's */
typedef std::unordered_map<my_pid_t, pcb_t*> pcb_list;

#endif // MY_TYPES_H
