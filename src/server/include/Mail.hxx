#ifndef MAIL_HXX
#define MAIL_HXX

#include <string>

class Mail {
   private:
    static Mail* m_inst;

    Mail() = default;
    ~Mail() = default;

    void sendEmail(const std::string& jsonDataStr,
                   const std::string& dest);

   public:
    Mail(const Mail& other) = delete;
    Mail(Mail&& other) = delete;

    static void  init();
    static Mail& get();
    static void  kill();

    /**
     * @brief Sends a recovery-code email.
     *
     * @param dest Destination address to send the email to.
     * @param code The recovery code to be snet
     */
    void sendRecoveryCode(const std::string& dest,
                          const std::string& code);

    /**
     * @brief Sends a welcome message.
     *
     * @param dest Destination address to send the email to.
     */
    void sendWelcomeMessage(const std::string& dest);

    /**
     * @brief Sends a message to inform that this destination
     * will now be used to send the recovery code to.
     *
     * @param dest Destination address to send the email to.
     */
    void sendChangedEmailMessage(const std::string& dest);
};

#endif  // MAIL_HXX