#include <stdio.h>
#include <fuse.h>


#include "hashtable.h"
#include "struct_tagfs.h"
#include "../parser/database.h"
#include "../parser/parse.h"


void translate_tags(){
    struct tag *oldTags = get_tags2();
    struct tag *tag;
    for (tag = oldTags; tag != NULL; tag = tag->hh.next) {
	add_tag_with_name(tag->name);
    }
}


void translate_files(){
    struct file *oldFiles = get_files2();
    struct file *file;

    for (file = oldFiles; file != NULL; file = file->hh.next) {
	add_file_with_name(file->name);
    }
}


void translate_taggedfiles(){
    struct file *f;
    struct files *files = get_files2();
    for(f=files; f != NULL; f=f->hh.next){
	int i = 0; 
	while (f->tags[i]){
	    tag_file(f->name, f->tags[i]->name);
	    i++; 
	}

    }
}
