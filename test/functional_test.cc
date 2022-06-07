#include <iostream>
#include "../base/SkipList.h"

#define FILE_PATH "./store/dumpFile"

int main(int argv, char* argc[]) {

    // 键值中的key用int型，如果用其他类型，需要自定义比较函数
    // 而且如果修改key的类型，同时需要修改skipList.load_file函数
    SkipList<int, std::string> mySkipList(6);
    mySkipList.insertElement(1, "南理工");
    mySkipList.insertElement(3, "北理工");
    mySkipList.insertElement(5, "南航");
    mySkipList.insertElement(7, "哈工大");
    mySkipList.insertElement(9, "北航");

    // 重复键值
    mySkipList.insertElement(5, "小清华");
    mySkipList.insertElement(7, "深职院");
    mySkipList.insertElement(9, "广轻工");

    std::cout << "SkipList's size : " << mySkipList.size() << std::endl;

    mySkipList.dumpFile();

    std::string value;
    if(mySkipList.searchElement(1, value)) {
        std::cout << "find value of key 1:" << value << std::endl;
    }
    else {
        std::cout << "skipList can not find the key" << std::endl;
    }

    if(mySkipList.searchElement(4, value)) {
        std::cout << "find value of key 1:" << value << std::endl;
    }
    else {
        std::cout << "skipList can not find the key" << std::endl;
    }

    mySkipList.displayList();

    mySkipList.deleteElement(1);
    mySkipList.deleteElement(4);

    std::cout << "SkipList's size : " << mySkipList.size() << std::endl;

    mySkipList.displayList();
}