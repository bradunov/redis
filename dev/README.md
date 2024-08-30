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

To test, download or build [redis-benchmark-go](https://github.com/redis-performance/redis-benchmark-go) and run the following:
```bash
./redis-benchmark-go -p 12000  -rueidis -r 10000000 -csc -cmd "SET __key__ __data__" -cmd-ratio 0.05 -cmd "GET __key__" -cmd-ratio 0.95
```
In [redis.conf](./redis.conf) file we reduced Redis operational memory. 
This will cause Redis to soon start printing eviction statistics. 

Other useful commands to track keys and eviction statistics during the experiment:
```
watch ./redis-cli -p 12000 info memory
watch ./redis-cli -p 12000 info keyspace
./redis-cli -p 12000 info | grep evict
```
Once the memory hits the limit (100 MB), eviction will start. 
The keyspace will reduce, first significantly, and then it will grow again. 
The actual pattern will depend on the amount of memory allocated (in [redis.conf](./redis.conf)).
 



