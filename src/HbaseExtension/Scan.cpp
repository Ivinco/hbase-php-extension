#include <hbase/serde/rpc-serde.h>
#include <hbase/client/scan.h>
#include "../../include/HbaseExtension/Scan.h"

namespace HbaseExtension {

    void Scan::setStartRow(Php::Parameters &params)
    {
        parametersChecker_.check(params);
        std::string startRow = params[0];
        scan_.SetStartRow(startRow);
    }

    void Scan::setStopRow(Php::Parameters &params)
    {
        parametersChecker_.check(params);
        std::string stopRow = params[0];
        scan_.SetStopRow(stopRow);
    }

    void Scan::setCaching(Php::Parameters &params)
    {
        parametersChecker_.check(params);
        int64_t caching = params[0];
        scan_.SetCaching(caching);
    }

    const hbase::Scan &Scan::getScan() const
    {
        return scan_;
    }
}
