#include "zepplin.h"

int main(){
	int fd;
	if (fd = shm_open(PARATAGECOLIS, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR))
	{
		close(fd);
	}
}
