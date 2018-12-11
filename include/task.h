#ifndef _TASK_H_
#define _TASK_H_

void task_close(int notify);

void task_database(char* name);

void task_create_database(char* name);

void task_drop_database(char* name);

void task_drop_entity(char* name);

void task_create_entity(char* name);

void task_commit();

void task_list_entities();

void task_create_field(char *name, data_t type, uint32_t size);

void perform(command* c);

#endif