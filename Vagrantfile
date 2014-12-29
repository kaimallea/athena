# -*- mode: ruby -*-
# vi: set ft=ruby :

# Vagrantfile API/syntax version. Don't touch unless you know what you're doing!
VAGRANTFILE_API_VERSION = "2"

Vagrant.configure(VAGRANTFILE_API_VERSION) do |config|

  config.vm.box = "ubuntu/trusty64"

$script = <<SCRIPT
sudo apt-get update -qq && apt-get install -y \
    git \
    build-essential \
    gcc-4.7 \
    gcc-4.7-multilib \
    g++-4.7 \
    g++-4.7-multilib \
    lib32z1 \
    lib32z1-dev \
    libc6-i386 \
    libc6-dev-i386
SCRIPT

  config.vm.provision "shell", inline: $script

end
