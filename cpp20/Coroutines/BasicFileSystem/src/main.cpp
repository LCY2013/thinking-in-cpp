//
// Created by fufeng on 2024/2/22.
//
#include <filesystem> // C++17文件系统库
#include <iostream>   // 标准输出
#include <thread> // 线程库

namespace fs = std::filesystem;

void createDirectoriesAsync(const std::string& directoryPath) {
    std::cout << "Creating directory " << directoryPath << std::endl;
    fs::create_directories(directoryPath);
}

int main() {
    std::cout << "Start, task!" << std::endl;

    //创建三个线程对象
    std::thread makeDirTask1(createDirectoriesAsync, "dir1/a/b/c");
    std::thread makeDirTask2(createDirectoriesAsync, "dir1/1/2/3");
    std::thread makeDirTask3(createDirectoriesAsync, "dir1/A/B/C");

    //等待线程执行结束
    makeDirTask1.join();
    makeDirTask2.join();
    makeDirTask3.join();
    std::cout << "End, task!" << std::endl;
    return 0;
}

