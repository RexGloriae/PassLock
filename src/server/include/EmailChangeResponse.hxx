#ifndef EMAIL_CHANGE_RESPONSE_HXX
#define EMAIL_CHANGE_RESPONSE_HXX

#include "Response.hxx"

class EmailChangeResponse : public Response {
   private:
    bool m_status;

   public:
    /**
     * @param status True if the email was changed, False otherwise.
     */
    EmailChangeResponse(bool status) : Response(), m_status(status) {};

    std::vector<std::vector<char>> encapsulate() override;
};

#endif  // EMAIL_CHANGE_RESPONSE_HXX