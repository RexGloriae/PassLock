#ifndef LOGIN_REQUEST_HXX
#define LOGIN_REQUEST_HXX

#include <vector>

#include "Request.hxx"

class LoginRequest : public Request {
   private:
    std::vector<char> m_username;
    std::vector<char> m_pass_hash;

   public:
    /**
     * @param username Username of the user creating this request.
     * @param hash Hash of the password used when trying to login.
     */
    LoginRequest(const std::vector<char>& username,
                 const std::vector<char>& hash);

    std::vector<char> get_username() const;
    std::vector<char> get_pass_hash() const;
    IResponse*        solve() override;
};

#endif  // LOGIN_REQUEST_HXX