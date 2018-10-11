#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "sem_tk.h"

#define FILENAME "./sample.c"
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

	// get value of loop variable (from command-line argument)
	loop = atoi( argv[1] );

	/* Create semaphore */
	key_t sem_key;
	if ( ( sem_key = ftok( FILENAME, 1 ) ) == (key_t)-1 ) {
		perror( "ftok error" );
		exit( 1 );
	}

	int sem_id;
	sem_id = Create( sem_key );
	Initialize( sem_id );

	if ( ( shmId = shmget( IPC_PRIVATE, SIZE, IPC_CREAT|S_IRUSR|S_IWUSR ) ) < 0 ) {
		perror( "i can't get no..\n" );
		exit( 1 );
	}
	if ( ( shmPtr = shmat( shmId, 0, 0 ) ) == (void*) -1 ) {
		perror( "can't attach\n" );
		exit( 1 );
	}

	shmPtr[0] = 0;
	shmPtr[1] = 1;

	if ( !( pid = fork() ) ) {
		for ( i = 0; i < loop; i++ ) {
			// swap the contents of shmPtr[0] and shmPtr[1]
			
			/* Acquire semaphore */
			Wait( sem_id );

			temp = shmPtr[0];
			shmPtr[0] = shmPtr[1];
			shmPtr[1] = temp;

			/* Release semahpore */
			Signal( sem_id );
		}
		if ( shmdt( shmPtr ) < 0 ) {
			perror( "just can't let go\n" );
			exit( 1 );
		}
		exit( 0 );
	}
	else
		for ( i = 0; i < loop; i++ ) {
			// swap the contents of shmPtr[1] and shmPtr[0]

			/* Acquire semaphore */
			Wait( sem_id );

			temp = shmPtr[1];
			shmPtr[1] = shmPtr[0];
			shmPtr[0] = temp;

			/* Release semahpore */
			Signal( sem_id );
		}

		wait( &status );
		printf( "values: %li\t%li\n", shmPtr[0], shmPtr[1] );

		if ( shmdt( shmPtr ) < 0 ) {
			perror( "just can't let go\n" );
			exit( 1 );
		}

		if ( shmctl( shmId, IPC_RMID, 0 ) < 0 ) {
			perror( "can't deallocate\n" );
			exit( 1 );
		}

		/* Delete semaphore */
		Destroy( sem_id );

	return 0;
}
