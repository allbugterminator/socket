#ifndef __SOCKET_TUTORIAL_ECHO_H__
#define __SOCKET_TUTORIAL_ECHO_H__

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

class EchoGameServer {
private:
    int server_phone;
    
public:
    EchoGameServer() : server_phone(-1) {}

    ~EchoGameServer() {
        std::cout << "\n============================" << std::endl;

        sleep(1);
        stop_game();
    }
    
    bool start_game() {
        std::cout << "\n🔁 开始建造回声游戏服务器..." << std::endl;
        
        // 1. 买电话
        server_phone = socket(AF_INET, SOCK_STREAM, 0);
        if (server_phone < 0) {
            std::cerr << "❌ 买电话失败！" << std::endl;
            return false;
        }
        
        // 2. 允许地址重用
        int yes = 1;
        setsockopt(server_phone, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
        
        // 3. 制作名片
        struct sockaddr_in my_address;
        memset(&my_address, 0, sizeof(my_address));
        my_address.sin_family = AF_INET;
        my_address.sin_addr.s_addr = INADDR_ANY;
        my_address.sin_port = htons(8888);
        
        // 4. 申请号码
        if (bind(server_phone, (struct sockaddr*)&my_address, sizeof(my_address)) < 0) {
            std::cerr << "❌ 申请号码失败！" << std::endl;
            close(server_phone);
            return false;
        }
        
        // 5. 打开铃声
        if (listen(server_phone, 5) < 0) {
            std::cerr << "❌ 打开铃声失败！" << std::endl;
            close(server_phone);
            return false;
        }
        
        std::cout << "✅ 回声服务器已启动！" << std::endl;
        std::cout << "   地址: 0.0.0.0:8888" << std::endl;
        std::cout << "   等待玩家加入..." << std::endl;
        
        return true;
    }
    
    void play_game() {
        try
        {
            while (true) {
                // 6. 等待玩家（accept）
                struct sockaddr_in player_address;
                socklen_t player_len = sizeof(player_address);
                int player_phone = accept(server_phone, (struct sockaddr*)&player_address, &player_len);
                if (player_phone < 0) {
                    std::cerr << "❌ 接电话失败！" << std::endl;
                    continue;
                }
                
                // 显示谁加入了
                char player_ip[20];
                inet_ntop(AF_INET, &player_address.sin_addr, player_ip, sizeof(player_ip));
                std::cout << "\n🎮 新玩家加入: " << player_ip << ":"
                        << ntohs(player_address.sin_port) << std::endl;
                
                // 开始回声游戏
                play_echo_with_player(player_phone);
                
                close(player_phone);
                std::cout << "👋 玩家离开游戏" << std::endl;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << "failed to play game, err: " << e.what() << std::endl;
            stop_game();
        }
        catch(...) 
        {
            std::cerr << "failed to play game, err: unknown error" << std::endl;
            stop_game();
        }
    }
    
private:
    void play_echo_with_player(int player_phone) {
        char buffer[1024];  // 对话缓冲区
        std::cout << "💬 开始回声游戏 (输入'bye'退出)" << std::endl;
        
        while (true) {
            // 清空缓冲区
            memset(buffer, 0, sizeof(buffer));
            
            // 7. 倾听玩家说话（recv）
            int bytes_received = recv(player_phone, buffer, sizeof(buffer) - 1, 0);
            
            if (bytes_received <= 0) {
                if (bytes_received == 0) {
                    std::cout << "   玩家主动离开" << std::endl;
                } else {
                    std::cerr << "   倾听失败" << std::endl;
                }
                break;
            }
            
            // 显示玩家说了什么
            std::cout << "   👤 玩家说: " << buffer << std::endl;
            
            // 检查是否要退出
            if (strstr(buffer, "bye") != nullptr || strstr(buffer, "quit") != nullptr) {
                std::cout << "   收到退出指令" << std::endl;
                break;
            }
            
            // 8. 重复玩家的话（send - Echo的核心！）
            int bytes_sent = send(player_phone, buffer, bytes_received, 0);
            if (bytes_sent < 0) {
                std::cerr << "   重复失败" << std::endl;
                break;
            }
            
            std::cout << "   🔁 回声: " << buffer << std::endl;
        }
    }
    
    void stop_game() {
        std::cout << "\nstart to stop_game, server_phone: " << server_phone << std::endl;
        if (server_phone >= 0) {
            close(server_phone);
            server_phone = -1;
            std::cout << "\n🛑 游戏服务器已关闭" << std::endl;
        }

        std::cout << "\nend to stop_game, server_phone: " << server_phone << std::endl;
    }

};

#endif