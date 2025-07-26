#ifndef REGISTERRESPONSE_HXX
#define REGISTERRESPONSE_HXX

#include "Response.hxx"

class RegisterResponse : public Response {
   private:
    bool m_answ;

   public:
    /**
     * @param answ True if the registration was done successfully, False
     * otherwise.
     */
    RegisterResponse(bool answ) : Response(), m_answ(answ) {};

    bool get_response() const;

    std::vector<std::vector<char>> encapsulate() override;
};

#endif  // REGISTERRESPONSE_HXX