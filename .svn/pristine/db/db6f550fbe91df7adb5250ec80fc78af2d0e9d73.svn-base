#include "uthash.h"
#include <stdio.h>
#include <string.h>


#include "database.h"

struct file *files = NULL;
struct tag *tags = NULL; 

struct file * create_file(char *filename){
	struct file * f = malloc(sizeof(struct file));
	strcpy(f->name, filename);
	int i = 0; 
	while(i < MAX_TAG){
		f->tags[i] = NULL;
		i++;
		} 
	return f; 
}

struct tag * create_tag(char * tag){
	struct tag * t = malloc(sizeof(struct tag)); 
	strcpy(t->name, tag); 
	int i = 0; 
	while(i < MAX_FILE){
		t->files[i] = NULL;
		i++;
		} 
	return t; 
}

void add_file(struct file *f) {
	struct file *tmp ; 
	HASH_FIND_STR(files, f->name, tmp);
    if(!tmp) 
    	HASH_ADD_STR(files, name, f);    
}

void add_tag(struct tag *t){
	struct tag *tmp; 
	HASH_FIND_STR(tags, t->name, tmp); 
	if(!tmp)
		HASH_ADD_STR(tags, name, t); 
}

int add_file_to_tag(struct file *f, char *t){
	struct tag * tmp; 
	struct file * tmp_;
	HASH_FIND_STR(tags, t, tmp);
	
	if(!tmp) return 0; 
	
	int i = 0; 
	while(tmp->files[i]){
		if(!strcmp(tmp->files[i]->name,f->name)){
			printf("Fichier existant\n");
			return 0; 
		}
		i++;
	}
	
	HASH_FIND_STR(files, f->name,tmp_);
	if(!tmp_) add_file(f);
	tmp->files[i]=tmp_;
	int j = 0; 
	while(tmp_->tags[j]){
		if(!strcmp(tmp_->tags[j]->name,t))
			return 0; 
		j++; }
	
	tmp_->tags[j]=tmp; 

	return 1; 
}

int add_tag_to_file(struct tag *t, char *filename){
	struct file *tmp; 
	struct tag *tmp_;
	HASH_FIND_STR(files, filename, tmp); 
	if (!tmp) return 0 ;
	int i = 0; 
	while(tmp->tags[i]) {
		if(!strcmp(t->name,tmp->tags[i]->name)){
			return 0; 
		}
		i++; 
	}
	HASH_FIND_STR(tags, t->name, tmp_); 
	if (!tmp_) add_tag(t);
	tmp->tags[i]=tmp_;
	int j = 0;  
	while(tmp_->files[j]){
		if(!strcmp(tmp_->files[j]->name,filename))
			return 0; 
		j++; 
	 }
	tmp_->files[j]=tmp; 

	return 1;
}


int rm_tag_from_file(struct tag *t, char *filename){
  struct file *tmp;
  struct tag *tmp_;
  HASH_FIND_STR(files, filename, tmp);
  if (!tmp) return 0;
  int i = 0;
  while(tmp->tags[i]) {
    if(strcmp(t->name, tmp->tags[i]->name)){
	tmp->tags[i] = NULL;
    }
    i++;
  }
  
  HASH_FIND_STR(tags, t->name, tmp_);
  if (!tmp) return 0;
  int j = 0;
  while(tmp_->files[j]){
    if(strcmp(tmp_->files[j]->name, filename)){
      tmp_->files[j] = NULL;
    }
    j++;
  

  return 1;

  }
}


void print_files_tag(struct tag *t){
	int i = 0 ; 
	while(t->files[i]!=NULL){
		printf("filename:%s\n",t->files[i]->name);
		i++; 
	}
}

void print_tags_file(struct file *f){
	int i = 0 ; 
	while(f->tags[i]!=NULL){
		printf("tag:%s\n",f->tags[i]->name);
		i++; 
	}
}

void databae_display(){
	printf("\n-----DATABASE-----\n");
	struct file *f; 
	for(f=files; f != NULL; f=f->hh.next){
		printf("filename: %s\n",f->name);
		int i = 0; 
		while (f->tags[i]){
			printf("\t----taged: %s\n",f->tags[i]->name);
			i++; 
		}

	}
}



struct tag *get_tags2(){
    return tags;
}

struct file *get_files2(){
    return files;
}

struct tag *find_tag2(char *tagName){
    struct tag *tagg;
    HASH_FIND_STR(tags, tagName, tagg);
    return tagg;
}


struct file *get_file2(char *fileName) {
    struct file *ff;
    HASH_FIND_STR(files, fileName, ff);
    return ff;
}
