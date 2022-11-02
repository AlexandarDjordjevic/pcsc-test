#include <cstdint>
#include <ios>
#include <iostream>
#include <cstdlib>
#include <PCSC/wintypes.h>
#include <PCSC/winscard.h>
#include <thread>
#include <chrono>
#include <cstring>
#include <vector>

void printState(uint32_t state)
{
    state &= 0x0000FFFF;
    std::cout << "State: " << std::hex << state << " ";
    if (state == SCARD_STATE_UNAWARE) {
        std::cout << "SCARD_STATE_UNAWARE ";
    }
    if (state & SCARD_STATE_IGNORE) {
        std::cout << "SCARD_STATE_IGNORE ";
    }
    if (state & SCARD_STATE_CHANGED) {
        std::cout << "SCARD_STATE_CHANGED ";
    }
    if (state & SCARD_STATE_UNKNOWN) {
        std::cout << "SCARD_STATE_UNKNOWN ";
    }
    if (state & SCARD_STATE_UNAVAILABLE) {
        std::cout << "SCARD_STATE_UNAVAILABLE ";
    }
    if (state & SCARD_STATE_EMPTY) {
        std::cout << "SCARD_STATE_EMPTY ";
    }
    if (state & SCARD_STATE_PRESENT) {
        std::cout << "SCARD_STATE_PRESENT ";
    }
    if (state & SCARD_STATE_ATRMATCH) {
        std::cout << "SCARD_STATE_ATRMATCH ";
    }
    if (state & SCARD_STATE_EXCLUSIVE) {
        std::cout << "SCARD_STATE_EXCLUSIVE ";
    }
    if (state & SCARD_STATE_INUSE) {
        std::cout << "SCARD_STATE_INUSE ";
    }
    if (state & SCARD_STATE_MUTE) {
        std::cout << "SCARD_STATE_MUTE ";
    }
    if (state & SCARD_STATE_UNPOWERED) {
        std::cout << "SCARD_STATE_UNPOWERED ";
    }
    std::cout << std::endl;
}

int main()
{
    SCARDCONTEXT context;
    auto result = SCardEstablishContext(SCARD_SCOPE_SYSTEM, nullptr, nullptr, &context);
    if (result != SCARD_S_SUCCESS) {
        std::cout << "SCardEstablishContext failed with error " << pcsc_stringify_error(result)
                  << std::endl;
        return EXIT_FAILURE;
    }

    char rawReadersNames[4096] {0};
    uint64_t readersNameLen = 0;

    for (auto i = 0; i < 4; i++) {
        result = SCardListReaders(context, NULL, rawReadersNames, &readersNameLen);
    }

    std::vector<std::string> readerNames;
    auto parsingPosition = 0;
    while (parsingPosition < readersNameLen) {
        auto strLen = strlen(&rawReadersNames[parsingPosition]);
        if (strLen == 0) {
            break;
        }

        readerNames.emplace_back(&rawReadersNames[parsingPosition], strLen);
        parsingPosition += strLen + 1;
    }

    std::cout << "Available readers: ";
    for (const auto& reader : readerNames) {
        std::cout << reader;
    }

    std::cout << std::endl << std::endl;

    if (result != SCARD_S_SUCCESS) {
        std::cout << "SCardListReaders failed with error " << pcsc_stringify_error(result)
                  << std::endl;
        return EXIT_FAILURE;
    }

    SCARD_READERSTATE readerState[2] {0};
    readerState[0].szReader = "Alcor Micro AU9540 01 00";
    readerState[1].szReader = "\\\\?PnP?\\Notification";

    while (true) {
        result = SCardGetStatusChange(context, INFINITE, readerState, 2);

        if (result != SCARD_S_SUCCESS) {
            std::cout << "SCardGetStatusChange failed with error " << pcsc_stringify_error(result)
                      << std::endl;
            return EXIT_FAILURE;
        }

        std::cout << "Reader " << readerState[0].szReader << " state: " << std::endl;
        printState(readerState[0].dwEventState);
        readerState[0].dwCurrentState = readerState[0].dwEventState;
        std::cout << "Reader status: " << std::endl;
        printState(readerState[1].dwEventState);
        readerState[1].dwCurrentState = readerState[1].dwEventState;
    }

    return EXIT_SUCCESS;
}
