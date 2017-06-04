# Build & Install Media Server
Build Dependencies 
Make sure that you have built /installed all of the following libraries before building Media Server 
- ibsndfile: Library for Reading and Writing Sound Files 
- libutil: UTIL Library 
- libamps: AMPS (Asynchronous Middleware for Protocol Servers) Library 
- libsipstack: SIP Stack Library
- libsdpparser: SDP Parsing Library
- libdnsresolver: DNS Resolver Library
- glib2: Glib Library
- libxml2: Library providing XML and HTML support
Get Source Code
mkdir imsmrf 
git clone https://github.com/inamkhosa/imsmrf.git 
Compile and Install
# cd imsmrf 
# ./autogen.sh --prefix=/usr –datarootdir=/etc 

For debugging with gdb, following command may be used:
# ./autogen.sh --prefix=/usr --datarootdir=/etc --enable-debug=yes 

For release compilation, following command may be used:
# ./autogen.sh --prefix=/usr --datarootdir=/etc --enable-debug=no --enable-opt=8
# make 
# make install

Note: If –prefix and –datarootdir are not given then HSS will install to default directories. Default directories for installation are /usr/local/bin for binary files , /usr/local/lib for libraries and /usr/local/hss for configuration files.

Note: if you use different directories for installation rather than default directories then you should add you binary file path to environment variable PATH and libraries file to environment variable LD_LIBRARY_PATH.
Configure Media Server
The configuration file config.xml is used to provide the configuration options for running the Media Server application. The default path for installation of this configuration file is /etc/ms-{version}. 
#vi /etc/ms-{version}/config.xml 
Change the IPs and Ports to reflect to your system IP and Port. 
Start Media Server
The name of application installed for executing the Media Server is 'ms'. In Linux the default path for its installation is /usr/local/bin. Use the following command for starting the Media Server:
#ms


# imsmrf is licensed under Creative Common License (https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode). 
You can look at https://creativecommons.org/licenses/by-nc-sa/4.0/ for non-commercial usage. 
