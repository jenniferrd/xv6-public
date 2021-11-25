//User program to compare walkers
#include "types.h"
#include "user.h"
#include "fcntl.h"

void runBothWalkers(int dir_in, int ino_in){
	if(fork()==0){
		close(1);
		dup(dir_in);

		char *child_argv[] = {"directoryWalker",(char*)0};
		exec(child_argv[0],child_argv);

	}

	if(fork()==0){
		close(1);
		dup(ino_in);

		char *child_argv[] = {"inodeTBWalker",(char*)0};
		exec(child_argv[0],child_argv);

	}


}

int main(void){
	int dir_pipes[2], ino_pipes[2];
	pipe(dir_pipes);
	pipe(ino_pipes);

	runBothWalkers(dir_pipes[1], ino_pipes[1]);
	int dir_out = dir_pipes[0], ino_out = ino_pipes[0];
	//to avoid complaints at compiling time
	ino_out++;
	dir_out++;
	wait();
	wait();

	exit();

}
