#/bin/sh
echo starting simple subnet manager
cd ~/cubium/simple_system/subnetmanager/
./subnet-driver &
echo done

echo starting simple component-A
cd ~/cubium/simple_system/component-A/
./component-A &
echo done

echo starting simple component-B
cd ~/cubium/simple_system/component-B/
./component-B
echo done
