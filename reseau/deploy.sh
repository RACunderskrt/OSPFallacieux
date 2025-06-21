#!/bin/bash

scp -P 2001 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 2001 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/client.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 2001 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/client_server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 2001 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/server root@127.0.0.1:/usr/local/bin
scp -P 2001 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/client root@127.0.0.1:/usr/local/bin
scp -P 2001 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/server.service root@127.0.0.1:/etc/systemd/system
scp -P 2001 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/client.service root@127.0.0.1:/etc/systemd/system

scp -P 2002 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 2002 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/client.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 2002 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/client_server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 2002 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/server root@127.0.0.1:/usr/local/bin
scp -P 2002 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/client root@127.0.0.1:/usr/local/bin
scp -P 2002 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/server.service root@127.0.0.1:/etc/systemd/system
scp -P 2002 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/client.service root@127.0.0.1:/etc/systemd/system

scp -P 2003 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 2003 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/client.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 2003 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/client_server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 2003 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/server root@127.0.0.1:/usr/local/bin
scp -P 2003 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/client root@127.0.0.1:/usr/local/bin
scp -P 2003 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/server.service root@127.0.0.1:/etc/systemd/system
scp -P 2003 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/client.service root@127.0.0.1:/etc/systemd/system

scp -P 3001 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3001 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/client.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3001 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/client_server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3001 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/server.service root@127.0.0.1:/etc/systemd/system
scp -P 3001 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/client.service root@127.0.0.1:/etc/systemd/system
ssh -p 3001 root@127.0.0.1 "cd /etc/systemd/system ; 
                            g++ /home/etudiant/helloOSPF/client.cpp -o client; cp client /usr/local/bin/client; 
                            g++ /home/etudiant/helloOSPF/server.cpp -o server; cp server /usr/local/bin/server;"

scp -P 3002 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3002 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/client.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3002 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/client_server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3002 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/server.service root@127.0.0.1:/etc/systemd/system
scp -P 3002 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/client.service root@127.0.0.1:/etc/systemd/system
ssh -p 3002 root@127.0.0.1 "cd /etc/systemd/system ; 
                            g++ /home/etudiant/helloOSPF/client.cpp -o client; cp client /usr/local/bin/client; 
                            g++ /home/etudiant/helloOSPF/server.cpp -o server; cp server /usr/local/bin/server;"

scp -P 3003 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3003 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/client.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3003 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/client_server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3003 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/server.service root@127.0.0.1:/etc/systemd/system
scp -P 3003 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/client.service root@127.0.0.1:/etc/systemd/system
ssh -p 3003 root@127.0.0.1 "cd /etc/systemd/system ; 
                            g++ /home/etudiant/helloOSPF/client.cpp -o client; cp client /usr/local/bin/client; 
                            g++ /home/etudiant/helloOSPF/server.cpp -o server; cp server /usr/local/bin/server;"

scp -P 3004 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3004 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/client.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3004 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/client_server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3004 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/server.service root@127.0.0.1:/etc/systemd/system
scp -P 3004 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/client.service root@127.0.0.1:/etc/systemd/system
ssh -p 3004 root@127.0.0.1 "cd /etc/systemd/system ; 
                            g++ /home/etudiant/helloOSPF/client.cpp -o client; cp client /usr/local/bin/client; 
                            g++ /home/etudiant/helloOSPF/server.cpp -o server; cp server /usr/local/bin/server;"

scp -P 3005 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3005 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/client.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3005 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/client_server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3005 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/server.service root@127.0.0.1:/etc/systemd/system
scp -P 3005 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/SocketCpp/client.service root@127.0.0.1:/etc/systemd/system
ssh -p 3005 root@127.0.0.1 "cd /etc/systemd/system ; 
                            g++ /home/etudiant/helloOSPF/client.cpp -o client; cp client /usr/local/bin/client; 
                            g++ /home/etudiant/helloOSPF/server.cpp -o server; cp server /usr/local/bin/server;"
