namespace HbaseExtension {
    class ResultConverter {

    public:
        static bool Verify(const std::shared_ptr<hbase::Result> &result);

        static Php::Object convertResultToPhpObject(const std::shared_ptr<hbase::Result> &result);
    };
}

