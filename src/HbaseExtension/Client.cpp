#include <HBase.pb.h>
#include <folly/Conv.h>
#include <hbase/client/client.h>
#include "../../include/HbaseExtension/Config.h"
#include "../../include/HbaseExtension/Client.h"
#include "../../include/HbaseExtension/Scan.h"
#include "../../include/HbaseExtension/Scanner.h"
#include "../../include/HbaseExtension/ResultConverter.h"

namespace HbaseExtension {

    void Client::__construct(Php::Parameters &params)
    {
        checkParams(params, "Please provide config");

        auto* configuration = dynamic_cast<Config*> (params[0].implementation());

        client_ = std::make_unique<hbase::Client>(*configuration->getConf());
    }

    void Client::table(Php::Parameters &params)
    {
        checkParams(params, "Please provide table name");

        std::string tableName = params[0];
        hbase::pb::TableName tn = folly::to<hbase::pb::TableName>(tableName);
        if (table_ != nullptr) {
            table_->Close();
        }
        table_ = client_->Table(tn);
    }

    Php::Value Client::get(Php::Parameters &params)
    {
        checkTable();
        checkParams(params, "Please provide row ids");

        std::vector<std::string> rows = params[0];
        std::vector<hbase::Get> gets;

        for(const auto& row: rows) {
            hbase::Get get(row);
            gets.push_back(get);
        }

        Php::Array phpResult;
        auto results = table_->Get(gets);

        for(const auto& result: results) {
            if (!ResultConverter::Verify(result)) {
                continue;
            }
            phpResult[result->Row()] = ResultConverter::convertResultToPhpObject(result);

        }

        return phpResult;
    }

    //todo make static fpr namespace
    void Client::checkParams(Php::Parameters &params, const std::string& message)
    {
        if (params.empty()) {
            //it's not possible to create custom exceptions with PHP CPP
            throw Php::Exception(message);
        }
    }

    void Client::checkTable()
    {
        if (table_ == nullptr) {
            //it's not possible to create custom exceptions with PHP CPP
            throw Php::Exception("No table selected");
        }
    }

    Php::Value Client::openScanner(Php::Parameters &params)
    {
        checkParams(params, "Please provide scan");
        checkTable();

        return Php::Object("HBaseNativeClient\\Scanner", new Scanner(params, table_));
    }
}