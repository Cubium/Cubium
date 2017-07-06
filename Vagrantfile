# -*- mode: ruby -*-
# vi: set ft=ruby :

# All Vagrant configuration is done below. The "2" in Vagrant.configure
# configures the configuration version (we support older styles for
# backwards compatibility). Please don't change it unless you know what
# you're doing.
Vagrant.configure(2) do |config|
  # The most common configuration options are documented and commented below.
  # For a complete reference, please see the online documentation at
  # https://docs.vagrantup.com.

  # Every Vagrant development environment requires a box. You can search for
  # boxes at https://atlas.hashicorp.com/search.
  config.vm.box = "debian/jessie64"

  # Disable automatic box update checking. If you disable this, then
  # boxes will only be checked for updates when the user runs
  # `vagrant box outdated`. This is not recommended.
  config.vm.box_check_update = true

  # Create a forwarded port mapping which allows access to a specific port
  # within the machine from a port on the host machine. In the example below,
  # accessing "localhost:8080" will access port 80 on the guest machine.
  # config.vm.network "forwarded_port", guest: 80, host: 8080

  # Create a private network, which allows host-only access to the machine
  # using a specific IP.
  # config.vm.network "private_network", ip: "192.168.33.10"

  # Provider-specific configuration so you can fine-tune various
  # backing providers for Vagrant. These expose provider-specific options.
  # Example for VirtualBox:
  config.vm.provider "virtualbox" do |vb|
    # Display the VirtualBox GUI when booting the machine
    # vb.gui = true
    vb.name = "Vagrant_Cubium_Debian_Jessie64"
    #  Customize the amount of memory on the VM:
    #  vb.memory = "1024"
  end

  # Create a public network, which generally matched to bridged network.
  # Bridged networks make the machine appear as another physical device on
  # your network.
  # config.vm.network "public_network"

  config.vm.synced_folder ".", "/Cubium", type: "virtualbox"

  # Set post-vagrant-up message
	config.vm.post_up_message = "

                                      +?II.   
               .?????????=          ??II~.                                      
                ,??IIII?????.      ??III,                                      
                   .:IIIII???I    ??IIII.                                       
                      :~=IIII?II =?III+                                         
                         +III7?II??I?..  .=??????????~                          
             .+III???????????IIIII?II?????????????????IIII                     
          .7IIIIII?????????????IIIII????IIIIIIIIII777777IIII:                   
        7777IIIII???~.. ..~??III?I??I?II,IIIIIIII ~II77 .?7777=                 
      777777IIII.            II??I?II?,~+=                                      
    .?77777II+                +II~???.                                          
   :?777777I                                                                    
  +?I777777      ZOOO7OO$                         OOOOOOO, 78OOOOOOOO8   ,8OOO8 
 .??777777      8O8   OOO                      .8OO+  ,OO8OO8I ZOO: OO? 8OO~OO.
 ??I777777     8O8    8O8 .~.,.       ,..,    OOOI    I .O8 . OOO: 8O8,8OO 8O$  
:III77777,    OOO    .OO7.OOZOO=?OZO=8OO8OO   OOOOO888:  ?   8OOOOOO ?OOOOOOO   
IIII77777     8O8   ,OO8 88 ~O8ZO+O=$O8 OO.OZ =8888O8OO=     OOZ    8O8   OO,   
IIII77777,    8OZ  7OOZ $O+,O8 8O? OOO. O888.      :OO8     OO8    8O8   8O8    
II77777777    +OOOOOO,  OOOOI  .8OO~88  OO$ O8OI?O8OO7      8O7   8O8    OO:    
I777777777I     .+:    ,O8                   OOOOO8=         $Z    .            
7777777777II.          7OO                                                      
77777777.       J U S T     K I D D I N G     I T S     C U B I U M                  
,7777.
"
  # Share an additional folder to the guest VM. The first argument is
  # the path on the host to the actual folder. The second argument is
  # the path on the guest to mount the folder. And the optional third
  # argument is a set of non-required options.
  # config.vm.synced_folder "../data", "/vagrant_data"

  #
  # View the documentation for the provider you are using for more
  # information on available options.

  # Define a Vagrant Push strategy for pushing to Atlas. Other push strategies
  # such as FTP and Heroku are also available. See the documentation at
  # https://docs.vagrantup.com/v2/push/atlas.html for more information.
  # config.push.define "atlas" do |push|
  #   push.app = "YOUR_ATLAS_USERNAME/YOUR_APPLICATION_NAME"
  # end

  # Enable provisioning with a shell script. Additional provisioners such as
  # Puppet, Chef, Ansible, Salt, and Docker are also available. Please see the
  # documentation for more information about their specific syntax and use.

   config.vm.provision "shell", inline: <<-SHELL
     sudo -i
	#=== Installing dev dependencies ==
     apt-get -y update
     apt-get -y upgrade
     apt-get -y install build-essential
     apt-get -y install xfce4
     apt-get -y install iceweasel
     apt-get -y install cmake
     apt-get -y install doxygen
     apt-get -y install git
     apt-get -y install vim
     apt-get -y install lightdm
     apt-get -y install libboost-all-dev
     apt-get -y install valgrind

  # #=== Installing and configuring GTest/GMock ==
     apt-get -y install google-mock
		 cd /usr/src/gtest
		 cmake CMakeLists.txt
		 make
     cp *.a /usr/lib

     cd /usr/src/gmock
     cmake CMakeLists.txt
     make
     cp *.a /usr/lib

   SHELL

end
