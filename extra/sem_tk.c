#include "sem_tk.h"

/*
 * Wrapper toolkit for sem.h.
 * @Version 1.0
 * @Author Troy Madsen
 * @Author Tim DeVries
 * @Date 2018/10/10
 */

struct sembuf lock[] = { { 0, -1, 0 } };
struct sembuf unlock[] = { { 0, 1, 0 }};

int Create( key_t sem_key ) {
	int sem_id;

	if ( ( sem_id = semget( sem_key, 1, IPC_CREAT | S_IRUSR | S_IWUSR ) ) == -1 ) {
		perror( "Semaphore creation failure" );
		exit( 1 );
	}

	return sem_id;
}

void Destroy( int sem_id ) {
	if ( semctl( sem_id, 0, IPC_RMID, 0 ) != 0 ) {
		perror( "Semaphore removal failure" );
		exit( 1 );
	}
}

void Initialize( int sem_id ) {
	if ( semop( sem_id, unlock, 1 ) == -1 ) {
		perror( "Semaphore initialization failure" );
		exit( 1 );
	}
}

/*
 * Takes a semaphore and releases the lock on it.
 * @param sem_id ID of semaphore to release.
 */
void Signal( int sem_id ) {
	if ( semop( sem_id, unlock, 1 ) == -1 ) {
		perror( "Semaphore release failure" );
		exit( 1 );
	}
}

/*
 * Takes a semaphore and acquires a lock on it.
 * @param sem_id ID of semaphore to lock.
 */
void Wait( int sem_id ) {
	if ( semop( sem_id, lock, 1 ) == -1 ) {
		perror( "Semaphore acquisition failure" );
		exit( 1 );
	}
}
