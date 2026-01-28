#include <iostream>
#include <string>
#include <thread>
#include "tutorial_info.h"
#include "echo_game.h"
#include "udp_experiments.h"
#include "client.h"

int main() {
    std::cout << "🎪 ====================================" << std::endl;
    std::cout << "   UDP编程奇遇记 - 完整演示" << std::endl;
    std::cout << "======================================" << std::endl;
    
    int choice = 0;
    
    while (true) {
        std::cout << "\n🔧 请选择要运行的功能：" << std::endl;
        std::cout << "1. 学习UDP Socket创建" << std::endl;
        std::cout << "2. 学习sendto()和recvfrom()" << std::endl;
        std::cout << "3. 运行UDP Echo服务器" << std::endl;
        std::cout << "4. 运行UDP客户端测试" << std::endl;
        std::cout << "5. 运行UDP特性实验" << std::endl;
        std::cout << "6. 查看UDP vs TCP对比" << std::endl;
        std::cout << "0. 退出" << std::endl;
        std::cout << "请输入选择: ";
        
        std::cin >> choice;
        std::cin.ignore();  // 清空输入缓冲区
        
        switch (choice) {
            case 1: {
                UDPSocketDemo demo;
                demo.create_udp_socket();
                break;
            }
                
            case 2: {
                UDPFunctions funcs;
                funcs.demonstrate_sendto();
                funcs.demonstrate_recvfrom();
                break;
            }
                
            case 3: {
                std::cout << "\n🏣 运行UDP Echo服务器..." << std::endl;
                UDPEchoServer server;
                
                if (server.start()) {
                    // 在新线程中运行服务器
                    std::thread server_thread([&server]() {
                        server.run();
                    });
                    
                    std::cout << "\n🎮 服务器已启动！现在可以：" << std::endl;
                    std::cout << "1. 选择选项4运行客户端测试" << std::endl;
                    std::cout << "2. 或用nc命令测试: nc -u 127.0.0.1 8888" << std::endl;
                    std::cout << "\n按Enter键停止服务器..." << std::endl;
                    
                    std::cin.get();  // 等待用户按Enter
                    
                    server.stop();
                    if (server_thread.joinable()) {
                        server_thread.join();
                    }
                }
                break;
            }
                
            case 4: {
                std::cout << "\n📱 运行UDP客户端测试..." << std::endl;
                UDPClientDemo client;
                client.demonstrate_tcp_vs_udp();
                client.create_udp_client();
                break;
            }
                
            case 5: {
                std::cout << "\n🔬 运行UDP特性实验..." << std::endl;
                UDPExperiments experiments;
                experiments.experiment_connectionless();
                experiments.experiment_message_boundary();
                break;
            }
                
            case 6: {
                std::cout << "\n📊 UDP vs TCP 完整对比" << std::endl;
                std::cout << R"(
┌─────────────────┬─────────────────────┬─────────────────────┐
│ 特性对比 │         TCP         │         UDP         │
├─────────────────┼─────────────────────┼─────────────────────┤
│ 连接方式 │ 面向连接(三次握手)  │ 无连接 │
│ 可靠性 │ 可靠传输 │ 不可靠传输 │
│ 数据顺序 │ 保证按序到达 │ 不保证顺序 │
│ 流量控制 │ 有(滑动窗口)        │ 无 │
│ 拥塞控制 │ 有 │ 无 │
│ 头部大小 │ 20-60字节 │ 8字节 │
│ 消息边界 │ 字节流，无边界 │ 数据报，有边界 │
│ 适用场景 │ 网页、文件、邮件 │ 视频、游戏、DNS     │
│ Socket类型 │ SOCK_STREAM         │ SOCK_DGRAM          │
│ 核心函数 │ connect/send/recv   │ sendto/recvfrom     │
└─────────────────┴─────────────────────┴─────────────────────┘
                )" << std::endl;
                break;
            }
                
            case 0:
                std::cout << "\n👋 感谢学习UDP编程！闪电侠期待与你再次相见！" << std::endl;
                return 0;
                
            default:
                std::cout << "❌ 无效选择，请重试" << std::endl;
        }
    }
    
    return 0;
}