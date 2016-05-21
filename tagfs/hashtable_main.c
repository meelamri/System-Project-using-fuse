#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hashtable.h"
//#include "struct_tagfs.h"


int main() {  
  add_tag_with_name("Tag1");
  add_tag_with_name("Tag2");
  add_file_with_name("Fichier1");
  add_file_with_name("Fichier2");
 

  tag_file("Fichier1", "Tag1");
  tag_file("Fichier2", "Tag1");
  tag_file("Fichier2", "Tag2");

print_tags();

  char* regexp1="T.*";
  char* regexp2="[:alpha:]*2";
  char* regexps[]={regexp1,regexp2};
  print_list_files(get_files_tagRegex(regexps));

  //print_files();
  //SaveDB();

  //delete_list_tags(list_tag);

  delete_all_tags(); 
  //print_files();
  delete_all_files();
  

  return 0;
}
