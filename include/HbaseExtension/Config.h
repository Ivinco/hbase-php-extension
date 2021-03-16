#pragma once
#include <phpcpp.h>
#include <hbase/client/zk-util.h>

namespace HbaseExtension {
    class Config : public Php::Base {

    public:
        static constexpr const char* HBASE_ZOOKEEPER_QUORUM = "HBASE_ZOOKEEPER_QUORUM";
        static constexpr const char* HBASE_ZOOKEEPER_CLIENT_PORT = "HBASE_ZOOKEEPER_CLIENT_PORT";
        static constexpr const char* HBASE_ZOOKEEPER_ZNODE_PARENT = "HBASE_ZOOKEEPER_ZNODE_PARENT";
        static constexpr const char* HBASE_ZOOKEEPER_META_REGION_SERVER = "HBASE_ZOOKEEPER_META_REGION_SERVER";
        static constexpr const char* HBASE_ZOOKEEPER_SESSION_TIMEOUT = "HBASE_ZOOKEEPER_SESSION_TIMEOUT";

        void set(Php::Parameters &params);
        const std::shared_ptr<hbase::Configuration> &getConf() const;

    private:
        std::shared_ptr<hbase::Configuration> conf_ = std::make_shared<hbase::Configuration>();

        bool isConfigKeyAllowed(const std::string& name);

    };
}

