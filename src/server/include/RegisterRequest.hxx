#ifndef REGISTER_REQUEST_HXX
#define REGISTER_REQUEST_HXX

#include <vector>

#include "Request.hxx"

class RegisterRequest : public Request {
   private:
    std::vector<char> m_username;
    std::vector<char> m_pass_hash;
    std::vector<char> m_salt;
    std::vector<char> m_email;

   public:
    /**
     * @param username Username of the user who created this request.
     * @param hash Hash of the password of the user.
     * @param salt Salt generated for client-side encryption.
     * @param email Email of the user.
     */
    RegisterRequest(const std::vector<char>& username,
                    const std::vector<char>& hash,
                    const std::vector<char>& salt,
                    const std::vector<char>& email);

    IResponse* solve() override;

    std::vector<char> get_username() const;
    std::vector<char> get_email() const;
    std::vector<char> get_pass_hash() const;
    std::vector<char> get_salt() const;
};

#endif  // REGISTER_REQUEST_HXX