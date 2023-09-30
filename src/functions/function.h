
#ifndef CPW_FUNCTIONS_FUNCTION_H
#define CPW_FUNCTIONS_FUNCTION_H


#include <string>
#include <memory>

#include "filters/filters_manager.h"


namespace CPW
{
    namespace Functions
    {
        class Function;
    }
}


class CPW::Functions::Function
{
    public:
        Function(std::string endpoint);

        std::string get_endpoint() const { return endpoint_; }
        std::shared_ptr<Filters::FiltersManager>& get_filters()
        {
            auto& var = filters_;
            return var;
        }
        std::shared_ptr<Filters::FiltersManager>& get_triggers()
        {
            auto& var = triggers_;
            return var;
        }

        void set_endpoint(std::string endpoint) { endpoint_ = endpoint; }

    private:
        std::string endpoint_;
        std::shared_ptr<Filters::FiltersManager> filters_;
        std::shared_ptr<Filters::FiltersManager> triggers_;

};


#endif // CPW_FUNCTIONS_FUNCTION_H
