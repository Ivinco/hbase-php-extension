#pragma once
#include <hbase/client/result.h>

namespace HbaseExtension {
    class ResultConverter {

    public:
        bool verify(const std::shared_ptr<hbase::Result> &result);
        Php::Object convertResultToPhpObject(const std::shared_ptr<hbase::Result> &result);
    };
}

