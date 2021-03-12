#pragma once
#include <phpcpp.h>
#include <hbase/client/configuration.h>

namespace HbaseExtension {
    class Config : public Php::Base {

        public:
            void setConf(Php::Parameters &params);

        private:
            std::shared_ptr<hbase::Configuration> conf_ = std::make_shared<hbase::Configuration>();
    public:
        const std::shared_ptr<hbase::Configuration> &getConf() const;
    };
}

