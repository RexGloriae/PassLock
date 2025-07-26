#ifndef PASSWORD_DEL_REQUEST_HXX
#define PASSWORD_DEL_REQUEST_HXX

#include "Request.hxx"

class PasswordDelRequest : public Request {
   private:
    std::vector<char> m_username;
    std::vector<char> m_stored_pass;

   public:
    /**
     * @param username Username of the user who created this request.
     * @param pass Vector representing the generated password to be
     * deleted.
     */
    PasswordDelRequest(const std::vector<char>& username,
                       const std::vector<char>& pass);

    IResponse* solve() override;

    std::vector<char> get_username() const override;
    std::vector<char> get_stored_pass() const override;
};

#endif  // PASSWORD_DEL_REQUEST_HXX