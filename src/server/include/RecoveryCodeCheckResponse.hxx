#ifndef RECOVERY_CODE_CHECK_RESPONSE_HXX
#define RECOVERY_CODE_CHECK_RESPONSE_HXX

#include "Response.hxx"

class RecoveryCodeCheckResponse : public Response {
   private:
    bool m_status;

   public:
    /**
     * @param status True if the recovery code is the right one, False
     * otherwise.
     */
    RecoveryCodeCheckResponse(bool status)
        : Response(), m_status(status) {};

    std::vector<std::vector<char>> encapsulate() override;
};

#endif  //  RECOVERY_CODE_FETCH_RESPONSE_HXX