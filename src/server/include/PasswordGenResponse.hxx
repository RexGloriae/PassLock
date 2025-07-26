#ifndef PASSWORD_GEN_RESPONSE_HXX
#define PASSWORD_GEN_RESPONSE_HXX

#include "Response.hxx"

class PasswordGenResponse : public Response {
   private:
    std::vector<char> m_pass;

   public:
    /**
     * @param pass Vector representing the generated password.
     */
    PasswordGenResponse(const std::vector<char>& pass)
        : Response(), m_pass(pass) {};

    std::vector<std::vector<char>> encapsulate() override;
};

#endif  // PASSWORD_GEN_RESPONSE_HXX