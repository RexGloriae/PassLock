#include "../include/RecoveryCodeFetchResponse.hxx"

#include "../include/Packager.hxx"

std::vector<std::vector<char>> RecoveryCodeFetchResponse::encapsulate() {
    std::vector<char> payload =
        Packager::get().encapsulate_recovery_code_fetch_response(m_status);

    std::vector<std::vector<char>> frame;
    frame.push_back(payload);
    return frame;
}