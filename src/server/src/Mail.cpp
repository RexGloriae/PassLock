#include "../include/Mail.hxx"

#include <windows.h>
#include <winhttp.h>

#include <iostream>
#include <string>
#pragma comment(lib, "winhttp.lib")  // << Link WinHTTP

#include "../include/Logger.hxx"
#include "../include/Secret.h"

Mail* Mail::m_inst = nullptr;

void Mail::sendEmail(const std::string& jsonDataStr,
                     const std::string& dest) {
    // Define server and URL path
    LPCWSTR server = L"script.google.com";
    LPCWSTR url = URL;

    const char* jsonData = jsonDataStr.c_str();

    DWORD dataLength = (DWORD)strlen(jsonData);

    // Initialize WinHTTP session
    HINTERNET hSession = WinHttpOpen(L"A WinHTTP Example/1.0",
                                     WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                                     WINHTTP_NO_PROXY_NAME,
                                     WINHTTP_NO_PROXY_BYPASS,
                                     0);

    if (!hSession) {
        std::cerr << "WinHttpOpen failed.\n";
        return;
    }

    // Connect to server
    HINTERNET hConnect =
        WinHttpConnect(hSession, server, INTERNET_DEFAULT_HTTPS_PORT, 0);

    if (!hConnect) {
        std::cerr << "WinHttpConnect failed.\n";
        WinHttpCloseHandle(hSession);
        return;
    }

    // Create a request handle
    HINTERNET hRequest = WinHttpOpenRequest(hConnect,
                                            L"POST",
                                            url,
                                            NULL,
                                            WINHTTP_NO_REFERER,
                                            WINHTTP_DEFAULT_ACCEPT_TYPES,
                                            WINHTTP_FLAG_SECURE);  // HTTPS

    if (!hRequest) {
        std::cerr << "WinHttpOpenRequest failed.\n";
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return;
    }

    // Set headers
    BOOL bResults =
        WinHttpAddRequestHeaders(hRequest,
                                 L"Content-Type: application/json\r\n",
                                 (DWORD)-1L,
                                 WINHTTP_ADDREQ_FLAG_ADD);

    // Send a request
    bResults = WinHttpSendRequest(hRequest,
                                  WINHTTP_NO_ADDITIONAL_HEADERS,
                                  0,
                                  (LPVOID)jsonData,
                                  dataLength,
                                  dataLength,
                                  0);

    if (!bResults) {
        std::cerr << "WinHttpSendRequest failed.\n";
    } else {
        // Receive a response
        bResults = WinHttpReceiveResponse(hRequest, NULL);

        if (bResults) {
            DWORD dwSize = 0;
            DWORD dwDownloaded = 0;
            LPSTR pszOutBuffer;
            do {
                dwSize = 0;
                if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
                    std::cerr << "WinHttpQueryDataAvailable failed.\n";
                    break;
                }

                if (!dwSize) break;

                pszOutBuffer = new char[dwSize + 1];
                if (!pszOutBuffer) {
                    std::cerr << "Out of memory.\n";
                    break;
                }

                ZeroMemory(pszOutBuffer, dwSize + 1);

                if (!WinHttpReadData(hRequest,
                                     (LPVOID)pszOutBuffer,
                                     dwSize,
                                     &dwDownloaded)) {
                    std::cerr << "WinHttpReadData failed.\n";
                } else {
                    // std::cout << "Response: " << pszOutBuffer << "\n";
                }

                delete[] pszOutBuffer;

            } while (dwSize > 0);
        } else {
            std::cerr << "WinHttpReceiveResponse failed.\n";
        }
    }

    Logger::get().write("An email has been successfully sent to" + dest);

    // Cleanup
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
}

void Mail::init() {
    if (Mail::m_inst == nullptr) {
        Mail::m_inst = new Mail();
    }
}
Mail& Mail::get() { return *(Mail::m_inst); }
void  Mail::kill() {
    if (Mail::m_inst != nullptr) {
        delete Mail::m_inst;
    }
    Mail::m_inst = nullptr;
}

void Mail::sendRecoveryCode(const std::string& dest,
                            const std::string& code) {
    std::string secret = SECRET;
    std::string jsonDataStr = R"({
    "secret": ")" + secret + R"(",
    "to": ")" + dest + R"(",
    "subject": "Passlock Password Recovery",
    "body": "A code has been requested to recover your password.\nIf you haven't requested this, please ignore the code.\nCode: )" +
                              code + R"("
    })";

    sendEmail(jsonDataStr, dest);
}

void Mail::sendWelcomeMessage(const std::string& dest) {
    std::string secret = SECRET;
    std::string jsonDataStr = R"({
    "secret": ")" + secret + R"(",
    "to": ")" + dest + R"(",
    "subject": "Welcome to PassLock!",
    "body": "Thank you for joining Passlock!\nWe hope you will enjoy using our application! For any further assistance, please feel free to email back on this address!"
    })";

    sendEmail(jsonDataStr, dest);
}

void Mail::sendChangedEmailMessage(const std::string& dest) {
    std::string secret = SECRET;
    std::string jsonDataStr = R"({
        "secret": ")" + secret +
                              R"(",
        "to": ")" + dest + R"(",
        "subject": "Email Change!",
        "body": "We confirm the email change to your PassLock account!\nIf you ever forget your password, the recovery code will now be sent to this email."
        })";

    sendEmail(jsonDataStr, dest);
}