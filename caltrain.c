#include <pthread.h>
#include "caltrain.h"



void
station_init(struct station *station)
{
    station->waitingPassengers = 0;
    station->availableSeats = 0;
    station->freeSeats = 0;
    pthread_mutex_init(&station->mutex, NULL);
    pthread_cond_init(&station->trainArrival, NULL);
    pthread_cond_init(&station->trainLeaving, NULL);
}

void
station_load_train(struct station *station, int count)
{
    if(count == 0 || station->waitingPassengers == 0)
        return;
    station->freeSeats = count < station->waitingPassengers ? count : station->waitingPassengers;
    station->availableSeats = count;
    pthread_mutex_lock(&station->mutex);
    pthread_cond_broadcast(&station->trainArrival);
    if(station->freeSeats != 0 && station->waitingPassengers != 0)
        pthread_cond_wait(&station->trainLeaving, &station->mutex);
    pthread_mutex_unlock(&station->mutex);
}

void
station_wait_for_train(struct station *station)
{
    pthread_mutex_lock(&station->mutex);
    station->waitingPassengers++;
    while(station->availableSeats == 0)
        pthread_cond_wait(&station->trainArrival, &station->mutex);
    station->waitingPassengers--;
    station->availableSeats--;
    pthread_mutex_unlock(&station->mutex);
}

void
station_on_board(struct station *station)
{
    pthread_mutex_lock(&station->mutex);
    station->freeSeats--;
    if(station->freeSeats == 0)
        pthread_cond_signal(&station->trainLeaving);
    pthread_mutex_unlock(&station->mutex);
}