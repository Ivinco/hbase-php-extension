
#include <hbase/client/client.h>
#include "../../include/HbaseExtension/Scanner.h"
#include "../../include/HbaseExtension/Scan.h"
#include "../../include/HbaseExtension/ResultConverter.h"

namespace HbaseExtension {
    Scanner::Scanner(Php::Parameters &params, const std::shared_ptr<hbase::Table>& table_)
    {
        auto* scan = dynamic_cast<Scan*> (params[0].implementation());
        close();
        scan_ = scan->getScan();
        scanner_ = table_->Scan(scan_);
    }

    void Scanner::close()
    {
        if (scanner_ != nullptr) {
            scanner_->Close();
        }
    }

    Php::Value Scanner::getList()
    {
        if (scanner_ == nullptr) {
            //it's not possible to create custom exceptions with PHP CPP
            throw Php::Exception("You should init scanner first with an 'Client::openScan function'");
        }

        Php::Array phpResult;
        int64_t limit = scan_.Caching();

        if (limit <= 0 ) {
            //it's not possible to create custom exceptions with PHP CPP
            throw Php::Exception("Caching must be greater, than 0; " + std::to_string(limit) + " is incorrect");
        }

        while (limit-- > 0) {
            auto result = scanner_->Next();
            if (!resultConverter_.verify(result)) {
                continue;
            }
            phpResult[result->Row()] = resultConverter_.convertResultToPhpObject(result);
        }

        return phpResult;
    }
}


