<?php


$hbaseConf = new HBaseNativeClient\Config();
$hbaseConf->set(HBaseNativeClient\Config::HBASE_ZOOKEEPER_QUORUM, "localhost");
$hbaseConf->set(HBaseNativeClient\Config::HBASE_ZOOKEEPER_CLIENT_PORT, "2181");
//$hbaseConf->set(HBaseNativeClient\Config::HBASE_ZOOKEEPER_ZNODE_PARENT, "/hbase-unsecure");

$hbaseClient = new HBaseNativeClient\Client($hbaseConf);
$hbaseClient->table("world:country");

$scan = new HBaseNativeClient\Scan();
$scan->setCaching(2);
$scan->setStartRow("0");

$scanner = $hbaseClient->openScanner($scan);

echo "Scan results \n";
echo "------------ \n";
do {
    $res = $scanner->getList();

    foreach ($res as $rowKey => $row) {
        echo $rowKey, PHP_EOL;
    }

} while (!empty($res));

$scanner->close();

echo "------------ \n";


echo "Get 'russia' results \n";
echo "------------ \n";
print_r($hbaseClient->get(['russia']));
echo "------------ \n";

$hbaseClient->close();

