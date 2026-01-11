#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "tutorial_info.h"
#include "echo_game.h"
#include "hello.h"
#include "client.h"

int main() {
    try
    {
        std::cout << "🎪 ====================================" << std::endl;
        std::cout << "    Socket编程奇遇记 - 完整演示" << std::endl;
        std::cout << "======================================" << std::endl;
        
        int choice = 0;
        
        while (true) {
            std::cout << "\n🔧 请选择要运行的功能：" << std::endl;
            std::cout << "1. 学习7个Socket函数（电话咒语）" << std::endl;
            std::cout << "2. 理解sockaddr_in（网络名片）" << std::endl;
            std::cout << "3. 运行Hello服务器（一次问候）" << std::endl;
            std::cout << "4. 运行Echo服务器（回声游戏）" << std::endl;
            std::cout << "5. 运行客户端测试" << std::endl;
            std::cout << "6. 查看Telnet测试指南" << std::endl;
            std::cout << "0. 退出" << std::endl;
            std::cout << "请输入选择: ";

            std::cin >> choice;
            std::cin.ignore();  // 清空输入缓冲区
            
            switch (choice) {
                case 1: {
                    TelephoneWizard wizard;
                    wizard.cast_spells();
                    break;
                }
                    
                case 2: {
                    NetworkBusinessCard card;
                    card.demonstrate();
                    break;
                }
                    
                case 3: {
                    std::cout << "\n🎁 运行Hello服务器..." << std::endl;
                    GreetingServer hello_server;
                    hello_server.say_hello();
                    break;
                }
                    
                case 4: {
                    std::cout << "\n🔁 运行Echo服务器..." << std::endl;
                    EchoGameServer echo_server;
                    
                    if (echo_server.start_game()) {
                        // 在新线程中运行服务器
                        std::thread server_thread([&echo_server]() {
                            echo_server.play_game();
                        });
                        
                        std::cout << "\n🎮 服务器已启动！现在可以：" << std::endl;
                        std::cout << "1. 打开新终端，运行: telnet 127.0.0.1 8888" << std::endl;
                        std::cout << "2. 或者选择选项5运行客户端测试" << std::endl;
                        std::cout << "\n按Enter键停止服务器..." << std::endl;
                        
                        std::cin.get();  // 等待用户按Enter
                        // 因为server_thread线程也持有echo_server对象，但是出了当前作用域后echo_server就会被释放，导致core dumped。
                    }
                    break;
                }
                    
                case 5: {
                    std::cout << "\n🧪 运行客户端测试..." << std::endl;
                    EchoTester tester;
                    tester.test_server();
                    break;
                }
                    
                case 6: {
                    std::cout << "\n🖥️ Telnet测试完全指南：" << std::endl;
                    std::cout << R"(
    === Telnet测试步骤 ===

    1. 启动服务器：
    在终端1运行：./socket_demo
    选择3或4启动服务器

    2. 测试Hello服务器（选项3）：
    在终端2运行：telnet 127.0.0.1 8888
    你应该看到：Hello! 欢迎来到我的第一个Socket服务器！
    然后连接自动关闭

    3. 测试Echo服务器（选项4）：
    在终端2运行：telnet 127.0.0.1 8888
    输入任何文字，服务器都会原样返回
    输入'bye'或'quit'退出

    4. 常见问题解决：
    • 连接被拒绝：确保服务器正在运行
    • 无法连接：检查防火墙设置
    • 无响应：确保端口号正确（8888）

    5. Windows用户注意：
    可能需要先启用Telnet客户端：
    控制面板 → 程序 → 启用或关闭Windows功能 → 勾选"Telnet客户端"
                    )" << std::endl;
                    break;
                }
                    
                case 0:
                    std::cout << "\n👋 感谢学习Socket编程！祝你成为网络编程高手！" << std::endl;
                    return 0;
                    
                default:
                    std::cout << "❌ 无效选择，请重试" << std::endl;
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "failed to test socket, err: " << e.what() << '\n';
        return 1;
    }
    catch(...)
    {
        std::cerr << "failed to test socket, err: unknown error" << '\n';
        return 1;
    }
    
    return 0;
}