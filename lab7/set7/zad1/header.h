// Wykorzystując semafory i pamięć wspólną z IPC Systemu V oraz standardu POSIX napisz program symulujący działanie salonu fryzjerskiego: 

// W salonie dostępnych jest: 

// M fryzjerów, N foteli (przy czym M >=N) , oraz poczekalnia z P miejscami. 

// Każdy fryzjer albo oczekuje na klienta (śpi), albo obsługuje klienta o ile taki się zgłosił bądź oczekiwał w poczekalni, 
// fryzjer nie był zajęty, a w salonie dostępny był wolny fotel. 

// Kiedy do salony przychodzi klient, budzi fryzjera (jeżeli jest jakiś śpiący), po czym fryzjer znajduje wolny fotel i obsługuje klienta. 
// Jeśli nie ma wolnego fotela, klient zajmuje jedno z wolnych miejsc w poczekalni. Jeśli nie ma miejsca w poczekalni, klient odchodzi.

// Należy zsynchronizować prace fryzjerów oraz obsługę klientów w taki sposób, aby jeden fryzjer w danej chwili obsługiwał co najwyżej jednego klienta, 
// jeden klient był obsługiwany przez co najwyżej jednego fryzjera, żaden z klientów ani żaden z fryzjerów nie został zagłodzony.

// Zakładamy, że fryzjerzy obsługują kolejnych klientów w kolejności zgłaszania się (i/lub w kolejności zajmowanego miejsca w poczekalni) 
// w miarę dostępnych foteli oraz to ze każdy z fryzjerów potrafi wykonać każdą z oferowanych przez salon fryzur.  

// Przyjmij ze czas wykonania kazdej z F oferowanych przez salon fryzur wynosi kilka sekund, i ze czasy te roznia sie dla kazdej z fryzur. 

// Zaimplementuj powyzszy symulator, wykorzystując mechanizmy synchronizacji procesów oraz pamięć współdzieloną ze standardu:

// IPC - System V (60%)

#ifndef __HEADER_H__
#define __HEADER_H__

#define HAIRDRESSER_TOTAL 3 // hairdressers
#define CHAIR_TOTAL 2 // chairs
#define WAITROOM_TOTAL 5 // waiting room
#define HAIRCUT_TOTAL 4 // number of hairstyles
#define BUFFER_SIZE 1024

typedef struct
{
    int taken; //is occupied now
    int haircut; //chosen haircut
} Chair;

typedef struct {
    int free_seats;
    int next_seat;
    int next_client;
} WaitRoom;


#endif