#/bin/sh
echo starting python subnet manager
cd ~/cubium/python_system/subnetmanager/
./py-subnet-driver &
echo done

echo starting python component-A
cd ~/cubium/python_system/py-component-A/
./py-component-A &
echo done

echo starting python component-B
cd ~/cubium/python_system/py-component-B/
./py-component-B
echo done
