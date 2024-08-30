# How to run Janus Redis tests


## Preparation

Build Janus and run `setup_janus_env.sh` in Janus root in every shell window you use for the demo. 
To build redis with Janus, run:
```bash
make BUILD_JANUS=yes
```
You need to set the parameter only once. For every subsequent build, just type `make`.


## Start Redis

Type the following command line:
```bash
cd ~/redis/src
~/janus/setup_janus_env.sh
./redis-server ../dev/redis.conf
```


## Run codelet examples

### Expire key codelet

This codelet is called when Redis checks key expiration. 
To build and load the codelet, type:
```bash
cd ~/redis/dev/codeletes/expire
~/janus/setup_janus_env.sh
make
./load.sh
```

To test, run the following:
```bash
cd ~/redis/src
./redis-cli -p 12000 SET AA B EX 3
```
This will set a key `AA` with expiration time 3s. 
Redis will then start the expiration check which will call a Janus hook and print stats in every loop. 



### Eviction codelet

This codelet is called when Redis starts looking for a key to evict due to a lack of memory. 
To build and load the codelet, type:
```bash
cd ~/redis/dev/codeletes/evict_key
~/janus/setup_janus_env.sh
make
./load.sh
```

To test, download or build (redis-benchmark-go)[https://github.com/redis-performance/redis-benchmark-go] and run the following:
```bash
./redis-benchmark-go -p 12000  -rueidis -r 1000000 -csc -cmd "SET __key__ __data__" -cmd-ratio 0.05 -cmd "GET __key__" -cmd-ratio 0.95
```
In [redis.conf](./redis.conf) file we reduced Redis operational memory. 
This will cause Redis to soon start printing eviction statistics. 

At some point Redis will return Out-Of-Memory and will stop serving requests or trying to evict. It seems that the at that point the system has used all the memory but has no more keys to evict. It may be that the memory is used for other purposes. I don't fully understand why this is happening. But for a good benchmark we need to create a setup that will continuously add and evict keys. This is TBD. 



