#define _GNU_SOURCE
#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <libgen.h>
#include <errno.h>
#include <assert.h>

#include "hashtable.h"
#include "struct_tagfs.h"

#include "../parser/parse.h"


static char *dirpath;
static DIR *dir;
FILE *console;


/*********************************
 * Logs
 *********************************/
 

#define LOGFILE "tagfs.log"
FILE *mylog;
#define LOG(args...) do { fprintf(mylog, args); fflush(mylog); } while (0)


/**********************************
 * Print functions
 *********************************/


void tagfs_print_tags(void *buf, fuse_fill_dir_t filler) {
    Tag *hashtab_tag = get_tags();
    Tag *new_tag;
    for(new_tag = hashtab_tag; new_tag != NULL; new_tag = new_tag->hh.next) {
	filler(buf, new_tag->name, NULL, 0);
    }
}

void tagfs_print_tags_other_than(void *buf, fuse_fill_dir_t filler, char *tagname) {
    struct Tag*hashtab_tag = get_tags();
    struct Tag *new_tag;
    for(new_tag = hashtab_tag; new_tag != NULL; new_tag = new_tag->hh.next) {
	if (strcmp(tagname, new_tag->name) != 0)
	    filler(buf, new_tag->name, NULL, 0);
    }
}

void tagfs_print_files_of_tag(Tag *t, void *buf, fuse_fill_dir_t filler) {
    LOG("Printing tags\n");
    List_File *current = t->list_files->next;
    while(current != NULL){
	filler(buf, current->file->name, NULL, 0);
	current = current->next;
    }
}


/*********************************
 * tagfs's fuse operation
 *********************************/



char *tagfs_realpath(const char *path)
{
    char *realpath;
    char *pathDup = strdup(path);
    asprintf(&realpath, "%s/%s", dirpath, basename(pathDup));

    free(pathDup);
    return realpath;
}


int is_ln_tag_ok(char *tag) {
    LOG("Checking %s\n", tag);
    if(strcmp(tag,".") == 0 || strcmp(tag,"/") == 0 || strcmp(tag,"..") == 0 || strcmp(tag, "") == 0)
	{
	    LOG("returning 1\n");
	    return 1;
	}
    else
	return 0;    
}



int tagfs_getattr(const char *path, struct stat *stbuf)
{
    char *realpath = tagfs_realpath(path);
    char *pathDup = strdup(path);
    char *tagname;
    
    int res, ln_stat;
    Tag *tag;

    LOG("getattr on : ' %s ' ->\n", realpath);
    
    char *ln_tag, *ln_file;
    ln_file = basename(pathDup);
    ln_tag = basename(dirname(pathDup));
    
    tag = find_tag(ln_tag);
    
    if (!is_ln_tag_ok(ln_file))
	ln_stat = stat(tagfs_realpath(ln_file), stbuf);
    else
	ln_stat = -1;
    
    if (!is_ln_tag_ok(ln_tag) && ln_stat == 0){
	LOG("Is file '%s' tagged with '%s' ? : %d\n", ln_file, ln_tag, have_tag(ln_file, ln_tag));
	if (!have_tag(ln_file, ln_tag)){
	    LOG("\nI got a  tagging request\n");
	    realpath = tagfs_realpath(ln_tag);
	    return -ENOENT;
	    
	}
    }
	    
    res = stat(realpath, stbuf);  
    if(res < 0 ) {
	tagname = basename(realpath);
	LOG("Looking for tag: %s\n", tagname);
	tag = find_tag(tagname);
	LOG("%p", tag);

    
	if (tag == NULL) {
	    LOG("Didn't find it\n");    
	    LOG("getattr returns %d\n", -errno);

	    res = -ENOENT;	    
	}
	else {
	    LOG("Found it");
	    res = stat(dirpath, stbuf);
	    LOG("\ngetattr returns %d\n", -res);
	}
    }
    
    //free(pathDup);
    LOG("\ngetattr returns %d\n", -res);
    return res;
}


int tagfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    
    struct dirent *dirent;
    int res = 0;
    char *tagName = strdup(path);
    tagName = basename(tagName);
    Tag *tag;


    LOG("tagfs_readdir on : ' %s ' tagName is : %s\n", path, tagName);
    tag = find_tag(tagName);
    
    if (tag != NULL) {
	tagfs_print_files_of_tag(tag, buf, filler);
	//tagfs_print_tags_other_than(buf, filler, tagName);
	return 0;
    }
    
    if(strcmp(path, "/") != 0) 
        return -ENOENT;
    
    rewinddir(dir);
    while ((dirent = readdir(dir)) != NULL){
	struct stat stbuf;
	res = tagfs_getattr(dirent->d_name, &stbuf);
	filler(buf, dirent->d_name, NULL, 0);
    }
    tagfs_print_tags(buf, filler);
    
    return res;
}


int tagfs_mkdir(const char *path, mode_t mode)
{
    char *pathDup = strdup(path);
    char *tagname = basename(pathDup);
   
    LOG("\nGoing to add the tag: %s\n", tagname);
    add_tag_with_name(tagname); 
    
    free(pathDup);
    return 0;
}


int tagfs_link(const char *from , const char *to){
    LOG("tagfs_link : ->\n");
    
    char *pathDup = strdup(to);
    char *filename = basename(pathDup);
    char *tagname = basename(dirname(pathDup));
    int res;
    LOG("tagname : %s, filename: %s\n", tagname, filename);



    File *f = find_file(filename);
    if (f == NULL) {
	add_file_with_name(filename);
    }
    
    res = tag_file(filename, tagname);
    LOG("tagfs_link return : %d\n", res);
    return 0;
}



int tagfs_unlink(const char *path) {
    char *pathDup  = strdup(path);
    char *fileName = basename(pathDup);
    char *tagName = basename(dirname(pathDup));

    int res;
    File *f = find_file(fileName);
    Tag *t = find_tag(tagName);

    if (f == NULL || t == NULL)
	res = -1;
    
    LOG("\ntagfs_unlink : Want to remove a tag from a file\nTagName : %s FileName : %s\n", tagName, fileName);
    res = untag_file(fileName, tagName);
    if (res == 1)
	res = 0; 
    LOG("tagfs_unlink : return %d", res);
    return res;
}



/*
 * Deletes a tag 
 * As unused tag are automatically deleted from the database we don't need this 
 */
int tagfs_rmdir(const char *path) {
    char *pathDup = strdup(path);
    char *tagname = basename(pathDup);

    LOG("\nWanna remove tag : %s", tagname);
    //delete_tag(tagname);
    
    free(pathDup);
    return 0;
}


int tagfs_rename(const char *from, const char *to){
    char *pathDup = strdup(from);
    char *pathDup2 = strdup(to);
    
    char *fileName = basename(pathDup);
    char *oldTagName = basename(dirname(pathDup));
    char * newTagName = basename(dirname(pathDup2));

    LOG("\n tagfs_rename : filename : %s  oldtag : %s ->> newtag : %s\n", fileName, oldTagName, newTagName);
    tag_file(fileName, newTagName);
    untag_file(fileName, oldTagName);


    free(pathDup);
    free(pathDup2);
    return 0;
}


/**********************************
 * Operation struct
 **********************************/

static struct fuse_operations tagfs_oper = {
    .getattr = tagfs_getattr,
    .readdir = tagfs_readdir,
    .mkdir = tagfs_mkdir,
    .link = tagfs_link,
    .unlink = tagfs_unlink,
    .rmdir = tagfs_rmdir,
    .rename = tagfs_rename
};




/**********************************
 * Main
 **********************************/

int main(int argc, char *argv[])
{
    int err;
    if (argc < 2){
	fprintf(stderr, "usage: ./tagfs rootdir mountdir");
	return 0;
    }
    
    /* find the absolute directory because fuse_main()
     * doesn't launch the daemon in the same current directory.*/
    dirpath = realpath(argv[1], NULL);
    dir = opendir(dirpath);
    
    if (!dir) {
	fprintf(stderr, "couldn't open directory %s\n", dirpath);
	exit(EXIT_FAILURE);
    }
    argv++;
    argc--;
    
    mylog = fopen(LOGFILE, "a"); /* append logs to previous executions */
    LOG("\n");

    LOG("starting tagfs in %s\n", dirpath);
	
    char *path = basename(argv[0]);
    asprintf(&path,"%s/.tags", path); 	
    
    LOG("Load database from %s\n", path);
    
    accepted(path);
    FILE *fp = fopen(path, "w+");
    err = fuse_main(argc, argv, &tagfs_oper,NULL);
    SaveDB(fp);
    fclose(fp);
    
    LOG("stopped tags with return code %d\n", err);
}
