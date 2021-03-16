#include <phpcpp.h>

namespace HbaseExtension {
    class Scan : public Php::Base {

    public:
        void setStartRow(Php::Parameters &params);
        void setStopRow(Php::Parameters &params);
        void setCaching(Php::Parameters &params);

        const hbase::Scan &getScan() const;

    private:
        hbase::Scan scan_{};
        int64_t caching;

        void checkParams(Php::Parameters &params);
    };
}