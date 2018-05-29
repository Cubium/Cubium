#!/bin/bash

# Usage: ./init-component.sh <component-name>

mkdir $1

echo "add_executable($1.out $1.cpp)" > $1/CMakeLists.txt
echo "target_link_libraries($1.out Cubium pthread)" >> $1/CMakeLists.txt

name="$1"
upperName=${name^^}
capitalName=${name^}


echo "#include \"../addresses.hpp\"" > $1/$1.cpp
echo "#include <component.hpp>" >> $1/$1.cpp
echo "#include <iostream>" >> $1/$1.cpp
echo "#include <unistd.h>" >> $1/$1.cpp
echo "" >> $1/$1.cpp
echo "#define COMP_NAME $capitalName" >> $1/$1.cpp
echo "#define COMP_ADDR la_$upperName" >> $1/$1.cpp
echo "#define MNGR_ADDR la_LSM" >> $1/$1.cpp
echo "" >> $1/$1.cpp
echo "class COMP_NAME : public Component" >> $1/$1.cpp
echo "{" >> $1/$1.cpp
echo "public:" >> $1/$1.cpp
echo "  COMP_NAME(std::shared_ptr<LocalCommunicator> com = nullptr) : Component(com, COMP_ADDR, MNGR_ADDR)" >> $1/$1.cpp
echo "  {" >> $1/$1.cpp
echo "  }" >> $1/$1.cpp
echo "" >> $1/$1.cpp
echo "  void handleSpaData(SpaMessage* message)" >> $1/$1.cpp
echo "  {" >> $1/$1.cpp
echo "  }" >> $1/$1.cpp
echo "" >> $1/$1.cpp
echo "  void sendData(LogicalAddress destination)" >> $1/$1.cpp
echo "  {" >> $1/$1.cpp
echo "    sleep(1);" >> $1/$1.cpp
echo "    std::string payload = \"Test string!\";" >> $1/$1.cpp
echo "    sendPayload(payload, destination);" >> $1/$1.cpp
echo "  }" >> $1/$1.cpp
echo "" >> $1/$1.cpp
echo "  void init()" >> $1/$1.cpp
echo "  {" >> $1/$1.cpp
echo "  }" >> $1/$1.cpp
echo "};" >> $1/$1.cpp
echo "" >> $1/$1.cpp
echo "int main()" >> $1/$1.cpp
echo "{" >> $1/$1.cpp
echo "  component_start<COMP_NAME>(COMP_ADDR);" >> $1/$1.cpp
echo "  return EXIT_SUCCESS;" >> $1/$1.cpp
echo "}" >> $1/$1.cpp

echo $1 >> $1/.gitignore
