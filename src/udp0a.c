/* udp0.c */
/*         Kompilacja Linux: clang udp0a.c -o udp0a.exe -pthread */
/* Kompilacja MSYS2 CLANG64: clang udp0a.c -o udp0a.exe -lws2_32 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    // Nagłówki dla Windows (Winsock2)
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #define close closesocket // W Windowsie sockety zamyka się przez closesocket()
    typedef int socklen_t;
#else
    // Nagłówki dla Linux / POSIX
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
#endif

// Używamy standardowych wątków C11 (<threads.h>), które są wspierane 
// zarówno przez nowoczesny profil MSYS2 CLANG64, jak i Linuxa.
#include <threads.h> 

#define BUFFER_SIZE 1024

// Funkcja wykonywana przez każdy wątek (zmieniony typ zwracany dla thrd_create)
int receive_udp(void* arg) {
    int port = *(int*)arg;
    free(arg); 

    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // 1. Tworzenie socketu UDP
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        fprintf(stderr, "[Port %d] Blad podczas tworzenia socketu\n", port);
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    server_addr.sin_port = htons(port);

    // 2. Podpięcie socketu pod konkretny port
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        fprintf(stderr, "[Port %d] Blad podczas bindowania (bind)\n", port);
        close(sockfd);
        return -1;
    }

    printf("[Port %d] Watek uruchomiony, nasluchiwanie...\n", port);

    // 3. Pętla odbierająca pakiety
    while (1) {
        // W Windows recvfrom przyjmuje bufor jako char*, w Linuxie jako void* - Clang CLANG64 wymaga rzutowania na (char*)
        int n = recvfrom(sockfd, (char*)buffer, BUFFER_SIZE - 1, 0, 
                         (struct sockaddr *)&client_addr, &addr_len);
        
        if (n < 0) {
            fprintf(stderr, "[Port %d] Blad podczas odbierania danych\n", port);
            continue;
        }

        buffer[n] = '\0'; 

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);

        printf("[Port %d] Odebrano z %s:%d -> %s\n", 
               port, client_ip, ntohs(client_addr.sin_port), buffer);
    }

    close(sockfd);
    return 0;
}

int main() {
    printf(" (.Y.) UDP-test with pure C programming language\n");

#ifdef _WIN32
    // Windows wymaga zainicjalizowania biblioteki sieciowej przed jej użyciem
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup nie powiodl sie.\n");
        return 1;
    }
#endif

    int ports[] = {21001, 21002, 21003};
    int num_ports = sizeof(ports) / sizeof(ports[0]);
    thrd_t threads[num_ports]; // Zmiana z pthread_t na standardowy thrd_t

    for (int i = 0; i < num_ports; i++) {
        int* port_arg = malloc(sizeof(int));
        *port_arg = ports[i];

        // Zmiana z pthread_create na thrd_create
        if (thrd_create(&threads[i], receive_udp, port_arg) != thrd_success) {
            fprintf(stderr, "Blad podczas tworzenia watku dla portu %d\n", ports[i]);
            free(port_arg);
            return 1;
        }
    }

    for (int i = 0; i < num_ports; i++) {
        thrd_join(threads[i], NULL); // Zmiana z pthread_join na thrd_join
    }

#ifdef _WIN32
    WSACleanup(); // Posprzątanie po Winsock na Windowsie
#endif

    return 0;
}
