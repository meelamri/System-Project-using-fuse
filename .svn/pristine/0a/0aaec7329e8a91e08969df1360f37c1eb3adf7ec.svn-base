#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"



#define LOGFILE "tagfs.log"
FILE *mylog;
#define LOG(args...) do { fprintf(mylog, args); fflush(mylog); } while (0)



Tag *hashtab_tag = NULL;
File *hashtab_file = NULL;


void add_tag_with_name(char *tagName) {
    Tag *new_tag = find_tag(tagName);
    if (new_tag == NULL) {
	new_tag = malloc(sizeof(Tag));
	new_tag->name = strdup(tagName);
	new_tag->list_files = malloc(sizeof(List_File));
	//new_tag->list_files->file = NULL;
	new_tag->list_files->next = NULL;
	HASH_ADD_KEYPTR(hh, hashtab_tag, new_tag->name, strlen(new_tag->name), new_tag);  /* id: name of key field */
    }
}


void add_file_with_name(char *fileName) {
    File *new_file = find_file(fileName);
    if (new_file == NULL) {
	new_file = malloc(sizeof(File));
	new_file->name = strdup(fileName);
	new_file->list_tags = malloc(sizeof(List_Tag));
	//new_file->list_tags->tag = NULL;
	new_file->list_tags->next = NULL;
	HASH_ADD_KEYPTR(hh, hashtab_file, new_file->name, strlen(new_file->name), new_file);  /* id: name of key field */
    }
}


Tag *find_tag(char *tagName) {
    Tag *new_tag;
    // LOG("In find_tag, looking for %s\n", tagName);
    HASH_FIND_STR(hashtab_tag, tagName, new_tag);
    //   LOG("In find_tag, returning %p\n", new_tag);
    return new_tag;
}


File *find_file(char *fileName) {
    File *new_file;
    // LOG("In find_tag, looking for %s\n", tagName);
    HASH_FIND_STR(hashtab_file, fileName, new_file);
    //   LOG("In find_tag, returning %p\n", new_tag);
    return new_file;
}


void delete_tag(char * tagName) { 
    Tag *tag = find_tag(tagName);
    List_File * fcurrent = tag->list_files->next;
    if(fcurrent != NULL) {
	while(fcurrent != NULL) {
	    delete_tag_from_file(fcurrent->file->name, tagName);
	    fcurrent = fcurrent->next;
	}
    }
    free(fcurrent);
    HASH_DEL(hashtab_tag, tag);  /* user: pointer to deletee */
    free(tag->name);
    delete_list_files(tag->list_files);
    free(tag);
}


void delete_file(char * fileName) {
    File *file = find_file(fileName);
    List_Tag * tcurrent = file->list_tags->next;
    if(tcurrent != NULL) {
	while(tcurrent != NULL) {
	    delete_file_from_tag(fileName, tcurrent->tag->name);
	    tcurrent = tcurrent->next;
	}
    }
    free(tcurrent);
    HASH_DEL(hashtab_file, file);  /* user: pointer to deletee */
    free(file->name);
    delete_list_tags(file->list_tags);
    free(file);
}

void delete_list_tags(List_Tag* list) {
  List_Tag *current = list;
  List_Tag *tmp = list->next;
  while(tmp != NULL) {
    free(current);
    current = tmp;
    tmp = tmp->next;
  }
  free(tmp);
}

void delete_list_files(List_File* list) {
  List_File *current = list;
  List_File *tmp = list->next;
  while(tmp != NULL) {
    free(current);
    current = tmp;
    tmp = tmp->next;
  }
  list->size=0;
  free(tmp);
}


void delete_all_tags() {
    Tag *current_tag, *tmp;
    
    HASH_ITER(hh, hashtab_tag, current_tag, tmp) {
	delete_tag(current_tag->name);
    }
    free(tmp); 
}


void delete_all_files() {
    File *current_file, *tmp;
    
    HASH_ITER(hh, hashtab_file, current_file, tmp) {
	delete_file(current_file->name);
    }
    free(tmp);
}

void print_tags() {
    Tag *new_tag;
    //printf("Les tags présents dans la table sont : \n");
    for(new_tag=hashtab_tag; new_tag != NULL; new_tag=(Tag*)(new_tag->hh.next)) {
        printf("-> %s \n", new_tag->name);
	print_files_for_tag(new_tag);
	printf("\n");
    }
    free(new_tag);
}


void print_files() {
    File *new_file;
    printf("---------- DATABASE ---------- \n");
    for(new_file=hashtab_file; new_file != NULL; new_file=(File*)(new_file->hh.next)) {
        printf("-> %s \n", new_file->name);
	print_tags_for_file(new_file);
	printf("\n");
    }
    free(new_file);
}


void print_files_for_tag(Tag *tag) {
    //printf("Le tag %s a ces fichiers : \n", tag->name);
    List_File * current = tag->list_files->next; 
    while(current != NULL) { 
	printf("\t --> %s \n", current->file->name);
	current = current->next;
    }
    free(current);
}


void print_tags_for_file(File *file) {
    List_Tag * current = file->list_tags->next; 
    while(current) { 
	printf("\t --> %s \n", current->tag->name);
	current = current->next;
    }
    free(current);
}


int have_tag(char *fileName, char *tagName) {
    Tag *tag = find_tag(tagName);
    File *file = find_file(fileName);
    if(tag != NULL && file != NULL) {
	List_Tag * tcurrent = file->list_tags->next;
	while(tcurrent != NULL) {
	    if(!strcmp(tcurrent->tag->name, tagName))
		return 1;
	    tcurrent = tcurrent->next;
	}
	free(tcurrent);
    } 
    return 0;
}


int tag_file(char *fileName, char *tagName) {
    Tag *tag = find_tag(tagName);

    if(tag == NULL ){
	add_tag_with_name(tagName);
	tag = find_tag(tagName);
    }
    
	
    File *file = find_file(fileName);
    if(file == NULL) {
        add_file_with_name(fileName);
	file = find_file(fileName);
    }

    add_file_to_list(tag->list_files,file);
    add_tag_to_list(file->list_tags,tag);
    
    return 1;
}

void add_file_to_list(List_File *list, File* file){
    List_File * fcurrent = list;
    List_File * ftoAdd = malloc(sizeof(List_File));
    ftoAdd->file = file;
    if(fcurrent != NULL) {
	while(fcurrent->next != NULL) {
	    fcurrent = fcurrent->next;
	}
    }
    fcurrent->next = ftoAdd;
    ftoAdd->next = NULL;
    list->size++;
    //free(fcurrent);
    //free(ftoAdd);
}

void add_tag_to_list(List_Tag *list, Tag* tag){
    List_Tag * tcurrent = list;
    List_Tag * ttoAdd = malloc(sizeof(List_Tag));
  
    ttoAdd->tag = tag;
  
    if(tcurrent != NULL) {
	while(tcurrent->next != NULL) {
	    tcurrent = tcurrent->next;
	}
    }
    tcurrent->next = ttoAdd;
    ttoAdd->next = NULL;
    //free(tcurrent);
    //free(ttoAdd);
}

int delete_file_from_tag(char *fileName, char *tagName){
    Tag *tag = find_tag(tagName);
    File *file = find_file(fileName);
    if(!have_tag(fileName, tagName)) 
	return 0;
  
    List_File * fcurrent = tag->list_files;

    while(fcurrent->next != NULL) {
	if(fcurrent->next->file != file) {
	    fcurrent = fcurrent->next;
	} else {
	    break;
	}
    }
    List_File * toDelete = fcurrent->next;
    fcurrent->next = fcurrent->next->next;
    free(toDelete);

    // EXTENSION : supprime tag sans fichier
    if(tag->list_files->next == NULL) {
	delete_tag_from_file(fileName, tagName);
	HASH_DEL(hashtab_tag, tag); 
	free(tag);
	tag->list_files->size--;
    }
    //free(fcurrent);
    return 1;
}

int delete_tag_from_file(char *fileName, char *tagName) {
    Tag *tag = find_tag(tagName);
    File *file = find_file(fileName);
    if(!have_tag(fileName, tagName)) 
	return 0;
    List_Tag * tcurrent = file->list_tags;

    while(tcurrent->next != NULL) {
	if(tcurrent->next->tag != tag) {
	    tcurrent = tcurrent->next;
	} else {
	    break;
	}
    }
    List_Tag * toDelete = tcurrent->next;
    tcurrent->next = tcurrent->next->next;
    //free(tcurrent);
    free(toDelete);
    return 1;
}


int untag_file(char *fileName, char *tagName) {
    return(delete_file_from_tag(fileName, tagName) && delete_tag_from_file(fileName, tagName));
}

List_Tag* tags_for_all(){
    int nbFichiers = HASH_COUNT(hashtab_file);
    List_Tag* list_tag = malloc(sizeof(List_Tag));
    Tag *current_tag,*tmp;
    HASH_ITER(hh,hashtab_tag,current_tag,tmp){
	if(current_tag->list_files->size == nbFichiers)
	    add_tag_to_list(list_tag,current_tag);     
    }
    free(current_tag);
    free(tmp);
    return list_tag;
}


Tag *get_tags(){
    return hashtab_tag;
}

File *get_files(){
    return hashtab_file;
}



int SaveDB (FILE* fp){
    //FILE* database = NULL;
    File *f;
  
    FILE* database = fp;
    //    if( database == NULL ) perror(path);
    LOG("Saving databse: %p, path:\n", database/*path*/);

    if (database!= NULL)
	{
	
	    /*List_Tag* tag4all = tags_for_all();
	      tag4all=tag4all->next; 
	      while(tag4all) { 
	      fprintf(database,"%s\n", tag4all->tag->name);
	      tag4all = tag4all->next;
	      }
	    */

	    for(f = hashtab_file; f != NULL; f=f->hh.next){
		fprintf(database, "[%s]\n", f->name);
		LOG("[%s]\n", f->name);
		fprintf(stdout, "[%s]\n", f->name);
		List_Tag * current = f->list_tags->next; 
		while(current) {
		    fprintf(database,"%s\n", current->tag->name);
		    fprintf(stdout,"%s\n", current->tag->name);
		    LOG("%s\n", current->tag->name);
		    current = current->next;
		}
		free(current);

	    }
	    //free(tag4all);
	}
  

    return 0;

  
}


void print_list_files(List_File *list){
  List_File *current=list->next;
  while(current!=NULL){
    printf("fichier retenu: %s\n", current->file->name);
    current=current->next;
  }
}

List_File *get_files_intersectionTags(List_Tag *list){
  List_File *files=malloc(sizeof(List_File));
  files->next=NULL;
  File *current_file, *tmp;
    List_Tag *current_tag=list->next;
    HASH_ITER(hh, hashtab_file, current_file, tmp) {
      while(current_tag!=NULL){
	if(!have_tag(current_file->name,current_tag->tag->name))
	  break;
	current_tag=current_tag->next;
      }
      if(current_tag==NULL)
	add_file_to_list(files,current_file);
    
      current_tag=list->next;
  }
  return files;
}


List_File* get_files_tagRegex(char** regexp){
  return get_files_intersectionTags(get_tags_regex(regexp));
  }


List_Tag* get_tags_regex(char** regexps){

  List_Tag *tags=malloc(sizeof(List_Tag));
  tags->next=NULL;
  int i=0;
    
  Tag *new_tag,*tmp;
    HASH_ITER(hh, hashtab_tag, new_tag, tmp){
      for(i=0;regexps[i]!=NULL;i++){
      if(!tag_matches_regex(new_tag,regexps[i]))
	break;
      }
      if(regexps[i]==NULL)
	add_tag_to_list(tags,new_tag);
    }
  return tags;
}


int tag_matches_regex(Tag* tag,char* regexp){
  int err;
  regex_t preg;
  const char *str_request = tag->name;
  err = regcomp (&preg, regexp, REG_NOSUB | REG_EXTENDED);
  if (err == 0){
    int match;

    match = regexec (&preg, str_request, 0, NULL, 0);
    regfree (&preg);

    if (match == 0){
      printf ("%s : valide pour %s\n", str_request,regexp);
      return 1;
    }
    else if (match == REG_NOMATCH){
      printf ("%s n\'est pas valide pour %s\n", str_request,regexp);
      return 0;
    }
  }
  return 0;
}

