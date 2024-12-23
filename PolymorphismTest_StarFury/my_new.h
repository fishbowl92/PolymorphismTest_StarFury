#pragma once

#include <cstdlib>   // malloc, free
#include <iostream>  // std::cerr
#include <fstream>   // 로그 파일 작성
#include <map>       // 메모리 할당 정보 관리
#include <cstring>   // strcpy_s
#include <ctime>     // 시간 정보 관리

// 메모리 추적 정보를 저장하는 구조체
typedef struct AllocInfo 
{
    void* ptr;              // 할당된 메모리 주소
    size_t size;            // 할당된 메모리 크기
    char filename[128];     // 메모리를 할당한 소스 파일 이름
    int line;               // 메모리를 할당한 소스 파일의 라인 번호
    bool array;             // 배열인지 여부(new[] 여부)
} AllocInfo;

// 전역 메모리 관리 클래스 선언
class MemoryManager 
{
public:
    // 메모리 추적 정보를 저장하는 컨테이너
    std::map<void*, AllocInfo> allocMap;

    // 메모리 할당 정보 등록
    void Register(void* ptr, size_t size, const char* filename, int line, bool array);

    // 메모리 해제 정보 확인 및 삭제
    void Unregister(void* ptr, bool array);

    // 소멸자에서 메모리 누수 로그 작성
    ~MemoryManager();
};

// 전역 메모리 관리 객체 선언
extern MemoryManager gMemoryManager;

// new 연산자 오버로딩 (파일 이름과 라인 번호를 포함)
void* operator new(size_t size, const char* filename, int line);
void* operator new[](size_t size, const char* filename, int line);

// delete 연산자 오버로딩 (배열 여부를 구분)
void operator delete(void* ptr) noexcept;
void operator delete[](void* ptr) noexcept;

// 매크로 정의로 new를 간편히 사용
#ifdef ENABLE_MEMORY_TRACKING
#define new new(__FILE__, __LINE__)
#endif