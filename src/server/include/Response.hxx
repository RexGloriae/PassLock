#ifndef RESPONSE_HXX
#define RESPONSE_HXX

#include "IResponse.hxx"

class Response : public IResponse {
   public:
    Response() = default;

    virtual bool                           get_response() const override;
    virtual std::vector<char>              get_salt() const override;
    virtual std::vector<std::vector<char>> encapsulate() override;
};

#endif  // RESPONSE_HXX