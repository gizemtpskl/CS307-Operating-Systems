#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

    printf("I'm SHELL process, with PID: %d - Main command is: man diff | grep -A 1 -e '-u' -m 1\n", getpid());

    int fd[2];  
    //file descriptor array with 2 integers
    if (pipe(fd) == -1) {
        printf("pipe failed\n");
        exit(1);
    }

    int rc = fork();
    if (rc < 0) {
    // fork failed
        fprintf(stderr, "fork failed\n");
        exit(1);

    } else if (rc == 0) {
        // first child
        printf("I’m MAN process, with PID: %d - My command is: man diff\n", getpid());

        dup2(fd[1], STDOUT_FILENO) ;
        close(fd[0]);

        close(fd[1]); 
        
        char*mycmd = "man";
        char*myargs[3];
        myargs[0] = "man";
        myargs[1] = "diff";
        myargs[2] = NULL; //end of the array
        execvp(mycmd, myargs); //execute

    } else {
        // parent goes down this path (main)
        int rc_wait = wait(NULL);

        int rc2 = fork();  //another child process for the second command (grep)
        if (rc2 < 0) {
    // fork failed
        fprintf(stderr, "fork failed\n");
        exit(1);
        } else if (rc2 == 0) {
        
        printf("I’m GREP process, with PID: %d - My command is: grep -A 1 -e '-u' -m 1 \n", getpid());
        dup2(fd[0], STDIN_FILENO);
        
        close(STDOUT_FILENO);
        open("output.txt", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);

        char * mycmd2 = "grep";
        char * myargs2[8];
        
        myargs2[0] = "grep";
        myargs2[1] = "-A";
        myargs2[2] = "1"; 
        myargs2[3] = "-e";
        myargs2[4] = "-u";
        myargs2[5] = "-m";
        myargs2[6] = "1";
        myargs2[7] = NULL;
        execvp(mycmd2, myargs2); 

        } else{
            close(fd[0]);
            close(fd[1]);
            waitpid(rc, NULL, 0);
            waitpid(rc2, NULL, 0);
            printf( "I'm SHELL process, with PID: %d - execution is completed, you can find the results in output.txt \n", (int) getpid()); 
        }

        

    }

    return 0;
}