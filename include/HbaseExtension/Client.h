#pragma once
#include "Config.h"
#include <hbase/client/client.h>
#include <hbase/client/table.h>

namespace HbaseExtension {
    class Client : public Php::Base {
    public:
        void __construct(const Config& configuration);
        void table(Php::Parameters &params);
        void __construct1(Php::Parameters &params);
        Php::Value get(Php::Parameters &params);

    private:
        std::unique_ptr<hbase::Client> client_;
        std::unique_ptr<hbase::Table> table_;

        Php::Object convertResultToPhpObject(const std::shared_ptr<hbase::Result> &result) const;
        bool Verify(const std::shared_ptr<hbase::Result>& result);
    };

}