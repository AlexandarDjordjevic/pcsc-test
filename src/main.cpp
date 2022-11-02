#include <iostream>
#include <cstdlib>
#include <PCSC/wintypes.h>
#include <PCSC/winscard.h>

void printState(uint32_t state){
    std::cout << "Current state: ";
    if(state == SCARD_STATE_UNAWARE) {
        std::cout << "SCARD_STATE_UNAWARE ";
    }
    if(state & SCARD_STATE_IGNORE) {
        std::cout << "SCARD_STATE_IGNORE ";
    }
    if(state & SCARD_STATE_CHANGED) {
        std::cout << "SCARD_STATE_CHANGED ";
    }
    if(state & SCARD_STATE_UNKNOWN) {
        std::cout << "SCARD_STATE_UNKNOWN ";
    }
    if(state & SCARD_STATE_UNAVAILABLE) {
        std::cout << "SCARD_STATE_UNAVAILABLE ";
    }
    if(state & SCARD_STATE_EMPTY) {
        std::cout << "SCARD_STATE_EMPTY ";
    }
    if(state & SCARD_STATE_PRESENT) {
        std::cout << "SCARD_STATE_PRESENT ";
    }
    if(state & SCARD_STATE_ATRMATCH) {
        std::cout << "SCARD_STATE_ATRMATCH ";
    }
    if(state & SCARD_STATE_EXCLUSIVE) {
        std::cout << "SCARD_STATE_EXCLUSIVE ";
    }
    if(state & SCARD_STATE_INUSE) {
        std::cout << "SCARD_STATE_INUSE ";
    }
    if(state & SCARD_STATE_MUTE) {
        std::cout << "SCARD_STATE_MUTE ";
    }
    if(state & SCARD_STATE_UNPOWERED) {
        std::cout << "SCARD_STATE_UNPOWERED ";
    }
    std::cout << std::endl;
}

int main(){
    SCARDCONTEXT context;
    auto result = SCardEstablishContext(SCARD_SCOPE_SYSTEM, nullptr, nullptr, &context);
    if (result != SCARD_S_SUCCESS){
        std::cout << "SCardEstablishContext failed with error " << pcsc_stringify_error(result) << std::endl;
        return EXIT_FAILURE;
    }

    while(true){
        SCARD_READERSTATE readerState;
        readerState.szReader = "\\\\?PnP?\\Notification";
        result = SCardGetStatusChange (context, INFINITE, &readerState, 1);
        if (result != SCARD_S_SUCCESS){
            std::cout << "SCardEstablishContext failed with error " << pcsc_stringify_error(result) << std::endl;
            return EXIT_FAILURE;
        }
        printState(readerState.dwCurrentState);
        readerState.dwEventState = readerState.dwCurrentState;
    }



    return EXIT_SUCCESS;
}
