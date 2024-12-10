#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PLANES 100
#define MAX_TYPES 5
#define MAX_FLIGHTS 100

#ifndef QUEUE_H
#define QUEUE_H

#define MAX_QUEUE_SIZE 1000

typedef struct {
    void *contents[MAX_QUEUE_SIZE];
    unsigned front, rear;
} queue;

queue *queue_new(void);
void queue_free(queue *);
int queue_send(queue *, void *);
void *queue_receive(queue *);
void *queue_front(queue *);
unsigned queue_size(queue *);
int queue_is_empty(queue *);

#endif

// Queue Implementation
queue *queue_new(void) {
    queue *q = malloc(sizeof(queue));
    if (!q) return NULL;
    q->front = 0;
    q->rear = 0;
    return q;
}

void queue_free(queue *q) {
    free(q);
}

int queue_send(queue *q, void *p) {
    if (queue_size(q) >= MAX_QUEUE_SIZE - 1)
        return -1;
    q->contents[q->rear] = p;
    q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
    return 0;
}

void *queue_receive(queue *q) {
    if (queue_is_empty(q))
        return NULL;
    void *p = q->contents[q->front];
    q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    return p;
}

void *queue_front(queue *q) {
    if (queue_is_empty(q)) return NULL;
    return q->contents[q->front];
}

unsigned queue_size(queue *q) {
    return (MAX_QUEUE_SIZE + q->rear - q->front) % MAX_QUEUE_SIZE;
}

int queue_is_empty(queue *q) {
    return q->front == q->rear;
}

typedef struct {
    int flightID;
    char departureTime[10];
    char arrivalTime[10];
    char destination[50];
    double distance;
    int pilotNumber;
    int teamID;
    int availableSeats;
} Flight;

typedef struct {
    int planeID;
    char planetype[50];
    char airline[50];
    char fleetName[50];
    char engineType[50];
    int capacity;
    double range;
} plane;

typedef struct {
    int teamID;
    char membersID[100]; // ids of team members
    int numberOfMembers;
} Team;

void createPlane(queue *planeQueue);
void showPlaneDetails(queue *planeQueue);
void countPlanesByType(queue *planeQueue, const char *types[], int counts[]);
void createFlight(Flight flights[], int *flightCount);
void updateFlight(Flight flights[], int flightCount);
void deleteFlight(Flight flights[], int *flightCount);
void showFlightDetails(Flight flights[], int flightCount);
void saveData(Flight flights[], int flightCount);
void loadData(Flight flights[], int *flightCount);
void displayMonthlyProfits(int months, float revenues[], float expenses[]);

// Plane management functions
void createPlane(queue *planeQueue) {
    plane *newPlane = malloc(sizeof(plane));
    if (!newPlane) {
        printf("Error: Memory allocation failed.\n");
        return;
    }

    printf("Enter Plane ID: ");
    scanf("%d", &newPlane->planeID);
    printf("Enter Airline Name: ");
    scanf(" %s", newPlane->airline);
    printf("Enter Fleet Name: ");
    scanf(" %s", newPlane->fleetName);
    printf("Enter Plane Type (Jet, Propeller, Cargo, Military, Passenger): ");
    scanf(" %s", newPlane->planetype);
    printf("Enter Engine Type: ");
    scanf(" %s", newPlane->engineType);
    printf("Enter Capacity: ");
    scanf("%d", &newPlane->capacity);
    printf("Enter Range (in km): ");
    scanf("%lf", &newPlane->range);

    if (queue_send(planeQueue, newPlane) == -1) {
        printf("Error: Queue is full. Cannot add new plane.\n");
        free(newPlane);
    } else {
        printf("Plane added successfully!\n");
    }
}

void showPlaneDetails(queue *planeQueue) {
    printf("\nPlane Details:\n");
    unsigned size = queue_size(planeQueue);
    for (unsigned i = 0; i < size; i++) {
        plane *currentPlane = queue_receive(planeQueue);
        if (currentPlane) {
            printf("Plane ID: %d\n", currentPlane->planeID);
            printf("Airline: %s\n", currentPlane->airline);
            printf("Fleet Name: %s\n", currentPlane->fleetName);
            printf("Plane Type: %s\n", currentPlane->planetype);
            printf("Engine Type: %s\n", currentPlane->engineType);
            printf("Capacity: %d\n", currentPlane->capacity);
            printf("Range: %.2f km\n", currentPlane->range);
            printf("-------------------------\n");
            queue_send(planeQueue, currentPlane); // Re-add to the queue.
        }
    }
}

void countPlanesByType(queue *planeQueue, const char *types[], int counts[]) {
    for (int i = 0; i < MAX_TYPES; i++) {
        counts[i] = 0;
    }

    unsigned size = queue_size(planeQueue);
    for (unsigned i = 0; i < size; i++) {
        plane *currentPlane = queue_receive(planeQueue);
        if (currentPlane) {
            for (int j = 0; j < MAX_TYPES; j++) {
                if (strcmp(currentPlane->planetype, types[j]) == 0) {
                    counts[j]++;
                    break;
                }
            }
            queue_send(planeQueue, currentPlane); // Re-add to the queue.
        }
    }
}

// Flight management functions
void createFlight(Flight flights[], int *flightCount) {
    printf("Enter Flight ID: ");
    scanf("%d", &flights[*flightCount].flightID);
    printf("Enter Departure Time (HH:MM): ");
    scanf("%s", flights[*flightCount].departureTime);
    printf("Enter Arrival Time (HH:MM): ");
    scanf("%s", flights[*flightCount].arrivalTime);
    printf("Enter Destination: ");
    scanf("%s", flights[*flightCount].destination);
    printf("Enter Distance (in km): ");
    scanf("%lf", &flights[*flightCount].distance);
    printf("Enter Pilot Number: ");
    scanf("%d", &flights[*flightCount].pilotNumber);
    printf("Enter Team ID: ");
    scanf("%d", &flights[*flightCount].teamID);
    printf("Enter Available Seats: ");
    scanf("%d", &flights[*flightCount].availableSeats);

    (*flightCount)++;
    printf("Flight created successfully!\n");
}

void updateFlight(Flight flights[], int flightCount) {
    int flightID, found = 0;
    printf("Enter Flight ID to update: ");
    scanf("%d", &flightID);

    for (int i = 0; i < flightCount; i++) {
        if (flights[i].flightID == flightID) {
            printf("Enter New Departure Time (HH:MM): ");
            scanf("%s", flights[i].departureTime);
            printf("Enter New Arrival Time (HH:MM): ");
            scanf("%s", flights[i].arrivalTime);
            printf("Enter New Destination: ");
            scanf("%s", flights[i].destination);
            printf("Enter New Distance (in km): ");
            scanf("%lf", &flights[i].distance);
            printf("Enter New Available Seats: ");
            scanf("%d", &flights[i].availableSeats);

            found = 1;
            printf("Flight updated successfully!\n");
            break;
        }
    }
    if (!found) {
        printf("Flight ID not found.\n");
    }
}

void deleteFlight(Flight flights[], int *flightCount) {
    int flightID, found = 0;
    printf("Enter Flight ID to delete: ");
    scanf("%d", &flightID);

    for (int i = 0; i < *flightCount; i++) {
        if (flights[i].flightID == flightID) {
            for (int j = i; j < *flightCount - 1; j++) {
                flights[j] = flights[j + 1];
            }
            (*flightCount)--; // Decrease the flight count
            found = 1;
            printf("Flight deleted successfully!\n");
            break;
        }
    }
    if (!found) {
        printf("Flight ID not found.\n");
    }
}

void showFlightDetails(Flight flights[], int flightCount) {
    printf("\nFlight Details:\n");
    for (int i = 0; i < flightCount; i++) {
        printf("Flight ID: %d\n", flights[i].flightID);
        printf("Departure Time: %s\n", flights[i].departureTime);
        printf("Arrival Time: %s\n", flights[i].arrivalTime);
        printf("Destination: %s\n", flights[i].destination);
        printf("Distance: %.2f km\n", flights[i].distance);
        printf("Pilot Number: %d\n", flights[i].pilotNumber);
        printf("Team ID: %d\n", flights[i].teamID);
        printf("Available Seats: %d\n", flights[i].availableSeats);
        printf("-------------------------\n");
    }
}

void saveData(Flight flights[], int flightCount) {
    FILE *file = fopen("flights.txt", "w");
    if (file == NULL) {
        printf("Error: Unable to open file for writing.\n");
        return;
    }
    for (int i = 0; i < flightCount; i++) {
        fprintf(file, "%d %s %s %s %.2lf %d %d %d\n",
                flights[i].flightID,
                flights[i].departureTime,
                flights[i].arrivalTime,
                flights[i].destination,
                flights[i].distance,
                flights[i].pilotNumber,
                flights[i].teamID,
                flights[i].availableSeats);
    }
    fclose(file);
    printf("Data saved successfully.\n");
}

void loadData(Flight flights[], int *flightCount) {
    FILE *file = fopen("flights.txt", "r");
    if (file) {
        *flightCount = 0;
        while (fscanf(file, "%d %s %s %s %lf %d %d %d",
                      &flights[*flightCount].flightID,
                      flights[*flightCount].departureTime,
                      flights[*flightCount].arrivalTime,
                      flights[*flightCount].destination,
                      &flights[*flightCount].distance,
                      &flights[*flightCount].pilotNumber,
                      &flights[*flightCount].teamID,
                      &flights[*flightCount].availableSeats) == 8) {
            (*flightCount)++;
        }
        fclose(file);
        printf("Data loaded successfully.\n");
    }
}

void displayMonthlyProfits(int months, float revenues[], float expenses[]) {
    printf("\nTotal Monthly Profits:\n");
    for (int i = 0; i < months; i++) {
        float profit = revenues[i] - expenses[i];
        printf("Month %d: Profit = %.2f\n", i + 1, profit);
    }
}

int main() {
    Flight flights[100];
    queue *planeQueue = queue_new();
    int choice;
    const char *types[MAX_TYPES] = {"Jet", "Propeller", "Cargo", "Military", "Passenger"};
    int counts[MAX_TYPES] = {0};

    if (!planeQueue) {
        printf("Error: Could not create the plane queue.\n");
        return 1;
    }

    int flightCount = 0;

    loadData(flights, &flightCount);
    float revenues[12], expenses[12];

    do {
        printf("\nFlight  Management System\n");
        printf("1. Create New Flight\n");
        printf("2. Update Flight Data\n");
        printf("3. Delete a Flight\n");
        printf("4. Show Flight Details\n");
        printf("5. Create a New Plane\n");
        printf("6. Show Plane Details\n");
        printf("7. Count Planes by Type\n");
        printf("8. Enter Revenues and Expenses\n");
        printf("9. Display Monthly Profits\n");
        printf("10. Save and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createFlight(flights, &flightCount);
                break;
            case 2:
                updateFlight(flights, flightCount);
                break;
            case 3:
                deleteFlight(flights, &flightCount);
                break;
            case 4:
                showFlightDetails(flights, flightCount);
                break;
            case 5:
                createPlane(planeQueue);
                break;
            case 6:
                showPlaneDetails(planeQueue);
                break;
            case 7:
                countPlanesByType(planeQueue, types, counts);
                printf("\nPlane Type Count:\n");
                for (int i = 0; i < MAX_TYPES; i++) {
                    printf("Type %s: %d\n", types[i], counts[i]);
                }
                break;
            case 8:
                printf("\nEnter Revenues and Expenses for 12 months:\n");
                for (int i = 0; i < 12; i++) {
                    printf("Month %d Revenue: ", i + 1);
                    scanf("%f", &revenues[i]);
                    printf("Month %d Expense: ", i + 1);
                    scanf("%f", &expenses[i]);
                }
                break;
            case 9:
                displayMonthlyProfits(12, revenues, expenses);
                break;
            case 10:
                saveData(flights, flightCount);
                printf("Data saved successfully. Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 10);

    return 0;
}
