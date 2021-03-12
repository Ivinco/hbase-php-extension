#include <HBase.pb.h>
#include <folly/Conv.h>
#include <hbase/client/client.h>
#include "../../include/HbaseExtension/Config.h"
#include "../../include/HbaseExtension/Client.h"

namespace HbaseExtension {

    void Client::__construct(Php::Parameters &params)
    {
        if (params.empty()) {
            //it's not possible to create custom exceptions with PHP CPP
            throw Php::Exception("Please provide config");
        }

        auto* configuration = dynamic_cast<Config*> (params[0].implementation());

        client_ = std::make_unique<hbase::Client>(*configuration->getConf());
    }

    void Client::table(Php::Parameters &params)
    {
        if (params.empty()) {
            //it's not possible to create custom exceptions with PHP CPP
            throw Php::Exception("Please provide table name");
        }

        std::string tableName = params[0];
        hbase::pb::TableName tn = folly::to<hbase::pb::TableName>(tableName);
        if (table_ != nullptr) {
            table_->Close();
        }
        table_ = client_->Table(tn);
    }

    Php::Value Client::get(Php::Parameters &params)
    {
        if (table_ == nullptr) {
            //it's not possible to create custom exceptions with PHP CPP
            throw Php::Exception("No table selected");
        }

        if (params.empty()) {
            //it's not possible to create custom exceptions with PHP CPP
            throw Php::Exception("Please provide row ids");
        }

        std::vector<std::string> rows = params[0];
        std::vector<hbase::Get> gets;

        for(const auto& row: rows) {
            hbase::Get get(row);
            gets.push_back(get);
        }

        Php::Array phpResult;
        auto results = table_->Get(gets);

        for(const auto& result: results) {
            if (!Verify(result)) {
                continue;
            }
            phpResult[result->Row()] = convertResultToPhpObject(result);

        }

        return phpResult;
    }

    Php::Object Client::convertResultToPhpObject(const std::shared_ptr<hbase::Result> &result) const {
        //Map<family,Map<qualifier,Map<timestamp,value>>>>
        //    std::map<std::string,
        //             std::map<std::string, std::map<int64_t, std::string, std::greater<int64_t> > > >;
        //most recent ts value is stored first
        hbase::ResultMap resultMap = result->Map();

        Php::Array columns;

        for(auto const &ent1 : resultMap) {
            auto family = ent1.first;
            Php::Object valueObject;
            for(auto const &ent2 : ent1.second) {
                Php::Object columnValue;
                columnValue["value"] = ent2.second.begin()->second;
                auto qualifier = ent2.first;

                columns[family + ":" + qualifier] = columnValue;
            }
        }

        Php::Object returnResult;
        returnResult["row"] = result->Row();
        returnResult["columns"] = columns;

        return returnResult;
    }

    bool Client::Verify(const std::shared_ptr<hbase::Result>& result) {
        return result != nullptr && !result->IsEmpty();
    }


}