#include <stdio.h>
#include <stdlib.h>

#include "coroutine.h"

const char* messages[] = { 
    "1. create\n",
    "2. yield\n",
    "3. resume\n",
    "4. finish\n",
    "5. end\n",
    "6. delete\n",
};


void __stdcall print(size_t index) {
    printf(messages[index - 1]);
}

void test_func(void* arg) {
    print(2);
  
    coro_yield(coro_current());
     
    print(4);
}
  
void test() {
    printf("test\n");

    print(1);
    struct coro_t* coro = new_coro(test_func, NULL);

    coro_resume(coro);

    print(3);

    coro_resume(coro);

    print(5);

    coro_free(coro);

    print(6);
}


void test_asm_func(void* arg) {
    __asm {
     
        push 2
        call print

        call coro_current

        push eax 
        call coro_yield
        add esp, 4 

        push 4
        call print

    }
}

void test_asm() {
    printf("test asm\n");

    __asm {
        push 1
        call print

        push esi 

        push 0
        push test_asm_func
        call new_coro
        add esp, 0x8
        mov esi, eax 

        push esi
        call coro_resume 
        add esp, 4 

        push 3
        call print

        push esi
        call coro_resume
        add esp, 4 

        push 5
        call print


        push esi 
        call coro_free
        add esp, 4

        push 6
        call print

        pop esi 
    }
}
int main() {
    test();

    printf("\n\n");

    test_asm();

    return 0;
}

