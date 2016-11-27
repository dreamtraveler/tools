#!/bin/sh
echo "mkdir server_bin"
./build.sh
rm -rf server_bin
mkdir server_bin
mkdir server_bin/Assets
mkdir server_bin/Assets/Resource
mkdir server_bin/Assets/Resource/table
mkdir server_bin/server
mkdir server_bin/server/bin
mkdir server_bin/server/bin/templates

echo "mv start"
cp -PfR	../Assets/Resource/table/*.*	server_bin/Assets/Resource/table/
cp -PfR	../server/bin/templates/*.*	    server_bin/server/bin/templates/
#cp -PfR	../server/bin/server_cfg.lua	server_bin/server/bin/
#cp -PfR	../server/bin/rpConfig.json		server_bin/server/bin/
#cp -PfR	../server/bin/area.json		    server_bin/server/bin/

#cp -PfR	../server/bin/server_restart.sh	server_bin/server/bin/
#cp -PfR	../server/bin/server_start.sh	server_bin/server/bin/
cp -PfR	../server/bin/server_stop.sh	server_bin/server/bin/

cp -PfR	../server/bin/center_server_start.sh	server_bin/server/bin/
cp -PfR	../server/bin/area_server_start.sh	    server_bin/server/bin/
cp -PfR	../server/bin/game_server_start.sh	    server_bin/server/bin/


cp -f	../server/bin/dbserver		server_bin/server/bin/
cp -f	../server/bin/gateway		server_bin/server/bin/
cp -f	../server/bin/loginserver	server_bin/server/bin/
cp -f	../server/bin/worldserver	server_bin/server/bin/
cp -f	../server/bin/gmt			server_bin/server/bin/
cp -f	../server/bin/logserver		server_bin/server/bin/
cp -f	../server/bin/recharge		server_bin/server/bin/
cp -f	../server/bin/centerserver	server_bin/server/bin/
cp -f	../server/bin/RechageProxy	server_bin/server/bin/
cp -f	../server/bin/AreaServer	server_bin/server/bin/

echo "mv end"

echo "md5sum"
rm -rf MD5
md5sum server_bin/server/bin/dbserver 	  >> MD5
md5sum server_bin/server/bin/gateway     >> MD5
md5sum server_bin/server/bin/loginserver >> MD5
md5sum server_bin/server/bin/worldserver >> MD5
md5sum server_bin/server/bin/gmt         >> MD5
md5sum server_bin/server/bin/logserver   >> MD5
md5sum server_bin/server/bin/recharge    >> MD5
md5sum server_bin/server/bin/centerserver    >> MD5
md5sum server_bin/server/bin/RechageProxy    >> MD5
md5sum server_bin/server/bin/AreaServer      >> MD5
