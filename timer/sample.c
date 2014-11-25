#include <time.h>
#include <stdio.h>
#include <unistd.h>

int get_time (struct timespec *tv);
int diff_time (struct timespec *start_time);

//=================================================
void main(){
    struct timespec tv;
    int rc, diff, i;
    
    rc = get_time(&tv);
    if(rc < 0){
        printf("get_time error\n\r");
        return;
    }
    i=0;
    while(1){
        diff = diff_time(&tv);
        if(diff >= 5000){
            printf("5sec passed (%d)\n\r", i);
            rc = get_time(&tv);
            if(rc < 0){
                printf("get_time error\n\r");
                return;
             } 
             i++;            
         }
        usleep(100000);  // 100msec wait 
    }

}
