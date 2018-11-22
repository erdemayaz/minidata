#ifndef _TASK_H_
#define _TASK_H_

void task_close(int notify);

void task_database(char* name);

void task_create();

void task_drop(char* name);

void task_entity(char* name);

void perform(command* c);

#endif