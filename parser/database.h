#ifndef DATABASE_H
#define DATABASE_H

#define MAX_TAG 100
#define MAX_FILE 1000





struct file{
	int id; 
	char name[20]; 
	struct tag* tags[MAX_TAG]; 
	UT_hash_handle hh;
}; 

struct tag{
	int id; 
	char name[20]; 
	struct file* files[MAX_FILE]; 
	UT_hash_handle hh;
}; 




struct file * create_file(char *filename);
struct tag * create_tag(char * tag);

struct tag *get_tags();

void add_file(struct file *f);
void add_tag(struct tag *t);
int add_file_to_tag(struct file *f, char *t);
int add_tag_to_file(struct tag *t, char *filename);
void print_files_tag(struct tag *t);
void print_tags_file(struct file *f);

int rm_tag_from_file(struct tag *t, char *filename);

struct file *get_file2(char *fileName);
struct tag *find_tag2(char *tagName);

int is_file_tagged_with(struct file *f, char *tagname);

#endif
