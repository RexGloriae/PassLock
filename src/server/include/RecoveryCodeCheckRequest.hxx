#ifndef RECOVERY_CODE_CHECK_REQUEST_HXX
#define RECOVERY_CODE_CHECK_REQUEST_HXX

#include "Request.hxx"

class RecoveryCodeCheckRequest : public Request {
   private:
    std::vector<char> m_username;
    std::vector<char> m_code;

   public:
    /**
     * @param username Username of the user who created this request.
     * @param code The code the user entered to reset his password.
     */
    RecoveryCodeCheckRequest(const std::vector<char>& username,
                             const std::vector<char>& code);

    IResponse* solve() override;

    std::vector<char> get_username() const override;
    std::vector<char> get_code() const override;
};

#endif  // RECOVERY_CODE_FETCH_REQUEST_HXX