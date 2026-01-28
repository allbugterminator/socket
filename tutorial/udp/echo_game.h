#ifndef __UDP_SOCKET_TUTORIAL_ECHO_H__
#define __UDP_SOCKET_TUTORIAL_ECHO_H__

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class UDPEchoServer {
private:
    int server_sock;
    bool running;
    
public:
    UDPEchoServer() : server_sock(-1), running(false) {}
    
    bool start(int port = 8888) {
        std::cout << "\n🏣 启动UDP Echo邮局..." << std::endl;
        
        // 1. 创建UDP Socket（买一部电报机）
        server_sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (server_sock < 0) {
            std::cerr << "❌ 购买电报机失败！" << std::endl;
            return false;
        }
        std::cout << "✅ 购买电报机成功！编号: " << server_sock << std::endl;
        
        // 2. 设置邮局地址
        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;  // 接收所有来源
        server_addr.sin_port = htons(port);
        
        // 3. 绑定地址（告诉邮局在哪里）
        if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            std::cerr << "❌ 设置邮局地址失败！" << std::endl;
            close(server_sock);
            return false;
        }
        
        std::cout << "✅ 邮局地址设置完成！" << std::endl;
        std::cout << "   地址: 0.0.0.0:" << port << std::endl;
        std::cout << "   状态: 等待电报..." << std::endl;
        
        running = true;
        return true;
    }
    
    void run() {
        char buffer[1024];
        
        while (running) {
            // 准备接收电报
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            
            std::cout << "\n📭 等待电报..." << std::endl;
            
            // 4. 接收电报（recvfrom）
            memset(buffer, 0, sizeof(buffer));
            ssize_t recv_len = recvfrom(
                server_sock, buffer, sizeof(buffer) - 1, 0,
                (struct sockaddr*)&client_addr, &client_len
            );
            
            if (recv_len < 0) {
                std::cerr << "❌ 接收电报失败！" << std::endl;
                continue;
            }
            
            // 获取发件人信息
            char client_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
            int client_port = ntohs(client_addr.sin_port);
            
            std::cout << "📨 收到电报！" << std::endl;
            std::cout << "   发件人: " << client_ip << ":" << client_port << std::endl;
            std::cout << "   内容: \"" << buffer << "\"" << std::endl;
            std::cout << "   长度: " << recv_len << " 字节" << std::endl;
            
            // 检查退出命令
            if (strstr(buffer, "bye") != nullptr || strstr(buffer, "quit") != nullptr) {
                std::cout << "   🏁 收到退出指令，停止服务" << std::endl;
                const char* goodbye = "Goodbye!";
                sendto(server_sock, goodbye, strlen(goodbye), 0,
                      (struct sockaddr*)&client_addr, client_len);
                break;
            }
            
            // 5. 发回电报（sendto - Echo的核心！）
            std::cout << "📤 正在回发电报..." << std::endl;
            
            ssize_t sent_len = sendto(
                server_sock, buffer, recv_len, 0,
                (struct sockaddr*)&client_addr, client_len
            );
            
            if (sent_len < 0) {
                std::cerr << "❌ 回发电报失败！" << std::endl;
            } else {
                std::cout << "✅ 回发电报成功！" << std::endl;
                std::cout << "   回声: \"" << buffer << "\"" << std::endl;
            }
        }
    }
    
    void stop() {
        running = false;
        if (server_sock >= 0) {
            close(server_sock);
            std::cout << "\n🛑 UDP Echo邮局已关闭" << std::endl;
        }
    }
    
    ~UDPEchoServer() {
        stop();
    }
};

#endif