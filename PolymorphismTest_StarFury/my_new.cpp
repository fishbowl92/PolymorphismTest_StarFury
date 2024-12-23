#include "my_new.h"

// 전역 메모리 관리 객체 정의
MemoryManager gMemoryManager;

// MemoryManager의 메모리 할당 정보 등록 함수
void MemoryManager::Register(void* ptr, size_t size, const char* filename, int line, bool array) 
{
    AllocInfo info = { ptr, size, {}, line, array };
    strncpy_s(info.filename, filename, sizeof(info.filename) - 1);
    allocMap[ptr] = info;
}

// MemoryManager의 메모리 해제 정보 확인 및 삭제 함수
void MemoryManager::Unregister(void* ptr, bool array) 
{
    auto it = allocMap.find(ptr);
    if (it != allocMap.end()) 
    {
        if (it->second.array != array) 
        {
            std::cerr << "ARRAY mismatch detected at " << it->second.filename << ":" << it->second.line << std::endl;
        }
        allocMap.erase(it);
    }
    else 
    {
        std::cerr << "NOALLOC detected for pointer " << ptr << std::endl;
    }
}

// MemoryManager 소멸자에서 메모리 누수 로그 작성
MemoryManager::~MemoryManager() 
{
    std::ofstream logFile;
    char logName[256];
    time_t now = time(nullptr);
    struct tm t;
    localtime_s(&t, &now);
    sprintf_s(logName, "Alloc_%04d%02d%02d_%02d%02d%02d.txt", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
    logFile.open(logName);

    for (const auto& entry : allocMap) 
    {
        const AllocInfo& info = entry.second;
        logFile << "LEAK    [" << entry.first << "] [" << info.size << "] " << info.filename << " : " << info.line << std::endl;
    }
    logFile.close();
}

// new 연산자 오버로딩
void* operator new(size_t size, const char* filename, int line) 
{
    void* ptr = malloc(size);
    if (!ptr) throw std::bad_alloc();
    gMemoryManager.Register(ptr, size, filename, line, false);
    return ptr;
}

void* operator new[](size_t size, const char* filename, int line) 
{
    void* ptr = malloc(size);
    if (!ptr) throw std::bad_alloc();
    gMemoryManager.Register(ptr, size, filename, line, true);
    return ptr;
}

// delete 연산자 오버로딩
void operator delete(void* ptr) noexcept 
{
    gMemoryManager.Unregister(ptr, false);
    free(ptr);
}

void operator delete[](void* ptr) noexcept 
{
    gMemoryManager.Unregister(ptr, true);
    free(ptr);
}
