#include <hbase/serde/rpc-serde.h>
#include <hbase/client/scan.h>
#include "../../include/HbaseExtension/Scan.h"

namespace HbaseExtension {

    void Scan::setStartRow(Php::Parameters &params)
    {
        checkParams(params);
        std::string startRow = params[0];
        scan_.SetStartRow(startRow);
    }

    void Scan::setStopRow(Php::Parameters &params)
    {
        checkParams(params);
        std::string stopRow = params[0];
        scan_.SetStopRow(stopRow);
    }

    void Scan::setCaching(Php::Parameters &params)
    {
        checkParams(params);
        caching = params[0];
        scan_.SetCaching(caching);
    }

    void Scan::checkParams(Php::Parameters &params)
    {
        if (params.empty()) {
            //it's not possible to create custom exceptions with PHP CPP
            throw Php::Exception("Please provide parameters");
        }
    }

    const hbase::Scan &Scan::getScan() const
    {
        return scan_;
    }
}
