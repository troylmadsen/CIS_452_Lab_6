#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define SIZE 16

/*
 * Continaully swaps memory spaces without issues.
 * @Version 1.0
 * @Author Troy Madsen
 * @Author Tim DeVries
 * @Date 2018/10/04
 */

int main ( int argc, char** argv )
{
	int status;
	long int i, loop, temp, *shmPtr;
	int shmId;
	pid_t pid;

	/* Create sembufs for semaphores */
	struct sembuf lock[1];
	struct sembuf unlock[1];

	/* Initialize sembufs */
	lock[0].sem_num = 0;
	lock[0].sem_op = -1;
	lock[0].sem_flg = 0;
	unlock[0].sem_num = 0;
	unlock[0].sem_op = 1;
	unlock[0].sem_flg = 0;

	// get value of loop variable (from command-line argument)
	loop = atoi( argv[1] );

	/* Create semaphore */
	key_t sem_key;
	int sem_id;
	if ( ( sem_key = ftok("./sample1.c", 1) ) == (key_t)-1 ) {
		perror( "ftok error" );
		exit( 1 );
	}

	if ( ( sem_id = semget( sem_key, 1, IPC_CREAT | S_IRUSR | S_IWUSR ) ) == -1 ) {
		perror( "semget error" );
		exit( 1 );
	}

	if ( semop( sem_id, unlock, 1 ) == -1 ) {
		perror( "semop error" );
		exit( 1 );
	}

	if ((shmId = shmget (IPC_PRIVATE, SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
		perror ("i can't get no..\n");
		exit (1);
	}
	if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
		perror ("can't attach\n");
		exit (1);
	}

	shmPtr[0] = 0;
	shmPtr[1] = 1;

	if (!(pid = fork())) {
		for (i=0; i<loop; i++) {
			// swap the contents of shmPtr[0] and shmPtr[1]
			
			/* Acquire semaphore */
			if ( semop( sem_id, lock, 1 ) == -1 ) {
				perror( "semop error" );
				exit( 1 );
			}

			temp = shmPtr[0];
			shmPtr[0] = shmPtr[1];
			shmPtr[1] = temp;

			/* Release semahpore */
			if ( semop( sem_id, unlock, 1 ) == -1 ) {
				perror( "semop error" );
				exit( 1 );
			}
		}
		if (shmdt (shmPtr) < 0) {
			perror ("just can't let go\n");
			exit (1);
		}
		exit(0);
	}
	else
		for (i=0; i<loop; i++) {
			// swap the contents of shmPtr[1] and shmPtr[0]

			/* Acquire semaphore */
			if ( semop( sem_id, lock, 1 ) == -1 ) {
				perror( "semop error" );
				exit( 1 );
			}

			temp = shmPtr[1];
			shmPtr[1] = shmPtr[0];
			shmPtr[0] = temp;

			/* Release semahpore */
			if ( semop( sem_id, unlock, 1 ) == -1 ) {
				perror( "semop error" );
				exit( 1 );
			}
		}

		wait (&status);
		printf ("values: %li\t%li\n", shmPtr[0], shmPtr[1]);

		if (shmdt (shmPtr) < 0) {
			perror ("just can't let go\n");
			exit (1);
		}

		if (shmctl (shmId, IPC_RMID, 0) < 0) {
			perror ("can't deallocate\n");
			exit(1);
		}

		/* Delete semaphore */
		if ( semctl( sem_id, 0, IPC_RMID, 0 ) != 0 ) {
			perror( "semctl error" );
			exit( 1 );
		}

	return 0;
}
