#include <phpcpp.h>
#include <iostream>
#include "include/HbaseExtension/Config.h"
#include "include/HbaseExtension/Client.h"



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
    hbaseExtensionConfiguration.method<&HbaseExtension::Config::setConf> ("setConf", {
            Php::ByVal("name", Php::Type::String),
            Php::ByVal("value", Php::Type::String)
    });

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

    hbaseNativeClient.add(std::move(hbaseExtensionConfiguration));
    hbaseNativeClient.add(std::move(hbaseExtensionClient));
    extension.add(std::move(hbaseNativeClient));

    return extension;
}
}
