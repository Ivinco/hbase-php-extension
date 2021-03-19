#!/bin/bash

/usr/src/hbase/bin/start-hbase.sh

echo "
  create_namespace 'world';
  create 'world:country', 'info', 'info';
  put 'world:country', 'russia', 'info:language', 'Russian';
  put 'world:country', 'russia', 'info:name', 'Russian Federation';
  put 'world:country', 'germany', 'info:language', 'German';
  put 'world:country', 'germany', 'info:name', 'Germany';
  put 'world:country', 'usa', 'info:language', 'English';
  put 'world:country', 'usa', 'info:name', 'United States of America';
  put 'world:country', 'uk', 'info:language', 'English';
  put 'world:country', 'uk', 'info:name', 'United Kingdom of Great Britain and Northern Ireland';
" | /usr/src/hbase/bin/hbase shell