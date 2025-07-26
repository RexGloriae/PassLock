#ifndef EMAIL_CHANGE_REQUEST_HXX
#define EMAIL_CHANGE_REQUEST_HXX

#include "Request.hxx"

class EmailChangeRequest : public Request {
   private:
    std::vector<char> m_username;
    std::vector<char> m_email;

   public:
    /**
     * @param username Username of the user creating this request.
     * @param email Email of the username creating this request.
     */
    EmailChangeRequest(const std::vector<char>& username,
                       const std::vector<char>& email);

    IResponse* solve() override;

    std::vector<char> get_username() const override;
    std::vector<char> get_email() const override;
};

#endif  // EMAIL_CHANGE_REQUEST_HXX