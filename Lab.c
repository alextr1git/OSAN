#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "sys/stat.h"
#include <string.h>
#include <errno.h>

void output_dublicates();
int check_directory(char const *dir_name, int minSize, int maxSize);
void add_path(const char *file_path, int size);

typedef char Path[PATH_MAX]; //Path is array of chars size =4096
int *sizearr;
char lineout[100];
char scriptname[20];
int counter = 0;
Path *file_paths;
FILE *outputfile;
int file_paths_count = 0;
int file_paths_max_count = 10;
const int file_path_delta = 10;

void PrintError(const char *scr_name, const char *msg_err, const char *f_name)
{
    fprintf(stderr, "%s: %s %s\n", scr_name, msg_err, (f_name) ? f_name : "");
} 

int main(int argc, char const *argv[])
{
snprintf(lineout, 1000, "%s%s", argv[0], argv[1]); 
    if(argc != 5){ //script dir min max output.txt
        fprintf(stderr, "%s: Invalid arguments.\n", lineout);
        fprintf(stderr, "Correct format is: script dir min max output.txt\n");
        return 1;
    }
    scriptname = argv[0];
    int min_size = strtol(argv[2], NULL, 10); // transfer string to long int basis=10
    int max_size = strtol(argv[3], NULL, 10);
    outputfile = fopen(argv[4],"w");
     if (!outputfile)
   {
     fprintf(stderr, "%s: Cannot open the file %s\n", lineout, argv[4]);//print error to the error stream with string before all
    return 1;
   }
    if(min_size < 0 || max_size < 0){
        fprintf(stderr, "%s: Size cannot be less than 0 bytes!\n", lineout);
        return 1;
    }

    if(min_size > max_size){
           fprintf(stderr, "%s: Max size cannot be lesser then min size\n", lineout);
    }

    file_paths = malloc(sizeof(Path) * file_path_delta);
    sizearr = (int*)malloc(file_path_delta * sizeof(int));
    
    if(!file_paths){
       
           fprintf(stderr, "%s: malloc error occured\n", lineout);
        return 1;
    }

    check_directory(argv[1], min_size, max_size);
      
    
    for(int i=0; i<file_paths_count;i++){
    	fprintf(outputfile,"%s %i\n", file_paths[i],sizearr[i]);
    	}
    	printf("%i\n", file_paths_count); 	
    	//printf("Counter is: %i\n", counter);
    	
    free(file_paths);
    free(sizearr);

    return 0;
}

void add_path(const char *file_path, int size){ 
// adding extra space in order to have bigger file_path
    if(file_paths_max_count == file_paths_count){
        file_paths_max_count += file_path_delta;
        file_paths = realloc(file_paths, sizeof(Path) * file_paths_max_count);
        sizearr = (int*)realloc(sizearr, file_paths_max_count * sizeof(int));
        if((!file_paths) || (!sizearr)){
             fprintf(stderr, "%s: realloc error occured\n", lineout);
            file_paths_max_count -= file_path_delta;
            return;
        }
    }
//then just add

    sizearr[file_paths_count] = size;
    strcpy(file_paths[file_paths_count++], file_path);//copy file_path to file_pathS since last element of it
}

int check_directory(char const *dir_name, int min_size, int max_size){
    DIR *current_dir = opendir(dir_name); //cur_dir - pointer to oped dir of out dir_name
    if(!current_dir){ 
      //printf(stderr, "Opendir error occured: %s \n", dir_name);
       fprintf(stderr, "%s: Cannot open dir: %s\n", lineout, dir_name);
        return 1;
    }

    struct dirent *d;
    errno = 0; 

    while((d = readdir(current_dir)) || errno != 0)
    {
		if (errno != 0) {
        PrintError(scriptName, strerror(errno), filepath);
        }  
	     //while exist files in catalog
		Path filepath;
		sprintf(filepath, "%s/%s", dir_name, d->d_name); //output to our char array with formating ourdirectory/innerfile
	     	      
	       struct stat file_stat; //struct stat type variable file_stat
		    stat(filepath, &file_stat); // writing filepath to _file_stat
	     	     
		if(d->d_type == DT_REG){ //If regular file
	  	 
		   counter++;
		    if(file_stat.st_size > min_size && file_stat.st_size < max_size){
		    //checking file size
		    //counter++;
		        add_path(filepath, file_stat.st_size); //check size and add path
			}
		}
		
		else if (d->d_type == DT_DIR && strcmp(".", d->d_name) && strcmp("..", d->d_name)){ 
		    check_directory(filepath, min_size, max_size);
		  
	       }
	   errno = 0; 
	    
    }     

   	 if(closedir(current_dir))
 	   {
      	     fprintf(stderr, "%s: Closedir error occured: %s\n", lineout, dir_name);
      	     return 1;
   	    }
    
    
    return 0;
    
}