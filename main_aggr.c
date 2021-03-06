#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

const int processNum = 10;
//const int processNum = 1;
const char* inputFile = "input.txt";


enum { NS_PER_SECOND = 1000000000 };

void sub_timespec(struct timespec t1, struct timespec t2, struct timespec *td)
{
    td->tv_nsec = t2.tv_nsec - t1.tv_nsec;
    td->tv_sec  = t2.tv_sec - t1.tv_sec;
    if (td->tv_sec > 0 && td->tv_nsec < 0)
    {
        td->tv_nsec += NS_PER_SECOND;
        td->tv_sec--;
    }
    else if (td->tv_sec < 0 && td->tv_nsec > 0)
    {
        td->tv_nsec -= NS_PER_SECOND;
        td->tv_sec++;
    }
}

//aggregation
int main() {
    int pid = 0, i;
    long long int numberNum = 0;
    FILE* fp = fopen(inputFile, "r+");
    FILE* splitedFile;
    
    fp = fopen(inputFile, "r+");
    long long int splitNum = 0;
    splitNum--;
    for(i = 0; i < processNum; i++){
        pid = fork();
        splitNum++;
        if(pid != 0) break;
    }
    
    if(pid == 0){
        for(i = 0; i < processNum; i++){
            int status;
            wait(&status);
        }
    }
    else{
        //children
        char fileNameBuf[60] = {0, };
        sprintf(fileNameBuf, "%s", "splited");
        sprintf(fileNameBuf + strlen(fileNameBuf), "_%lld.txt", splitNum);
        splitedFile = fopen(fileNameBuf, "r");
        if(splitedFile == NULL){
            printf("file open error\n");
            return 0;
        }
        
        printf("file name : %s\n", fileNameBuf);
        
        char outNameBuf[60] = {0, };
        sprintf(outNameBuf, "%s", "out");
        sprintf(outNameBuf + strlen(outNameBuf), "_%lld.txt", splitNum);
        FILE*outFile = fopen(outNameBuf, "a");
        
        printf("out name : %s\n", outNameBuf);
        
        int testcase = 5, t = 0;
        
        for(t = 0; t < testcase; t++){
            printf("iter %d\n", t);
            fseek(splitedFile, 0, SEEK_SET);
            double sum = 0;
            int cnt = 0, threshold = 100;
            int j = 0;
            while(1){
                int tmp = 0;
                //int eof = fscanf(splitedFile, "%d", &tmp);
                
                char tmpChar[60] = {0, };
                if(!fgets(tmpChar, 59, splitedFile)){
                    printf("%lf\n", sum);
                    break;
                }
                tmp = atoi(tmpChar);
                
                sum += tmp;
                
                cnt++;
            }
            //?????? ??????.
            
            printf("%lld one iter done\n", splitNum);
            //go to start point of file
            fseek(splitedFile, 0, SEEK_SET);
            
            //measure time
            struct timespec start, finish, delta;
            clock_gettime(CLOCK_REALTIME, &start);
            
            sum = 0;
            while(1){
                int tmp = 0;
                //int eof = fscanf(splitedFile, "%d", &tmp);
                char tmpChar[60] = {0, };
                if(!fgets(tmpChar, 59, splitedFile)){
                    printf("%lf\n", sum);
                    break;
                }
                tmp = atoi(tmpChar);
                
                sum += tmp;
                
            }
            clock_gettime(CLOCK_REALTIME, &finish);
            sub_timespec(start, finish, &delta);
            //time measure end
            printf("%lld two iter done\n", splitNum);
            
            fprintf(outFile, "child [PID: %d] process finished\n", getpid());
            fprintf(outFile, "latency : %ld.%.9ld\n", delta.tv_sec, delta.tv_nsec);
        }
        fclose(splitedFile);
        fclose(outFile);
    }
    
    return 0;
}
