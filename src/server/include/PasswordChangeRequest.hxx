#ifndef PASSWORD_CHANGE_REQUEST_HXX
#define PASSWORD_CHANGE_REQUEST_HXX

#include "Request.hxx"

class PasswordChangeRequest : public Request {
   private:
    std::vector<char> m_username;
    std::vector<char> m_hash;

   public:
    /**
     * @param username Username of the user who created this request.
     * @param hash Hash of the new password.
     */
    PasswordChangeRequest(const std::vector<char>& username,
                          const std::vector<char>& hash);

    IResponse* solve() override;

    std::vector<char> get_username() const override;
    std::vector<char> get_pass_hash() const override;
};

#endif  // PASSWORD_CHANGE_REQUEST_HXX