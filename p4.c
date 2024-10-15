#define USE_SHM

#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#define PORT 11111
#define WAIT_TIME 1000
#define SHM_NAME "/dev/uio0"
#define SHM_SIZE 524288 // 512 * 1024
// #define SHM_SIZE 4096
#define SHM_FLAG_SPACE 1024
#define SHM_VM_INFO 0
#define SHM_DP_PACKET2 131072 // 128 * 1024
#define SHM_PACKET 262144 // 256 * 1024
#define SHM_RESULT 393216 // 384 * 1024

int main(){
    // int fd = shm_open(SHM_NAME, O_RDWR, 0666);

    // if (fd == -1) {
    //     perror("shm_open");
    //     exit(EXIT_FAILURE);
    // }

    int fd = open("/dev/uio0", O_RDWR);
    if (fd < 0) {
        perror("open");
        exit(1);
    }

    printf("fd: %d，SHM_SIZE: %d\n", fd, SHM_SIZE);

    // if (ftruncate(fd, SHM_SIZE) == -1) {
    //     perror("ftruncate");
    //     exit(EXIT_FAILURE);
    // }

    char *shm_ptr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 4096);
    // if (shm_ptr == MAP_FAILED) {
    //     perror("mmap");
    //     exit(EXIT_FAILURE);
    // }
    if (shm_ptr < 0) {
        perror("mmap");
        exit(1);
    }

    printf("SHM opened.\n");

    printf("mapped to %p\n", shm_ptr);
    printf("shm: %s\n", shm_ptr);
    
    // read
    for(int i=0; i<100; i++){
        printf("shm: %s\n", shm_ptr);
        usleep(WAIT_TIME*100);
    }

    // // send
    strcpy(shm_ptr, "this is result");
    printf("wrote\n");
    printf("shm: %s\n", shm_ptr);

    // dp_packet2
    // dp_packet2 = (struct dp_packet_p4*)malloc(dp_packet2_size);
    // if(dp_packet2 == NULL){
    //     fprintf(stderr, "ERROR: failed to malloc() 1\n");
    //     exit(EXIT_FAILURE);
    // }

    // memset(dp_packet2, 0, dp_packet2_size);

    // while (*((char *)shm_ptr + SHM_DP_PACKET2) != 1) {
    //     printf("d: %d\n", *((char *)shm_ptr + SHM_DP_PACKET2));
    //     usleep(WAIT_TIME);
    // }
    // printf("process started.\n");
    // memcpy(dp_packet2, shm_ptr+SHM_DP_PACKET2+SHM_FLAG_SPACE, dp_packet2_size);
    // *((char *)shm_ptr + SHM_DP_PACKET2) = 0;

    // packet
    // if(dp_packet2->allocated_ == 0){
    //     result[0]='3';
    //     result[1]='\0';
    //     printf("allocated_ is 0\n\n");
    // }else{
    //     packet = malloc(dp_packet2->allocated_);
    //     if(packet == NULL){
    //         fprintf(stderr, "ERROR: failed to malloc() 2\n");
    //         free(dp_packet2);
    //         exit(EXIT_FAILURE);
    //     }
    //     dp_packet2->base_ = packet;

    //     memset(dp_packet2->base_, 0, dp_packet2->allocated_);
    //     while (*((char *)shm_ptr + SHM_PACKET) != 1) {
    //         usleep(WAIT_TIME);
    //     }
    //     memcpy(dp_packet2->base_, shm_ptr+SHM_PACKET+SHM_FLAG_SPACE, dp_packet2->allocated_);
    //     *((char *)shm_ptr + SHM_PACKET) = 0;

    //     struct standard_metadata std_meta = { .packet_length = dp_packet2->allocated_ };

    //     fn_ret = fn(dp_packet2, &std_meta);

    //     result[0]='0'+fn_ret;
    //     result[1]='\0';
        
    //     free(packet);
    // }

    // result
    // while (*((char *)shm_ptr + SHM_RESULT) != 0) {
    //     usleep(WAIT_TIME);
    // }
    // memcpy(shm_ptr+SHM_RESULT+SHM_FLAG_SPACE, result, sizeof(result));
    // *((char *)shm_ptr + SHM_RESULT) = 1;

    // free(dp_packet2);

    munmap(shm_ptr, SHM_SIZE);
    close(fd);

    return 0;
}
