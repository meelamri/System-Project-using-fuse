#ifndef STRUCT_TAGFS_H
#define STRUCT_TAGFS_H

#include "uthash.h"

typedef struct File File;
struct File{
  char * name; 
  struct List_Tag* list_tags;
  UT_hash_handle hh; 
};

typedef struct Tag Tag;
struct Tag{
  char *name; 
  struct List_File* list_files; 
  UT_hash_handle hh; 
}; 

typedef struct List_Tag List_Tag;
struct List_Tag{
  struct Tag* tag; 
  struct List_Tag* next;
};

typedef struct List_File List_File;
struct List_File{
  struct File* file; 
  struct List_File* next;
  int size;
};


#endif

