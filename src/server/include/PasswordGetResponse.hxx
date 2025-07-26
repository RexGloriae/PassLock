#ifndef PASSWORD_GET_RESPONSE_HXX
#define PASSWORD_GET_RESPONSE_HXX

#include "Response.hxx"

class PasswordGetResponse : public Response {
   private:
    std::vector<std::tuple<std::vector<char>,
                           std::vector<char>,
                           std::vector<char>>>
        m_passwords;

   public:
    /**
     * @param passwords A vector of tuples, where each tuple contains:
     *
     *         - std::vector<char>: The platform where the password is
     * used.
     *
     *         - std::vector<char>: The password itself.
     *
     *         - std::vector<char>: The username associated with that
     * password on the platform.
     */
    PasswordGetResponse(
        const std::vector<std::tuple<std::vector<char>,
                                     std::vector<char>,
                                     std::vector<char>>>& passwords)
        : Response(), m_passwords(passwords) {};

    std::vector<std::vector<char>> encapsulate() override;
};

#endif  // PASSWORD_GET_RESPONSE_HXX