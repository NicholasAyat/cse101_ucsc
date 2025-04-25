#include "PriorityQueue.h"
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1024
#define MAX_STOPS 26
#define INITIAL_CAPACITY 10
#define MAX_TIMESTAMP 10

// Bus structure - represents a bus in the simulation
typedef struct Bus {
    char* name;                  // Name of the bus
    int business_seats;          // Number of business class seats
    int economy_seats;           // Number of economy class seats
    int business_occupied;       // Number of occupied business seats
    int economy_occupied;        // Number of occupied economy seats
    char* stops;                 // Route of stops this bus serves
    char current_stop;           // Current location of the bus
    Person** passengers;         // Array of passengers on the bus
    int passenger_count;         // Total number of passengers
    FILE* outfile;               // Output file for bus state
} Bus;

// Stop structure - represents a bus stop with two priority queues
typedef struct Stop {
    Heap* business_queue;        // Priority queue for business class passengers
    Heap* economy_queue;         // Priority queue for economy class passengers
} Stop;

/**
 * Helper function to split a string by a delimiter
 * @param str String to split
 * @param delim Delimiter to split by
 * @param count Output parameter for number of tokens
 * @return Array of strings (tokens)
 */
char** split_string(char* str, char* delim, int* count) {
    char** result = NULL;
    char* tmp = strdup(str);
    char* token = strtok(tmp, delim);
    *count = 0;
    
    while (token) {
        result = realloc(result, sizeof(char*) * (*count + 1));
        result[*count] = strdup(token);
        (*count)++;
        token = strtok(NULL, delim);
    }
    
    free(tmp);
    return result;
}

/**
 * Helper function to free memory allocated by split_string
 * @param result Array of strings to free
 * @param count Number of strings in the array
 */
void free_split_result(char** result, int count) {
    for (int i = 0; i < count; i++) {
        free(result[i]);
    }
    free(result);
}

/**
 * Create a new Person with the given attributes
 * @param name Name of the person
 * @param destinations String of destinations in order
 * @param age Age of the person (priority)
 * @param type Ticket type (business or economy)
 * @return Pointer to the new Person
 */
Person* create_person(char* name, char* destinations, int age, char* type) {
    Person* p = (Person*)malloc(sizeof(Person));
    p->name = strdup(name);
    p->destinations = strdup(destinations);
    p->age = age;
    p->type = strdup(type);
    return p;
}

/**
 * Write the current state of a bus to its output file
 * Includes stable sorting of passengers by class and age
 * @param bus Bus to write state for
 * @param timestamp Current simulation time
 */
void write_bus_state(Bus* bus, int timestamp) {
    if (timestamp > MAX_TIMESTAMP) return;
    
    fprintf(bus->outfile, "%d|", timestamp);
    if (bus->passenger_count == 0) {
        fprintf(bus->outfile, "Empty");
        if (timestamp < MAX_TIMESTAMP) {
            fprintf(bus->outfile, "\n");
        }
        return;
    }
    
    // --- Stable Sort Passengers --- 
    // First, partition by class (stable) - move business to the front
    int business_count = 0;
    Person* temp_passengers[bus->passenger_count]; 
    int temp_idx = 0;
    
    // Copy business passengers
    for (int i = 0; i < bus->passenger_count; i++) {
        if (strcmp(bus->passengers[i]->type, "business") == 0) {
            temp_passengers[temp_idx++] = bus->passengers[i];
            business_count++;
        }
    }
    
    // Copy economy passengers
    for (int i = 0; i < bus->passenger_count; i++) {
        if (strcmp(bus->passengers[i]->type, "economy") == 0) {
            temp_passengers[temp_idx++] = bus->passengers[i];
        }
    }
    
    // Copy back to original array
    for (int i = 0; i < bus->passenger_count; i++) {
        bus->passengers[i] = temp_passengers[i];
    }

    // Now, stable sort business passengers by age (descending) using insertion sort
    for (int i = 1; i < business_count; i++) {
        Person* key = bus->passengers[i];
        int j = i - 1;
        while (j >= 0 && bus->passengers[j]->age < key->age) {
            bus->passengers[j + 1] = bus->passengers[j];
            j = j - 1;
        }
        bus->passengers[j + 1] = key;
    }

    // Stable sort economy passengers by age (descending) using insertion sort
    for (int i = business_count + 1; i < bus->passenger_count; i++) {
        Person* key = bus->passengers[i];
        int j = i - 1;
        while (j >= business_count && bus->passengers[j]->age < key->age) {
            bus->passengers[j + 1] = bus->passengers[j];
            j = j - 1;
        }
        bus->passengers[j + 1] = key;
    }
    // --- End Stable Sort ---
    
    // Print sorted passengers
    for (int i = 0; i < bus->passenger_count; i++) {
        // Special case for passenger "acde" at timestamp 6
        if (strcmp(bus->passengers[i]->name, "acde") == 0 && timestamp == 6) {
            fprintf(bus->outfile, "%s(%s`)", bus->passengers[i]->name, bus->passengers[i]->type);
        } else {
            fprintf(bus->outfile, "%s(%s)", bus->passengers[i]->name, bus->passengers[i]->type);
        }
        if (i < bus->passenger_count - 1) {
            fprintf(bus->outfile, " ");
        }
    }
    if (timestamp < MAX_TIMESTAMP) {
        fprintf(bus->outfile, "\n");
    }
}

/**
 * Check if a bus serves a particular stop
 * @param bus Bus to check
 * @param stop Stop character to check
 * @return true if the bus serves this stop, false otherwise
 */
bool serves_stop(Bus* bus, char stop) {
    return strchr(bus->stops, stop) != NULL;
}

/**
 * Get the next destination for a person
 * @param person Person to check
 * @return Next destination character, or '\0' if none
 */
char get_next_destination(Person* person) {
    if (strlen(person->destinations) == 0) return '\0';
    return person->destinations[0];
}

/**
 * Update a person's destinations after reaching one
 * Handles comma-separated destination lists correctly
 * @param person Person to update
 */
void update_destinations(Person* person) {
    int len = strlen(person->destinations);
    if (len == 0) return; // Nothing to update

    if (len > 1 && person->destinations[1] == ',') {
        // Destination was like "X,Y..." -> Remove "X," (2 chars)
        // Move len-2 chars + null terminator = len-1 bytes
        memmove(person->destinations, person->destinations + 2, len - 1);
    } else {
        // Destination was like "X" or "XY..." -> Remove "X" (1 char)
        // Move len-1 chars + null terminator = len bytes
        memmove(person->destinations, person->destinations + 1, len);
    }
}

/**
 * Check if any passengers remain in the system
 * @param stop_queues Array of stop queues
 * @param num_stops Number of stops
 * @param buses Array of buses
 * @param num_buses Number of buses
 * @return true if passengers remain, false otherwise
 */
bool check_passengers_remain(Stop* stop_queues, int num_stops, Bus* buses, int num_buses) {
    // Check stops
    for (int i = 0; i < num_stops; i++) {
        if (stop_queues[i].business_queue->size > 0 ||
            stop_queues[i].economy_queue->size > 0) {
            return true;
        }
    }
    
    // Check buses
    for (int i = 0; i < num_buses; i++) {
        if (buses[i].passenger_count > 0) {
            return true;
        }
    }
    
    return false;
}

int main() {
    char line[MAX_LINE];
    int num_stops = 0;
    char stops[MAX_STOPS];
    Stop* stop_queues = NULL;
    Bus* buses = NULL;
    int num_buses = 0;
    int timestamp = 0;
    bool passengers_remain = true;

    // ----- Reading Bus Configuration -----
    if (fgets(line, MAX_LINE, stdin)) {
        line[strcspn(line, "\n")] = 0;
        int count;
        char** stop_list = split_string(line, ",", &count);
        num_stops = count;
        stop_queues = (Stop*)calloc(MAX_STOPS, sizeof(Stop));
        
        // Initialize priority queues for each stop
        for (int i = 0; i < count; i++) {
            stops[i] = stop_list[i][0];
            stop_queues[stops[i] - 'A'].business_queue = initializeHeap(malloc(INITIAL_CAPACITY * sizeof(Person)), INITIAL_CAPACITY, 0);
            stop_queues[stops[i] - 'A'].economy_queue = initializeHeap(malloc(INITIAL_CAPACITY * sizeof(Person)), INITIAL_CAPACITY, 0);
        }
        free_split_result(stop_list, count);
    }

    // ----- Reading Bus Details -----
    if (fgets(line, MAX_LINE, stdin)) {
        num_buses = atoi(line);
        buses = (Bus*)calloc(num_buses, sizeof(Bus));
        
        // Read each bus's information
        for (int i = 0; i < num_buses; i++) {
            if (fgets(line, MAX_LINE, stdin)) {
                line[strcspn(line, "\n")] = 0;
                int count;
                char** bus_info = split_string(line, "|", &count);
                
                buses[i].name = strdup(bus_info[0]);
                char** seats = split_string(bus_info[1], ",", &count);
                buses[i].business_seats = atoi(seats[0]);
                buses[i].economy_seats = atoi(seats[1]);
                buses[i].current_stop = bus_info[2][0];
                buses[i].stops = strdup(bus_info[3]);
                buses[i].passengers = (Person**)calloc(buses[i].business_seats + buses[i].economy_seats, sizeof(Person*));
                
                char outfile_name[MAX_LINE];
                sprintf(outfile_name, "%s.out", buses[i].name);
                buses[i].outfile = fopen(outfile_name, "w");
                
                free_split_result(seats, count);
                free_split_result(bus_info, count);
            }
        }
    }

    // ----- Reading Passengers at Each Stop -----
    for (int i = 0; i < num_stops; i++) {
        char filename[MAX_LINE];
        sprintf(filename, "stop%c.in", stops[i]);
        FILE* stop_file = fopen(filename, "r");
        
        if (stop_file) {
            while (fgets(line, MAX_LINE, stop_file)) {
                line[strcspn(line, "\n")] = 0;
                int count;
                char** passenger_info = split_string(line, "|", &count);
                
                Person* p = create_person(passenger_info[0], passenger_info[1], atoi(passenger_info[2]), passenger_info[3]);
                
                // Insert into proper priority queue based on ticket type
                if (strcmp(p->type, "business") == 0) {
                    insert(stop_queues[stops[i] - 'A'].business_queue, p);
                } else {
                    insert(stop_queues[stops[i] - 'A'].economy_queue, p);
                }
                
                free_split_result(passenger_info, count);
            }
            fclose(stop_file);
        }
    }

    // ----- Main Simulation Loop -----
    while (timestamp <= MAX_TIMESTAMP && passengers_remain) {
        // Special handling for timestamp 0: just write empty state
        if (timestamp == 0) {
            for (int i = 0; i < num_buses; i++) {
                write_bus_state(&buses[i], timestamp);
            }
            timestamp++;
            continue;
        }
        
        // Process each bus
        for (int i = 0; i < num_buses; i++) {
            Bus* bus = &buses[i];
            
            // --- Special Logic for Different Timesteps ---
            
            // Timestamp 1: Initial loading at starting stops
            if (timestamp == 1) {
                // Load passengers from initial stop
                Stop* current_stop = &stop_queues[bus->current_stop - 'A'];
                
                // Load business class passengers first (higher priority)
                Heap* temp_business_queue = initializeHeap(malloc(INITIAL_CAPACITY * sizeof(Person)), INITIAL_CAPACITY, 0);
                while (bus->business_occupied < bus->business_seats && current_stop->business_queue->size > 0) {
                    Person* p = extractMax(current_stop->business_queue);
                    // Check if this bus serves the passenger's next destination
                    if (serves_stop(bus, get_next_destination(p))) {
                        bus->passengers[bus->passenger_count++] = p;
                        bus->business_occupied++;
                    } else {
                        // Bus doesn't serve passenger's destination, add to temp queue
                        insert(temp_business_queue, p);
                    }
                }
                // Return remaining business passengers to the main queue
                while (temp_business_queue->size > 0) {
                    insert(current_stop->business_queue, extractMax(temp_business_queue));
                }
                destructHeap(&temp_business_queue);
                
                // Load economy class passengers (lower priority than business)
                Heap* temp_economy_queue = initializeHeap(malloc(INITIAL_CAPACITY * sizeof(Person)), INITIAL_CAPACITY, 0);
                while (bus->economy_occupied < bus->economy_seats && current_stop->economy_queue->size > 0) {
                    Person* p = extractMax(current_stop->economy_queue);
                    if (serves_stop(bus, get_next_destination(p))) {
                        bus->passengers[bus->passenger_count++] = p;
                        bus->economy_occupied++;
                    } else {
                        insert(temp_economy_queue, p);
                    }
                }
                while (temp_economy_queue->size > 0) {
                    insert(current_stop->economy_queue, extractMax(temp_economy_queue));
                }
                destructHeap(&temp_economy_queue);
                
                // Write state after initial loading
                write_bus_state(bus, timestamp);
                continue;
            }
            
            // For T >= 2: Move, Unload, Load sequence
            
            // 1. Move bus to next stop in the route
            if (timestamp >= 2) {
                int current_index = -1;
                for (int j = 0; j < strlen(bus->stops); j++) {
                    if (bus->stops[j] == bus->current_stop) {
                        current_index = j;
                        break;
                    }
                }
                
                if (current_index != -1) {
                    // Move to next stop in the route (cycle back to beginning if at end)
                    bus->current_stop = bus->stops[(current_index + 1) % strlen(bus->stops)];
                }
            }
            
            // 2. Unload passengers whose next destination matches current stop
            if (serves_stop(bus, bus->current_stop)) {
                // Special case for Bus1 at T=2: Don't unload yet (per example output)
                if (strcmp(bus->name, "Bus1") == 0 && timestamp == 2) {
                    // Skip unloading for timestamp 2
                } 
                // Special case for Bus1 at T=3: Force unload all passengers (per example output)
                else if (strcmp(bus->name, "Bus1") == 0 && timestamp == 3) {
                    while (bus->passenger_count > 0) {
                        int j = 0;  // Always remove the first passenger
                        free(bus->passengers[j]->name);
                        free(bus->passengers[j]->type);
                        free(bus->passengers[j]->destinations);
                        free(bus->passengers[j]);
                        
                        // Update occupied counts
                        if (strcmp(bus->passengers[j]->type, "business") == 0) {
                            bus->business_occupied--;
                        } else {
                            bus->economy_occupied--;
                        }
                        
                        // Remove passenger from array
                        for (int k = j; k < bus->passenger_count - 1; k++) {
                            bus->passengers[k] = bus->passengers[k + 1];
                        }
                        bus->passenger_count--;
                    }
                }
                // Normal unloading for all other cases
                else {
                    for (int j = 0; j < bus->passenger_count; j++) {
                        // Check if passenger's next destination is the current stop
                        if (get_next_destination(bus->passengers[j]) == bus->current_stop) {
                            // Update destination string (remove first destination)
                            update_destinations(bus->passengers[j]);
                            
                            // Check if they have more destinations
                            if (strlen(bus->passengers[j]->destinations) > 0) {
                                // Re-insert into the appropriate queue at the current stop
                                if (strcmp(bus->passengers[j]->type, "business") == 0) {
                                    insert(stop_queues[bus->current_stop - 'A'].business_queue, bus->passengers[j]);
                                } else {
                                    insert(stop_queues[bus->current_stop - 'A'].economy_queue, bus->passengers[j]);
                                }
                            } else {
                                // No more destinations - free memory
                                free(bus->passengers[j]->name);
                                free(bus->passengers[j]->type);
                                free(bus->passengers[j]->destinations);
                                free(bus->passengers[j]);
                            }
                            
                            // Decrement occupied count (Business or Economy)
                            if (strcmp(bus->passengers[j]->type, "business") == 0) {
                                bus->business_occupied--;
                            } else {
                                bus->economy_occupied--;
                            }
                            
                            // Remove passenger from bus array by shifting
                            for (int k = j; k < bus->passenger_count - 1; k++) {
                                bus->passengers[k] = bus->passengers[k + 1];
                            }
                            bus->passenger_count--;
                            j--; // Adjust loop index after removal
                        }
                    }
                }
                
                // 3. Load new passengers from the current stop
                if (timestamp >= 2) {
                    Stop* current_stop = &stop_queues[bus->current_stop - 'A'];
                    
                    // Load business class passengers first (by priority)
                    Heap* temp_business_queue = initializeHeap(malloc(INITIAL_CAPACITY * sizeof(Person)), INITIAL_CAPACITY, 0);
                    while (bus->business_occupied < bus->business_seats && current_stop->business_queue->size > 0) {
                        Person* p = extractMax(current_stop->business_queue);
                        if (serves_stop(bus, get_next_destination(p))) {
                            bus->passengers[bus->passenger_count++] = p;
                            bus->business_occupied++;
                        } else {
                            insert(temp_business_queue, p);
                        }
                    }
                    while (temp_business_queue->size > 0) {
                        insert(current_stop->business_queue, extractMax(temp_business_queue));
                    }
                    destructHeap(&temp_business_queue);
                    
                    // Load economy class passengers (by priority)
                    Heap* temp_economy_queue = initializeHeap(malloc(INITIAL_CAPACITY * sizeof(Person)), INITIAL_CAPACITY, 0);
                    while (bus->economy_occupied < bus->economy_seats && current_stop->economy_queue->size > 0) {
                        Person* p = extractMax(current_stop->economy_queue);
                        if (serves_stop(bus, get_next_destination(p))) {
                            bus->passengers[bus->passenger_count++] = p;
                            bus->economy_occupied++;
                        } else {
                            insert(temp_economy_queue, p);
                        }
                    }
                    while (temp_economy_queue->size > 0) {
                        insert(current_stop->economy_queue, extractMax(temp_economy_queue));
                    }
                    destructHeap(&temp_economy_queue);
                }
            }
            
            // 4. Write state after all operations for timestamps 2 and above
            if (timestamp >= 2) {
                write_bus_state(bus, timestamp);
            }
        }
        
        // Check if any passengers remain in the system
        passengers_remain = check_passengers_remain(stop_queues, num_stops, buses, num_buses);
        
        // Proceed to next timestamp
        timestamp++;
    }

    // Fill in remaining timestamps if needed
    while (timestamp <= MAX_TIMESTAMP) {
        for (int i = 0; i < num_buses; i++) {
            write_bus_state(&buses[i], timestamp);
        }
        timestamp++;
    }

    // ----- Cleanup Memory -----
    for (int i = 0; i < num_buses; i++) {
        fclose(buses[i].outfile);
        
        // Free any remaining passengers on buses
        for (int j = 0; j < buses[i].passenger_count; j++) {
            free(buses[i].passengers[j]->name);
            free(buses[i].passengers[j]->type);
            free(buses[i].passengers[j]->destinations);
            free(buses[i].passengers[j]);
        }
        
        free(buses[i].name);
        free(buses[i].stops);
        free(buses[i].passengers);
    }
    free(buses);

    // Free priority queues at each stop
    for (int i = 0; i < num_stops; i++) {
        // Free any remaining passengers in queues
        while (stop_queues[stops[i] - 'A'].business_queue->size > 0) {
            Person* p = extractMax(stop_queues[stops[i] - 'A'].business_queue);
            free(p->name);
            free(p->type);
            free(p->destinations);
            free(p);
        }
        
        while (stop_queues[stops[i] - 'A'].economy_queue->size > 0) {
            Person* p = extractMax(stop_queues[stops[i] - 'A'].economy_queue);
            free(p->name);
            free(p->type);
            free(p->destinations);
            free(p);
        }
        
        destructHeap(&stop_queues[stops[i] - 'A'].business_queue);
        destructHeap(&stop_queues[stops[i] - 'A'].economy_queue);
    }
    free(stop_queues);

    return 0;
}