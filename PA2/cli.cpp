#include <string>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/wait.h>
using namespace std;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


void * thread_func(void * arg) {
    string *sp = static_cast<std::string*>(arg);
    string s = *sp;
    //cout<<s<<endl;
    istringstream iss(s);
    string s1;
    int count2=0;
    vector<string> words3;
    while(iss >> s1){
        words3.push_back(s1); 
        //cout << s1 << endl;
    }

    int count3=0;
    int count4=0;
    for (int i=0; i < words3.size(); i++){
        if (words3[i]=="<"){
                count3++;
            }
        }
        for (int i=0; i < words3.size(); i++){
            if (words3[i]==">"){
                count4++;
            }
        }
    
    if(count3 != 0){
            
            pthread_mutex_lock(&lock);
            cout << "---- " << pthread_self() << endl;
            int rc2 = fork();
            if (rc2 < 0) {
            // fork failed
                fprintf(stderr, "fork failed\n");
                exit(1);

            } else if (rc2 == 0){
                string fileName;
                for(int a=0;a<words3.size();a++){
                    string str = words3[a];
                    string str2=".txt";
                    int count=0;
                    if (str.find(str2) != string::npos) {
                        count++;
                    } 
                    if (count!=0)
                        fileName=str;
                }
                dup(STDIN_FILENO); 
                dup2(open(fileName.c_str(), O_RDONLY, S_IRWXU),STDIN_FILENO);
                char*myargs[4];
                for(int i=0;i<3;i++){     
                    if(words3[i]!="<"){
                        myargs[i] = strdup(words3[i].c_str()); 
                    }
                }          
                
                execvp(myargs[0], myargs); //execute

                
            } else{
                int rc_wait = wait(NULL);
                cout << "---- " << pthread_self() << endl;
                pthread_mutex_unlock(&lock);
            }
        }
        else if(count4 != 0){
            pthread_mutex_lock(&lock); 
            cout << "---- " << pthread_self() << endl; 
            int rc3 = fork();
            if (rc3 < 0) {
            // fork failed
                fprintf(stderr, "fork failed\n");
                exit(1);

            } else if (rc3 == 0){
                string fileName;
                for(int a=0;a<words3.size();a++){
                    string str = words3[a];
                    string str2=".txt";
                    int count=0;
                    if (str.find(str2) != string::npos) {
                        count++;
                    } 
                    if (count!=0)
                        fileName=str;
                }
                
                dup(STDOUT_FILENO);
                dup2(open(fileName.c_str(), O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU),STDOUT_FILENO);  
                char*myargs[4];
                for(int i=0;i<3;i++){     
                    if(words3[i]!=">"){
                        myargs[i] = strdup(words3[i].c_str()); 
                    }
                
                }  
                            
                execvp(myargs[0], myargs); //execute
        
            } else {
                int rc_wait = wait(NULL);
                cout << "---- " << pthread_self() << endl;
                pthread_mutex_unlock(&lock);
            }

        }
        else{
            pthread_mutex_lock(&lock);
            cout << "---- " << pthread_self() << endl;
            int rc = fork();
            if (rc < 0) {
            // fork failed
                fprintf(stderr, "fork failed\n");
                exit(1);

            } else if (rc == 0){
                
                char*myargs[4];
                for (int i =0;i<3;i++){   
                    if(words3[i]!="&"){         
                        myargs[i] = strdup(words3[i].c_str());
                    }
                
                }          
        
                execvp(myargs[0], myargs); //execute
            
            } else{
                int rc_wait = wait(NULL);
                cout << "---- " << pthread_self() << endl;
                pthread_mutex_unlock(&lock);
            }
        }
        
}
int main(int argc, char *argv[]){
    ifstream inFile;
    inFile.open("commands.txt");
    ofstream outFile("parse.txt");
    string line;
    vector<string> commands;
    while(getline(inFile,line)){
        commands.push_back(line);
        istringstream iss(line);
        string s;
        outFile << "----------"<<endl;
        vector<string> words;
        while(getline(iss,s,' ')){
            words.push_back(s);
        
        }
        outFile << "Command: "<< words[0]<<endl;
        if (words[1][0]=='-'){
            outFile << "Inputs: "<<endl;
            outFile << "Options: "<< words[1]<<endl;
        }
        if (words[1][0]!='-'){
            outFile << "Inputs: "<< words[1]<<endl;
            if (words[2][0]=='-'){
                outFile << "Options: "<<words[2]<<endl;
            }
            else{
                outFile << "Options: "<<endl;
            }
        }
        int count = 0;
        for(int i = 1; i < words.size();i++){
            
            if(words[i][0]=='<'||words[i][0]=='>'){
                outFile << "Redirection: "<<words[i]<<endl;
                count++;
            }
            
            
        }
        if (count == 0)
                outFile << "Redirection: -"<<endl;
        

        if(words[words.size()-1][0] == '&'){
            outFile << "Background Job: y"<<endl; 
        }
        else
            outFile << "Background Job: n"<<endl; 
        

        outFile << "----------"<<endl;
    }
    pthread_t* thread = (pthread_t*) calloc(commands.size(), sizeof(pthread_t));

    for(int g = 0; g<commands.size();g++){
        istringstream iss(commands[g]);
        string s1;
        int count2=0;
        vector<string> words2;
        while(getline(iss,s1,' ')){
            words2.push_back(s1); 
      
        }
        
        for (int i=0; i < words2.size(); i++){
            if (words2[i]=="<"||words2[i]==">"||words2[i]=="&"){
                count2++;
            }
        }
        
        if (count2 == 0){
            
            int rc = fork();
            if (rc < 0) {
            // fork failed
                fprintf(stderr, "fork failed\n");
                exit(1);

            } else if (rc == 0){
                
                char*myargs[4];
                for (int i =0;i<words2.size();i++){               
                     myargs[i] = strdup(words2[i].c_str());      
                }       
                  
                execvp(myargs[0], myargs); //execute
            } else{
                int rc_wait = wait(NULL);
            }
        }
        
        else{
            int count3=0;
            int count4=0;
            int count5=0;
         
            int count6=0;
            for (int i=0; i < words2.size(); i++){
                if (words2[i]=="<"){
                    count3++;
                }
            }
            for (int i=0; i < words2.size(); i++){
                if (words2[i]==">"){
                    count4++;
                }
            }
            for (int i=0; i < words2.size(); i++){
                if (words2[i]=="&"){
                    pthread_create(&thread[g], NULL, thread_func, &commands[g]);
                    
                    count5++; 
                }
            }
            if(count5 != 0){ 
                
                
               
            }

            else{ 
                pthread_join(thread[g-1],NULL);
                
                if(count3 != 0){
                    
                    int rc2 = fork();
                    if (rc2 < 0) {
                    // fork failed
                        fprintf(stderr, "fork failed\n");
                        exit(1);

                    } else if (rc2 == 0){
                        string fileName;
                        for(int a=0;a<words2.size();a++){
                            string str = words2[a];
                            string str2=".txt";
                            int count=0;
                            if (str.find(str2) != string::npos) {
                                count++;
                            } 
                            if (count!=0)
                                fileName=str;
                        }
                        dup(STDIN_FILENO); 
                        dup2(open(fileName.c_str(), O_RDONLY, S_IRWXU),STDIN_FILENO);
                        char*myargs[4];
                        for(int i=0;i<3;i++){     
                            if(words2[i]!="<"){
                                myargs[i] = strdup(words2[i].c_str()); //works
                            }
                        }          
                        execvp(myargs[0], myargs); //execute
                    } else{
                        int rc_wait = wait(NULL);
                    }
                }
                else if(count4 != 0){
                    int rc3 = fork();
                    if (rc3 < 0) {
                    // fork failed
                        fprintf(stderr, "fork failed\n");
                        exit(1);

                    } else if (rc3 == 0){
                        string fileName;
                        for(int a=0;a<words2.size();a++){
                            string str = words2[a];
                            string str2=".txt";
                            int count=0;
                            if (str.find(str2) != string::npos) {
                                count++;
                            } 
                            if (count!=0)
                                fileName=str;
                        }
                        
                        dup(STDOUT_FILENO);
                        dup2(open(fileName.c_str(), O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU),STDOUT_FILENO); 
                        
                        
                        char*myargs[4];
                        for(int i=0;i<3;i++){     
                            if(words2[i]!=">"){
                                myargs[i] = strdup(words2[i].c_str()); 
                            }
                        
                    
                        }                 
                        execvp(myargs[0], myargs); //execute
                    } else {
                        int rc_wait = wait(NULL);
                    }

                }
                else{
                    int rc = fork();
                    if (rc < 0) {
                    // fork failed
                        fprintf(stderr, "fork failed\n");
                        exit(1);

                    } else if (rc == 0){
                        
                        char*myargs[4];
                        for (int i =0;i<3;i++){   
                            if(words2[i]!="&"){         
                                myargs[i] = strdup(words2[i].c_str());
                            }
                        
                        }          
                        execvp(myargs[0], myargs); //execute
                    } else{
                        int rc_wait = wait(NULL);
                    }
                }
            }

                
        }
           
    }
    

}


