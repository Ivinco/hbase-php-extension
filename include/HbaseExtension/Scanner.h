#pragma once
#include <phpcpp.h>
#include <hbase/client/result-scanner.h>
#include <hbase/client/table.h>
#include <hbase/client/scan.h>
#include "ResultConverter.h"

namespace HbaseExtension {
    class Scanner :  public Php::Base, public Php::Object {
    public:
        Scanner(Php::Parameters &params, const std::shared_ptr<hbase::Table>& table_);
        void close();
        Php::Value getList();

    private:
        std::shared_ptr<hbase::ResultScanner> scanner_;
        hbase::Scan scan_;
        ResultConverter resultConverter_{};
    };
}