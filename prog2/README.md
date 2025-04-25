# Bus Transportation System Simulator

A C-based simulation program that models buses traveling between stops and transporting passengers according to priority queue principles.

## Compilation

```
make
```

## Execution

```
./prog2 < busconfig.in
```

## Input Files

- `busconfig.in` - Main configuration file with stops and bus information
- `stopX.in` - Passenger information for each stop (X is the stop letter)

### Input Format

#### busconfig.in
```
A,B,C       # Comma-separated list of stops
2           # Number of buses
Bus0|1,2|A|ABC   # Bus name|business seats,economy seats|starting stop|route
Bus1|2,3|C|BC    # Another bus definition
```

#### stopX.in (e.g., stopA.in)
```
passenger_name|destinations|age|ticket_type
```

## Output Files

- `BusX.out` - Generated output file for each bus

### Output Format
```
timestamp|passenger_list
```
Where passenger_list is either "Empty" or a space-separated list of passengers in the format "name(ticket_type)".

## Algorithm

The simulation operates on these principles:
1. Buses move between stops following their routes
2. Passengers board based on priority (business class first, then by age)
3. Only passengers whose next destination is served by the bus can board
4. At each stop, unloading happens before loading

## File Structure

- `simulator.c` - Main simulation implementation
- `PriorityQueue.h/c` - Priority queue interface for passenger management
- `Heap.h/c` - Heap implementation used by the priority queue 