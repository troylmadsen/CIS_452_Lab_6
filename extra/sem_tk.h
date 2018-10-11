#ifndef SEM_TK_H
#define SEM_TK_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>

/*
 * Wrapper toolkit for sem.h.
 * @Version 1.0
 * @Author Troy Madsen
 * @Author Tim DeVries
 * @Date 2018/10/10
 */

/*
 * Creates a semaphore using the specified key.
 * @param key Key of the semaphore to create.
 */
int Create( key_t key );

/*
 * Destroys a semaphore specified by the ID.
 * @param sem_id ID of the semaphore to destroy.
 */
void Destroy( int sem_id );

/*
 * Initializes the semaphore sepecified by the ID.
 * @param sem_id ID of semaphore to initialize.
 */
void Initialize( int sem_id );

/*
 * Takes a semaphore and releases the lock on it.
 * @param sem_id ID of semaphore to release.
 */
void Signal( int sem_id );

/*
 * Takes a semaphore and acquires a lock on it.
 * @param sem_id ID of semaphore to lock.
 */
void Wait( int sem_id );

#endif /* SEM_TK_H */
