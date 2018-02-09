#include <stdio.h>                                                                
#include <sys/time.h>                                                             
#include <unistd.h>                                                               
#include <string.h>                                                               
#include <sys/stat.h>                                                             
#include <sys/types.h>                                                            
#include <fcntl.h>                                                                
                                                                                  
#define N_BYTES_TO_READ 8                                                         
#define N_WARMUP_RUNS 10
#define N_REAL_RUNS 1000


static const char zero[N_BYTES_TO_READ] = { 0 };                                
static char buf[N_BYTES_TO_READ];                                                      

ssize_t do_syscalls() {
  struct timeval before, after;                                                   

  memset(&buf, -1, N_BYTES_TO_READ);                                              

  gettimeofday(&before, NULL);                                                    
                                                                                  
  int fd = open("/dev/zero", O_RDONLY);                                           
  read(fd, buf, N_BYTES_TO_READ);                                                 
  close(fd);                                                                      
                                                                                  
  gettimeofday(&after, NULL);                                                     
                                                                                  
  size_t us_elapsed = (100000) * (after.tv_sec - before.tv_sec)                   
                      + (after.tv_usec - before.tv_usec);                         
  
  if (memcmp(&buf, &zero, N_BYTES_TO_READ)) return -1;
  else return us_elapsed;
}

int main(int argc, char *argv[]) {                                                
                                                                                  
  size_t this_time, sum_times = 0;

  for (size_t i = 0; i < N_REAL_RUNS; ++i) {
                                                                                  
    if ((this_time = do_syscalls()) == -1) {
      printf("Oops, read from /dev/null failed...\n");                              
      return -1;
    }                                                                               
    else {                                                                          
      sum_times += this_time;
    }                                                                               
  }

  printf("Open/read/close succeeded in, on average, %f us.\n", (float)sum_times/N_REAL_RUNS);

  return 0;                                                                       
} 
