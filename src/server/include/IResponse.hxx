#ifndef IRESPONSE_HXX
#define IRESPONSE_HXX

#include <vector>

#include "Payload.h"

class IResponse {
   public:
    virtual ~IResponse() = default;

    virtual bool              get_response() const = 0;
    virtual std::vector<char> get_salt() const = 0;

    /**
     * @brief Encapsulates the response into a app-standardized frame.
     *
     * @return A vector of payloads.
     */
    virtual std::vector<std::vector<char>> encapsulate() = 0;
};

#endif  // IRESPONSE_HXX