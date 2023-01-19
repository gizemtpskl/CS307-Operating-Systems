#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

struct People{
    char firstName[50];
    char lastName[50];
    char gender[50];
} People;



int getpeoplecount (char * filename){
    FILE *fp;
    int count = 0;  
    char c;  
    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Could not open file %s", filename);
        return 0;
    }

    for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') 
            count = count + 1;

    fclose(fp);
    return(count);
}

const char * check(const char * filename)
{
    const char * dot = strrchr(filename, '.');
    if(!dot|| dot== filename) return "";
    return dot + 1;
}



void myFunc(char * directoryName,const int count, struct People people[]){
    
    DIR * dir = opendir(directoryName);
    if (dir == NULL)
        return;

    struct dirent * a;
    a = readdir(dir);
    while(a != NULL)
    {
        if(a->d_type == DT_DIR&&strcmp(a->d_name, ".") != 0 && strcmp(a->d_name, "..")!= 0){
            
    
            char path[500]= {0};

            strcat(path, directoryName);
            strcat(path, "/");
            strcat(path, a->d_name);
            myFunc(path,count,people);
        }
        else if(a->d_type == DT_REG&&strcmp(check(a->d_name), "txt") == 0&& strcmp(directoryName, ".") == 0 && (strcmp(a->d_name, "database.txt") != 0 )){ 
           
        
            char path2[500]={0};
            strcat(path2, directoryName);
            strcat(path2, "/");
            strcat(path2, a->d_name);

            FILE *file1 = fopen(path2, "r+");
            char line[250];
            
            while(fscanf(file1, "%s", line) == 1)
            {
                int i = 0;
                for (i = 0; i < count; i++) {
                    char * line_name = strdup(line);
                    if(strcmp(line_name, people[i].firstName)==0){
                   
                        if(strcmp(people[i].gender,"f")==0)
                        {
                            fseek(file1, -strlen(line_name) - 4, SEEK_CUR);
                            fputs("Ms.", file1);
                            fseek(file1, strlen(line_name) + 2, SEEK_CUR);
                            fputs(people[i].lastName, file1);
                        }
                        else if(strcmp(people[i].gender, "m")==0)
                        {
                            fseek(file1, -strlen(line_name)- 4, SEEK_CUR);
                            fputs("Mr.", file1);
                            fseek(file1, strlen(line_name) + 2, SEEK_CUR);
                            fputs(people[i].lastName, file1);
                        }
                    }
                }
  
                
            }
            fclose(file1);
        }
        a = readdir(dir);
    }
    
    closedir(dir);
}






int main(int argc, char * argv[]) {
    FILE * dataFile= fopen("database.txt","r");
    if (dataFile == NULL) {
        printf("database.txt could not be opened.\n");
        return 1;
    }
   
    fseek(dataFile, 0, SEEK_SET);
    struct People people[1000];
  
   
    int count = 0;

    
    char temp[500] = {0};
    while(fgets(temp,sizeof(temp),dataFile) != NULL)

    {
        sscanf(temp, " %s %s %s ", people[count].gender, people[count].firstName,people[count].lastName);
        count++;
       
        
    }
    fclose(dataFile);
    myFunc(".",count,people);
    
    
    return 0;
}