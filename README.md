# Extension to work with HBase directly from PHP.

It's built around  [HBase native client](https://github.com/apache/hbase-native-client) with [PHP CPP library](http://www.php-cpp.com/)

:exclamation: Currently, it read-only, e.g. "scan" and "get" are implemented. No puts, deletes, creates, etc.

:exclamation::exclamation: **It's not production ready.** We've stopped working on it because of the [memory leak](https://issues.apache.org/jira/browse/HBASE-25581)

## Usage

Create config object

```php
$hbaseConf = new HBaseNativeClient\Config();
$hbaseConf->set(HBaseNativeClient\Config::HBASE_ZOOKEEPER_QUORUM, "host");
$hbaseConf->set(HBaseNativeClient\Config::HBASE_ZOOKEEPER_CLIENT_PORT, "2181");
$hbaseConf->set(HBaseNativeClient\Config::HBASE_ZOOKEEPER_ZNODE_PARENT, "/hbase-unsecure");
```

Initialize client and select a table

```php
$hbaseClient = new HBaseNativeClient\Client($hbaseConf);
$hbaseClient->table("ns:table");
```

Get some documents

```php
$hbaseClient->get(['rowkey']);
```

Scan table using with open scanner
```php
$scan = new HBaseNativeClient\Scan();
$scan->setCaching(2);
$scan->setStartRow("0");
$scan->setStopRow("9");

$scanner = $hbaseClient->openScanner($scan);

do {
    $res = $scanner->getList();

    foreach ($res as $rowKey => $row) {
        echo $rowKey, PHP_EOL;
    }

} while (!empty($res));

$scanner->close();
```

Close the client
```php
$hbaseClient->close();
```


## Building and playing around

There a Docker container to build up the entire development environment
Just build a container (heads-up - it takes some time)

```shell script
docker build --tag hbase-php-extension . 
```

Run it

```shell script
docker run --rm -it --name hbase -v $PWD:/usr/src/hbase-php-extension -w /usr/src/hbase-php-extension hbase-php-extension /bin/bash
```

Build extension 
```shell script
cmake -DHBASE_NATIVE_CLIENT_PATH=/usr/src/hbase-native-client . && make
```

Check it's working - this command should output 'phphbase'
```shell script
php -m | grep hbase
```

Start HBase instance and fill in some data
```shell script
./start-hbase.sh
```

And run demo script
```shell script
php demo.php
```
