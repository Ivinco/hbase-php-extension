#include "../../include/HbaseExtension/Config.h"

namespace HbaseExtension {
    void Config::set(Php::Parameters &params)
    {
        if (params.size() < 2) {
            //it's not possible to create custom exceptions with PHP CPP
            throw Php::Exception("Please provide config");
        }

        std::string name = params[0];
        if (!isConfigKeyAllowed(name)) {
            //it's not possible to create custom exceptions with PHP CPP
            throw Php::Exception(name + " config key is not supported");
        }


        std::string value = params[1];

        conf_->Set(name, value);
    }

    const std::shared_ptr<hbase::Configuration> &Config::getConf() const {
        return conf_;
    }

    // todo refactor
    // put all of these constants into some structure, check their presence with some find() method
    // also refactor main
    bool Config::isConfigKeyAllowed(const std::string& name) {
        return name == hbase::ZKUtil::kHBaseZookeeperQuorum_
            || name == hbase::ZKUtil::kHBaseZookeeperClientPort_
            || name == hbase::ZKUtil::kHBaseZnodeParent_
            || name == hbase::ZKUtil::kHBaseMetaRegionServer_
            || name == hbase::ZKUtil::kHBaseZookeeperSessionTimeout_;
    }

}

