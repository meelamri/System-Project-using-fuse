%{
#include "parse.h"
#include "../tagfs/hashtable.h"
char buffer[MAX_SIZE];
int applytoall = 0;
char tagapplytoall[BUFLEN];
char filename_[BUFLEN];
struct tag * all;
%}


%option noyywrap
tagforall  [a-z A-Z]+[a-z A-Z 0-9"."]*"appliqué à tous les fichiers""\n"
filename [a-z A-Z]+[a-z A-Z 0-9"."]*
tag [a-z A-Z]+[a-z A-Z 0-9"."]*
commentary "#"+
applytagto "["{filename}"]\n"
filetagedby {tag}" "*"appliqué au fichier"*" "*{filename}*"\n"{commentary}*
tagsfile {applytagto}{filetagedby}+

%%
{tagforall} {strcpy(buffer,yytext);
applytoall++;
char *pch = strtok(buffer, " ");
strcpy(tagapplytoall,pch);
//all = create_tag(tagapplytoall);
//add_tag(all);
}

{tagsfile} {strcpy(buffer,yytext);
int i = 0;
//printf("\ninput:\n\x1B[34m%s\x1B[34m\n",buffer);
//printf("\x1b[32mparse ok\n\x1b[32m\n");
char *pch = strtok(buffer, " []\n");
while (pch != NULL) {
	if (i==0){
		add_file_with_name(pch);
		//f = create_file_(pch);
		strcpy(filename_,pch);
		//add_file(f);
		//printf("filename:%s\n",filename_);
	}	
	else{
		tag_file(filename_,pch);
	
		/*if(applytoall)
		add_tag_to_file(all, filename_);
		t= NULL;
		*/
	}
		pch = strtok (NULL, " []\n");
		i++;
	}
}

%%

void accepted (char *path){

long lSize;
char *buffer;

FILE *fp = fopen ( path , "rb" );
if( !fp ) perror(path),exit(1);

fseek( fp , 0L , SEEK_END);
lSize = ftell( fp );
rewind( fp );

/* allocate memory for entire content */
buffer = calloc( 1, lSize+1 );
if( !buffer ) fclose(fp),fputs("memory alloc fails",stderr),exit(1);

/* copy the file into the buffer */
if( 1!=fread( buffer , lSize, 1 , fp) )
fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);
YY_BUFFER_STATE str_buffer = yy_scan_string(buffer);
yylex();
yy_delete_buffer(str_buffer);


fclose(fp);
free(buffer);






}
       
