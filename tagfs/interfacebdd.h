#ifndef INTERFACE_BDD_H
#define INTERFACE_BDD_H

struct files_list * creat_files_list(); 
struct tags_list * creat_tags_list(); 
struct files_list * init_files_list(struct files_list *); 
struct tags_list * init_tags_list(struct tags_list *); 
void add_file(struct file *); 
void add_tags(struct tags *);
//void remove_file(struct file *); 
//void remove_tag(struct tag *); 


