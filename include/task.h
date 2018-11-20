#ifndef _TASK_H_
#define _TASK_H_

void task_close();

void task_database(char* name);

void task_create();

void task_drop(char* name);

void perform(command* c);

#endif