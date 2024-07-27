#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER "192.168.137.49"
#define PORT 3838
#define NICK "kaniberkali"
#define USER "ali 0 * :Ali"
#define CHANNEL "#test"

void send_message(int sockfd, const std::string &message) {
    send(sockfd, message.c_str(), message.length(), 0);
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;

    // Socket oluşturma
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket oluşturma hatası" << std::endl;
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // IP adresini çözümleme
    if (inet_pton(AF_INET, SERVER, &server_addr.sin_addr) <= 0) {
        std::cerr << "Geçersiz adres/Adres desteklenmiyor" << std::endl;
        return 1;
    }

    // Sunucuya bağlanma
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Bağlantı hatası" << std::endl;
        return 1;
    }

    // Kimlik doğrulama
    send_message(sockfd, "NICK " NICK "\r\n");
    send_message(sockfd, "USER " USER "\r\n");

    // Kanal oluşturma
    send_message(sockfd, "JOIN " CHANNEL "\r\n");

    // Sunucudan mesajları okuma
    char buffer[1024];
    while (true) {
        int bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            std::cerr << "Bağlantı kesildi" << std::endl;
            break;
        }
        buffer[bytes_received] = '\0';
        std::cout << buffer;

        // PING mesajlarını yanıtla
        if (strncmp(buffer, "PING", 4) == 0) {
            send_message(sockfd, "PONG " + std::string(buffer + 5) + "\r\n");
        }
    }

    close(sockfd);
    return 0;
}
