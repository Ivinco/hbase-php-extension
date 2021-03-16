#pragma once
#include "Config.h"
#include "ParametersChecker.h"
#include "ResultConverter.h"
#include <hbase/client/client.h>
#include <hbase/client/table.h>

namespace HbaseExtension {
    class Client : public Php::Base {
    public:
        void table(Php::Parameters &params);
        void __construct(Php::Parameters &params);
        Php::Value get(Php::Parameters &params);
        Php::Value openScanner(Php::Parameters &params);
        void close();

    private:
        std::unique_ptr<hbase::Client> client_;
        std::shared_ptr<hbase::Table> table_;
        ParametersChecker parametersChecker_{};
        ResultConverter resultConverter_{};

    private:
        void checkTable();
    };

}