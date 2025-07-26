#ifndef RECOVERY_CODE_FETCH_REQUEST_HXX
#define RECOVERY_CODE_FETCH_REQUEST_HXX

#include "Request.hxx"

class RecoveryCodeFetchRequest : public Request {
   private:
    std::vector<char> m_username;

   public:
    /**
     * @param username Username of the user who created this request.
     */
    RecoveryCodeFetchRequest(const std::vector<char>& username);

    IResponse* solve() override;

    std::vector<char> get_username() const override;
};

#endif  // RECOVERY_CODE_FETCH_REQUEST_HXX