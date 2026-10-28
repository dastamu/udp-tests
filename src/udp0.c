/* udp0.c */
/* gcc src/udp0.c -o build-gcc/udp0.exe -pthread */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUFFER_SIZE 1024

// Funkcja wykonywana przez każdy wątek
void* receive_udp(void* arg) {
    int port = *(int*)arg;
    free(arg); // Zwolnienie pamięci przydzielonej w main() dla numeru portu

    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // 1. Tworzenie socketu UDP (SOCK_DGRAM)
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        fprintf(stderr, "[Port %d] Błąd podczas tworzenia socketu\n", port);
        pthread_exit(NULL);
    }

    // Konfiguracja struktury adresowej
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // Nasłuchuj na wszystkich interfejsach
    server_addr.sin_port = htons(port);

    // 2. Podpięcie socketu pod konkretny port (Bind)
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        fprintf(stderr, "[Port %d] Błąd podczas bindowania (bind)\n", port);
        close(sockfd);
        pthread_exit(NULL);
    }

    printf("[Port %d] Wątek uruchomiony, nasłuchiwanie...\n", port);

    // 3. Pętla odbierająca pakiety
    while (1) {
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0, 
                         (struct sockaddr *)&client_addr, &addr_len);
        
        if (n < 0) {
            fprintf(stderr, "[Port %d] Błąd podczas odbierania danych\n", port);
            continue;
        }

        buffer[n] = '\0'; // Dodanie znaku końca stringu

        // Konwersja IP klienta na czytelny format tekstowy
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);

        // Wyświetlenie odebranej wiadomości
        printf("[Port %d] Odebrano z %s:%d -> %s\n", 
               port, client_ip, ntohs(client_addr.sin_port), buffer);
    }

    close(sockfd);
    return NULL;
}


int main() {
  printf(" (.Y.) UDP-test with pure C programming language\n");

    int ports[] = {21001, 21002, 21003};
    int num_ports = sizeof(ports) / sizeof(ports[0]);
    pthread_t threads[num_ports];

    // Tworzenie osobnego wątku dla każdego portu
    for (int i = 0; i < num_ports; i++) {
        // Alokujemy pamięć na port, aby uniknąć problemów z wyścigiem danych (race condition)
        int* port_arg = malloc(sizeof(int));
        *port_arg = ports[i];

        if (pthread_create(&threads[i], NULL, receive_udp, port_arg) != 0) {
            fprintf(stderr, "Błąd podczas tworzenia wątku dla portu %d\n", ports[i]);
            free(port_arg);
            return 1;
        }
    }

    // Oczekiwanie na zakończenie wątków (w tym programie działają w nieskończoność)
    for (int i = 0; i < num_ports; i++) {
        pthread_join(threads[i], NULL);
    }

  return 0;
}
