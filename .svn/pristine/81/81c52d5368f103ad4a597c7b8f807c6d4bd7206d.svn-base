CPPFLAGS=-D_FILE_OFFSET_BITS=64 
CFLAGS=-Wno-unused-label -Wno-unused-function


PROGRAMS = tagfs perfs_1tag_Nfichiers hashtable perfs_Ntags_1fichier:

all: tagfs perfs_1tag_Nfichier perfs_Ntags_1fichier

%: %.c hashtable.c
	gcc $(CPPFLAGS) $(CFLAGS) -Wall $^ -o $@ -lfuse



tagfs: tagfs.c  hashtable.c 
	flex --outfile=../parser/parse.c ../parser/parse.l
	gcc $(CPPFLAGS) $(CFLAGS) -Wall $^ ../parser/parse.c  -o $@ -lfuse

hashtable: hashtable.c hashtable_main.c 
	gcc $(CPPFLAGS) $(CFLAGS) -Wall $^ -o $@

perfs_1tag_Nfichier:
	gcc perfs_1tag_Nfichiers.c -o $@


perfs_Ntags_1fichier:
	gcc perfs_Ntags_1fichier.c -o $@


reset_database:
	cp Photos-tags.sav Photos/.tags

m:
	./tagfs Photos mnt
um:
	fusermount -u mnt

mm:
	make && make um && make m && cd mnt


clean:
	rm $(PROGRAMS)
