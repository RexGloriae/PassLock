#ifndef RECOVERY_CODE_FETCH_RESPONSE_HXX
#define RECOVERY_CODE_FETCH_RESPONSE_HXX

#include "Response.hxx"

class RecoveryCodeFetchResponse : public Response {
   private:
    bool m_status;

   public:
    /**
     * @param status True if the recovery code was sent successfully, False
     * otherwise.
     */
    RecoveryCodeFetchResponse(bool status)
        : Response(), m_status(status) {};

    std::vector<std::vector<char>> encapsulate() override;
};

#endif  //  RECOVERY_CODE_FETCH_RESPONSE_HXX