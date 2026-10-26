// Kompilacja
// g++ -std=c++20 -O3 udp1.cpp -o udp1.exe

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>   // Wymagane do std::thread
#include <chrono>   // Do usypiania głównego wątku (std::this_thread::sleep_for)
#include <cstring>

class UdpServer {
private:
    int sockfd = -1;

public:
    UdpServer() = default;

    // Destruktor automatycznie zamknie socket, gdy obiekt będzie niszczony
    ~UdpServer() {
        if (sockfd >= 0) {
            std::cout << "[Serwer] Zamykanie gniazda w destruktorze..." << std::endl;
            close(sockfd);
        }
    }

    // Blokujemy kopiowanie obiektu dla bezpieczeństwa sieciowego
    UdpServer(const UdpServer&) = delete;
    UdpServer& operator=(const UdpServer&) = delete;

    bool uruchom(int port) {
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) return false;

        struct sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);

        if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            close(sockfd);
            sockfd = -1;
            return false;
        }
        return true;
    }

    // Metoda, którą wywołamy w osobnym wątku
    void petlaOdbiorcza() {
        std::cout << "[Wątek] Rozpoczęto nasłuchiwanie..." << std::endl;
        char buffer[1024];
        
        while (sockfd >= 0) { // Pętla działa, dopóki gniazdo jest otwarte
            struct sockaddr_in client_addr{};
            socklen_t addr_len = sizeof(client_addr);

            // Blokujące wywołanie recvfrom
            ssize_t bytes = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, 
                                     (struct sockaddr*)&client_addr, &addr_len);

            if (bytes < 0) {
                // Jeśli socket zostanie zamknięty z głównego wątku, recvfrom zostanie przerwane tym błędem
                std::cerr << "[Wątek] Błąd recvfrom (Kod " << errno << "): " << std::strerror(errno) << std::endl;
                break; 
            }

            buffer[bytes] = '\0';
            std::cout << "[Wątek] Odebrano: " << buffer << std::endl;
        }
        std::cout << "[Wątek] Zakończono pracę." << std::endl;
    }
};

int main() {
    UdpServer serwer;

    // 1. Inicjalizacja serwera na porcie 8080
    if (!serwer.uruchom(8080)) {
        std::cerr << "[Main] Nie udało się uruchomić serwera!" << std::endl;
        return 1;
    }
    std::cout << "[Main] Serwer uruchomiony poprawnie." << std::endl;

    // 2. Uruchomienie metody klasy w osobnym wątku
    // Składnia: std::thread( &NazwaKlasy::Metoda, std::ref(instancja_obiektu) )
    std::thread watek_sieciowy(&UdpServer::petlaOdbiorcza, std::ref(serwer));

    // Główny wątek (main) może teraz robić coś innego, np. symulujemy pracę przez 10 sekund
    std::cout << "[Main] Główny wątek wykonuje inne zadania..." << std::endl;
    for (int i = 1; i <= 5; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "[Main] Czas mija: " << i * 2 << "s..." << std::endl;
    }

    // 3. Prawidłowe zakończenie programu
    std::cout << "[Main] Koniec pracy programu. Czekam na zakończenie wątku..." << std::endl;
    
    // Gdy funkcja main() dobiega końca, obiekt 'serwer' zostanie zniszczony, 
    // wywoła się jego destruktor, który zrobi close(sockfd). 
    // Wtedy recvfrom() w wątku natychmiast się odblokuje i wątek się zakończy.

    watek_sieciowy.join(); // Czekamy, aż wątek sieciowy oficjalnie skończy pracę
    std::cout << "[Main] Wątek zamknięty. Wyjście." << std::endl;

    return 0;
}
