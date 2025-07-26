#ifndef SALT_FETCH_RESPONSE_HXX
#define SALT_FETCH_RESPONSE_HXX

#include "Response.hxx"

class SaltFetchResponse : public Response {
   private:
    std::vector<char> m_salt;

   public:
    /**
     * @param salt Vector representing the salt used for client-side
     * encryption.
     */
    SaltFetchResponse(const std::vector<char>& salt);

    std::vector<char> get_salt() const;

    std::vector<std::vector<char>> encapsulate() override;
};

#endif  // SALT_FETCH_RESPONSE_HXX