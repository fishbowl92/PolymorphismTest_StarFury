#pragma once

#include <cstdlib>   // malloc, free
#include <iostream>  // std::cerr
#include <fstream>   // �α� ���� �ۼ�
#include <map>       // �޸� �Ҵ� ���� ����
#include <cstring>   // strcpy_s
#include <ctime>     // �ð� ���� ����

// �޸� ���� ������ �����ϴ� ����ü
typedef struct AllocInfo 
{
    void* ptr;              // �Ҵ�� �޸� �ּ�
    size_t size;            // �Ҵ�� �޸� ũ��
    char filename[128];     // �޸𸮸� �Ҵ��� �ҽ� ���� �̸�
    int line;               // �޸𸮸� �Ҵ��� �ҽ� ������ ���� ��ȣ
    bool array;             // �迭���� ����(new[] ����)
} AllocInfo;

// ���� �޸� ���� Ŭ���� ����
class MemoryManager 
{
public:
    // �޸� ���� ������ �����ϴ� �����̳�
    std::map<void*, AllocInfo> allocMap;

    // �޸� �Ҵ� ���� ���
    void Register(void* ptr, size_t size, const char* filename, int line, bool array);

    // �޸� ���� ���� Ȯ�� �� ����
    void Unregister(void* ptr, bool array);

    // �Ҹ��ڿ��� �޸� ���� �α� �ۼ�
    ~MemoryManager();
};

// ���� �޸� ���� ��ü ����
extern MemoryManager gMemoryManager;

// new ������ �����ε� (���� �̸��� ���� ��ȣ�� ����)
void* operator new(size_t size, const char* filename, int line);
void* operator new[](size_t size, const char* filename, int line);

// delete ������ �����ε� (�迭 ���θ� ����)
void operator delete(void* ptr) noexcept;
void operator delete[](void* ptr) noexcept;

// ��ũ�� ���Ƿ� new�� ������ ���
#ifdef ENABLE_MEMORY_TRACKING
#define new new(__FILE__, __LINE__)
#endif