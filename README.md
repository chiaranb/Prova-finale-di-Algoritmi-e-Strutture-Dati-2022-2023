# FindPath
## Final Project of Algorithms and Principles of Computer Science course at Politecnico di Milano
Each station along the highway is identified by its distance from the starting point, denoted as a natural number. These stations are equipped with a fleet of electric vehicles, each having its autonomy represented by a positive integer.

The primary objective is to determine the most efficient route between two given stations with the fewest stops required. At each stop, vehicles are interchanged, utilizing those available at the service station.

It's important to note that route planning solely involves theoretical considerations; therefore, no physical relocation of vehicles occurs upon route request.

The input text file contains a sequence of commands, one per line, with the following format. All positive or zero integer values can be encoded in 32 bits.

- **`aggiungi-stazione distanza numero-auto autonomia-auto-1 ... autonomia-auto-n`**: Adds a station, located at the indicated distance, equipped with car-number vehicles, each with the indicated autonomy.

  For example:
         **`aggiungi-stazione 10 3 100 200 300`**
  adds a station 10 units away from the beginning of the highway, with a fleet of three vehicles with autonomies of 100, 200, and 300 km respectively.

  If a station already exists at       the indicated distance, the command does nothing. Expected response: **`aggiunta`** or **`non aggiunta`**.

- **`demolisci-stazione distance`**: Removes the station located at the indicated distance, if it exists. Expected response: **`demolita`** or **`non demolita`**.

- **`aggiungi-auto distanza-stazione autonomia-auto-da-aggiungere`**: If the station exists, adds a car to it. Multiple cars with the same autonomy are possible. Expected response: **`aggiunta`** or **`non aggiunta`**.

- **`rottama-auto distanza-stazione autonomia-auto-da-rottamare`**: Removes a car from the indicated station, if the station exists and has at least one car with the indicated autonomy. Expected response: **`rottamata`** or **`non rottamata`**.

- **`pianifica-percorso distanza-stazione-partenza distanza-stazione-arrivo`**: Requests planning the route with the above constraints. Expected response: the waypoints in order of travel, represented by the distances of the stations from the beginning of the highway, separated by spaces and followed by a newline at the end. Departure and arrival must be included; if they coincide, the station is printed only once. If the route does not exist, prints **`neesun percorso`**. The planning action does not alter the stations or their vehicle fleets. The given stations are definitely present.

## Tools used

- Valgrind
- Callgrind
- Massif-Visualizer
- Address-Sanitizer
- GDB
- GCC

## Software used

- Clion

Grade: 30L
