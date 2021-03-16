#include <phpcpp.h>
#include <iostream>
#include "include/HbaseExtension/Config.h"
#include "include/HbaseExtension/Client.h"
#include "include/HbaseExtension/Scan.h"
#include "include/HbaseExtension/Scanner.h"



/**
 *  tell the compiler that the get_module is a pure C function
 */
extern "C" {

/**
 *  Function that is called by PHP right after the PHP process
 *  has started, and that returns an address of an internal PHP
 *  strucure with all the details and features of your extension
 *
 *  @return void*   a pointer to an address that is understood by PHP
 */
PHPCPP_EXPORT void *get_module()
{
    static Php::Extension extension("hello", "1.0");

    Php::Namespace hbaseNativeClient("HBaseNativeClient");

    Php::Class<HbaseExtension::Config> hbaseExtensionConfiguration("Config");
    hbaseExtensionConfiguration.method<&HbaseExtension::Config::set> ("set", {
            Php::ByVal("name", Php::Type::String),
            Php::ByVal("value", Php::Type::String)
    });

    hbaseExtensionConfiguration.add(Php::Constant(HbaseExtension::Config::HBASE_ZOOKEEPER_QUORUM, hbase::ZKUtil::kHBaseZookeeperQuorum_));
    hbaseExtensionConfiguration.add(Php::Constant(HbaseExtension::Config::HBASE_ZOOKEEPER_CLIENT_PORT, hbase::ZKUtil::kHBaseZookeeperClientPort_));
    hbaseExtensionConfiguration.add(Php::Constant(HbaseExtension::Config::HBASE_ZOOKEEPER_ZNODE_PARENT, hbase::ZKUtil::kHBaseZnodeParent_));
    hbaseExtensionConfiguration.add(Php::Constant(HbaseExtension::Config::HBASE_ZOOKEEPER_META_REGION_SERVER, hbase::ZKUtil::kHBaseMetaRegionServer_));
    hbaseExtensionConfiguration.add(Php::Constant(HbaseExtension::Config::HBASE_ZOOKEEPER_SESSION_TIMEOUT, hbase::ZKUtil::kHBaseZookeeperSessionTimeout_));

    Php::Class<HbaseExtension::Client> hbaseExtensionClient("Client");
    hbaseExtensionClient.method<&HbaseExtension::Client::__construct> ("__construct", {
            Php::ByVal("config", "HBaseNativeClient\\Config")
    });
    hbaseExtensionClient.method<&HbaseExtension::Client::table> ("table", {
            Php::ByVal("tableName", Php::Type::String)
    });
    hbaseExtensionClient.method<&HbaseExtension::Client::get>     ("get", {
            Php::ByVal("rowKeys", Php::Type::Array)
    });
    hbaseExtensionClient.method<&HbaseExtension::Client::openScanner>     ("openScanner", {
            Php::ByVal("scan", "HBaseNativeClient\\Scan")
    });

    Php::Class<HbaseExtension::Scan> hbaseExtensionScan("Scan");
    hbaseExtensionScan.method<&HbaseExtension::Scan::setStartRow> ("setStartRow", {
            Php::ByVal("startRow", Php::Type::String)
    });
    hbaseExtensionScan.method<&HbaseExtension::Scan::setStopRow> ("setStopRow", {
            Php::ByVal("stopRow", Php::Type::String)
    });
    hbaseExtensionScan.method<&HbaseExtension::Scan::setCaching> ("setCaching", {
            Php::ByVal("caching", Php::Type::Numeric)
    });

    Php::Class<HbaseExtension::Scanner> hbaseExtensionScanner("Scanner");
    hbaseExtensionScanner.method<&HbaseExtension::Scanner::getList> ("getList");
    hbaseExtensionScanner.method<&HbaseExtension::Scanner::close> ("close");
    hbaseExtensionScanner.method("__construct", Php::Private);
    hbaseExtensionScanner.method("__clone", Php::Private);

    hbaseNativeClient.add(std::move(hbaseExtensionConfiguration));
    hbaseNativeClient.add(std::move(hbaseExtensionClient));
    hbaseNativeClient.add(std::move(hbaseExtensionScan));
    hbaseNativeClient.add(std::move(hbaseExtensionScanner));
    extension.add(std::move(hbaseNativeClient));

    return extension;
}
}
