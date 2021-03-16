#include <phpcpp.h>

namespace HbaseExtension {
    class ParametersChecker {
    public:
        void check(Php::Parameters &params, const std::string& message);
        void check(Php::Parameters &params);
    };
}