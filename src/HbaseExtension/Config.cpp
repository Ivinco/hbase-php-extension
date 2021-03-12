#include <hbase/client/configuration.h>
#include "../../include/HbaseExtension/Config.h"

namespace HbaseExtension {
    void Config::setConf(Php::Parameters &params)
    {
        std::string name = params[0];
        std::string value = params[1];

        conf_->Set(name, value);
    }

    const std::shared_ptr<hbase::Configuration> &Config::getConf() const {
        return conf_;
    }

}

