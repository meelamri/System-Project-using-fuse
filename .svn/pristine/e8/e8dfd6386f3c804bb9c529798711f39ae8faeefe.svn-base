#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct_tagfs.h"
#include <regex.h>

void add_tag_with_name(char *tagName);
void add_tag_to_list(List_Tag *list, Tag* tag);
Tag *find_tag(char *tagName);
void delete_tag(char * tagName);
void delete_all_tags();
void print_tags();
void print_files_for_tag(Tag *tag);
Tag *get_tags();
int delete_file_from_tag(char *fileName, char *tagName);
List_Tag* tags_for_all();
List_Tag* get_tags_regex(char** regexp);
void delete_list_tags(List_Tag* list);
int tag_matches_regex(Tag* tag,char* regexp);

int tag_file(char * fileName, char * tagName); // retourne 0 si le fichier n'existe pas, 1 si ajout ok
int untag_file(char * fileName, char * tagName); // retourne 0 si le fichier et le tag ne sont pas appariés, 1 si suppression ok
int have_tag(char *fileName, char *tagName);

void add_file_with_name(char *fileName);
void add_file_to_list(List_File *list, File* file);
File *find_file(char *fileName);
void delete_file(char *fileName);
void delete_all_files();
void print_files();
void print_tags_for_file(File *file);
File *get_files();
int delete_tag_from_file(char *fileName, char *tagName);
List_File *get_files_intersectionTags(List_Tag *list);
List_File* get_files_tagRegex(char** regexp);
void delete_list_files(List_File* list);
void print_list_files(List_File *list);
int SaveDB (FILE* fp);

#endif
