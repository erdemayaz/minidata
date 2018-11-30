#ifndef _TASK_H_
#define _TASK_H_

void task_close(int notify);

void task_database(char* name);

void task_create_database(char* name);

void task_drop(char* name);

void task_create_entity(char* name);

void task_commit();

void task_list_entities();

void perform(command* c);

#endif