#ifndef LOGINRESPONSE_HXX
#define LOGINRESPONSE_HXX

#include "Response.hxx"

class LoginResponse : public Response {
   private:
    bool m_answ;

   public:
    /**
     * @param answ True if the login was done successfully, False
     * otherwise.
     */
    LoginResponse(bool answ) : Response(), m_answ(answ) {};

    bool get_response() const;

    std::vector<std::vector<char>> encapsulate() override;
};

#endif  // LOGINRESPONSE_HXX