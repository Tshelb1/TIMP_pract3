#include <iostream>
#include <string>
#include <cstdlib> //exit()
#include <cstring> // strpy()
#include <unistd.h> //close()
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;
void Exception(const string & why, const int exitCode ) // Исключения (возможные ошибки)
{
    cout << "ErrorCode:"<<exitCode <<endl<< why << endl;
    exit(exitCode);
}
int main()
{
    // структура с адресом нашей программы (клиента)
    sockaddr_in * selfAddr = new (sockaddr_in);
    selfAddr->sin_family = AF_INET; // интернет протокол IPv4
    selfAddr->sin_port = 0;         // любой порт на усмотрение ОС
    selfAddr->sin_addr.s_addr = 0;
    // структура с адресом "на той стороне" (сервера)
    sockaddr_in * remoteAddr = new (sockaddr_in);
    remoteAddr->sin_family = AF_INET;     // интернет протокол IPv4
    remoteAddr->sin_port = htons(13);  // port 13
    remoteAddr->sin_addr.s_addr = inet_addr("82.179.90.12"); //  адрес
    // буфер для передачи и приема данных
    char *buffer = new char[4096];
    strcpy(buffer,"Please, tell me the exact date and time");   //копируем строку
    int msgLen = strlen(buffer);      //вычисляем длину строки
    // создаём сокет
    int mySocket = socket(AF_INET, SOCK_DGRAM, 0); //udp протокол
    if (mySocket == -1) {
        close(mySocket);
        Exception("Error open socket",11);
    }
    //связываем сокет с адрессом
    int rc = bind(mySocket,(const sockaddr *) selfAddr, sizeof(sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        Exception("Error bind socket with local address",12);
    }
    //установливаем соединение
    rc = connect(mySocket, (const sockaddr*) remoteAddr, sizeof(sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        Exception("Error connect socket with remote server.", 13);
    }
    //передаём сообщение из буффера
    rc = send(mySocket, buffer, msgLen, 0);
    if (rc == -1) {
        close(mySocket);
        Exception("Error send message", 14);
    }
    cout << "We send: " << buffer << endl;
    // принимаем ответ в буффер
    rc = recv(mySocket, buffer, 4096, 0);
    if (rc == -1) {
        close(mySocket);
        Exception("Error receive answer.", 15);
    }
    buffer[rc] = '\0'; // конец принятой строки
    cout << "We receive: " << buffer; // вывод полученного сообщения от сервера
    // закрыем сокет
    close(mySocket);
    delete selfAddr;
    delete remoteAddr;
    delete[] buffer;
    return 0;
}