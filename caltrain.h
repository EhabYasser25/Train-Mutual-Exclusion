#include <pthread.h>

struct station {
    int waitingPassengers;
    int availableSeats;
    int freeSeats;
    pthread_mutex_t mutex;
    pthread_cond_t trainArrival;
    pthread_cond_t trainLeaving;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);