#ifndef SALT_FETCH_REQUEST_HXX
#define SALT_FETCH_REQUEST_HXX

#include <vector>

#include "Request.hxx"

class SaltFetchRequest : public Request {
   private:
    std::vector<char> m_username;

   public:
    /**
     * @param username Username of the user who created this request.
     */
    SaltFetchRequest(const std::vector<char>& username);

    std::vector<char> get_username() const;
    IResponse*        solve() override;
};

#endif  // SALT_FETCH_REQUEST_HXX