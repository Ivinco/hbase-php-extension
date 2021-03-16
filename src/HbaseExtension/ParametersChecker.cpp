#include "../../include/HbaseExtension/ParametersChecker.h"

namespace HbaseExtension {
    void ParametersChecker::check(Php::Parameters &params, const std::string& message) {
        if (params.empty()) {
            //it's not possible to create custom exceptions with PHP CPP
            throw Php::Exception(message);
        }
    }

    void ParametersChecker::check(Php::Parameters &params)
    {
        if (params.empty()) {
            //it's not possible to create custom exceptions with PHP CPP
            throw Php::Exception("Please provide parameters");
        }
    }
}