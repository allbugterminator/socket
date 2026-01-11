# socket

## tutorial
包含socket教程以及Hello程序和echo服务器
- client.h：客户端测试代码
- echo_game.h：echo服务器实现
- hello.h：hello服务器程序
- tutorial_info.h：教程信息
- socket_tutorial.cpp：完整演示程序

### 编译命令

```cpp
# 在Linux/Mac上编译
g++ -std=c++11 -o socket_demo socket_tutorial.cpp -pthread

# 在Windows上编译（如果使用MinGW）
g++ -std=c++11 -o socket_demo.exe socket_tutorial.cpp -lws2_32
```

### 运行步骤

```cpp
# 终端1：运行服务器
./socket_demo
# 选择3或4运行服务器

# 终端2：运行客户端测试
telnet 127.0.0.1 8888
# 或者
./socket_demo
# 选择5运行客户端测试
```