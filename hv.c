#define USE_SHM

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <net/if.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdio.h>
#include <arpa/inet.h>
#include <syslog.h>
#include <sys/time.h>
#include <sys/mman.h>

#define WAIT_TIME 100

#define SHM_NAME "/dev/shm/ivshmem"
#define SHM_SIZE 524288 // 512 * 1024
#define SHM_FLAG_SPACE 1024
#define SHM_VM_INFO 0
#define SHM_DP_PACKET2 131072 // 128 * 1024
#define SHM_PACKET 262144 // 256 * 1024
#define SHM_RESULT 393216 // 384 * 1024

#ifdef USE_SHM
static int fd;
static char *shm_ptr;
#endif

int
prepare_shm(void)
{
    fd = open(SHM_NAME, O_RDWR);

    if (fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // if (ftruncate(fd, SHM_SIZE) == -1) {
    //     perror("ftruncate");
    //     exit(EXIT_FAILURE);
    // }

    shm_ptr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 8192);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    printf("SHM opened.\n");

    printf("mapped to %p\n", shm_ptr);
    printf("shm: %s\n", shm_ptr);

    // *((char *)shm_ptr + SHM_DP_PACKET2) = 0;
    // *((char *)shm_ptr + SHM_PACKET) = 0;
    // *((char *)shm_ptr + SHM_RESULT) = 0;

    return 0;
}

int main(){
    if ((prepare_shm()) != 0) {
        fprintf(stderr, "ERROR: Cannot open shm\n");
        return 1;
    }

    // send
    // strcpy(shm_ptr, "this is packet.");
    // printf("wrote\n");

    usleep(WAIT_TIME*100);

    // receive
    char buffer[100];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer));
    // printf("bytes_read: %ld\n", bytes_read);
    // printf("Error opening file: %s\n", strerror(errno));
    while (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        printf("%s", buffer);
        bytes_read = read(fd, buffer, sizeof(buffer));
    }

    // dp_packet2
    // while (*(shm_ptr + SHM_DP_PACKET2) != 0) {
    //     usleep(WAIT_TIME);
    // }
    // memcpy(shm_ptr+SHM_DP_PACKET2+SHM_FLAG_SPACE, &dp_packet2, size);
    // *((char *)shm_ptr + SHM_DP_PACKET2) = 1;

    // packet
    // while (*(shm_ptr + SHM_PACKET) != 0) {
    //     usleep(WAIT_TIME);
    // }
    // memcpy(shm_ptr+SHM_PACKET+SHM_FLAG_SPACE, packet_data->base_, dp_packet2.allocated_);
    // *((char *)shm_ptr + SHM_PACKET) = 1;

    //result
    // memset(result, 0, sizeof(result));
    // while (*(shm_ptr + SHM_RESULT) != 1) {
    //     usleep(WAIT_TIME);
    // }
    // memcpy(result, shm_ptr+SHM_RESULT+SHM_FLAG_SPACE, sizeof(result));
    // *((char *)shm_ptr + SHM_RESULT) = 0;

    munmap(shm_ptr, SHM_SIZE);
    close(fd);

    return 0;
}
