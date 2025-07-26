#ifndef PASSWORD_GET_REQUEST_HXX
#define PASSWORD_GET_REQUEST_HXX

#include "Request.hxx"

class PasswordGetRequest : public Request {
   private:
    std::vector<char> m_username;

   public:
    /**
     * @param username Username of the user who created this request.
     */
    PasswordGetRequest(const std::vector<char>& username);

    IResponse* solve() override;

    std::vector<char> get_username() const override;
};

#endif  // PASSWORD_GET_REQUEST_HXX