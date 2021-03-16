#include "../../include/HbaseExtension/Scanner.h"
#include "../../include/HbaseExtension/Scan.h"
#include <hbase/client/client.h>
#include <folly/Conv.h>
#include "../../include/HbaseExtension/ResultConverter.h"

namespace HbaseExtension {

    Php::Object ResultConverter::convertResultToPhpObject(const std::shared_ptr<hbase::Result> &result)
    {
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

    bool ResultConverter::verify(const std::shared_ptr<hbase::Result>& result) {
        return result != nullptr && !result->IsEmpty();
    }
}