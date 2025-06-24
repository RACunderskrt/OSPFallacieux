#!/bin/bash

scp -P 2001 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 2001 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/client.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 2001 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/client_server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 2001 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/client_server.service root@127.0.0.1:/etc/systemd/system

scp -P 2002 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 2002 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/client.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 2002 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/client_server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 2002 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/client_server.service root@127.0.0.1:/etc/systemd/system

scp -P 2003 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 2003 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/client.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 2003 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/client_server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 2003 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/client_server.service root@127.0.0.1:/etc/systemd/system

scp -P 3001 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3001 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/client.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3001 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/client_server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3001 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/client_serverT.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3001 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/client_server.service root@127.0.0.1:/etc/systemd/system
ssh -p 3001 root@127.0.0.1 "cd /etc/systemd/system ; 
                            g++ /home/etudiant/helloOSPF/client_server.cpp -o client_server -pthread; cp client_server /usr/local/bin/client_server;"

scp -P 3002 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3002 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/client.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3002 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/client_server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3001 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/client_serverT.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3002 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/client_server.service root@127.0.0.1:/etc/systemd/system
ssh -p 3002 root@127.0.0.1 "cd /etc/systemd/system ; 
                            g++ /home/etudiant/helloOSPF/client_server.cpp -o client_server -pthread; cp client_server /usr/local/bin/client_server;"

scp -P 3003 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3003 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/client.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3003 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/client_server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3001 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/client_serverT.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3003 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/client_server.service root@127.0.0.1:/etc/systemd/system
ssh -p 3003 root@127.0.0.1 "cd /etc/systemd/system ; 
                            g++ /home/etudiant/helloOSPF/client_server.cpp -o client_server -pthread; cp client_server /usr/local/bin/client_server;"

scp -P 3004 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3004 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/client.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3004 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/client_server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3001 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/client_serverT.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3004 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/client_server.service root@127.0.0.1:/etc/systemd/system
ssh -p 3004 root@127.0.0.1 "cd /etc/systemd/system ; 
                            g++ /home/etudiant/helloOSPF/client_server.cpp -o client_server -pthread; cp client_server /usr/local/bin/client_server;"

scp -P 3005 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3005 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/client.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3005 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/client_server.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3001 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/client_serverT.cpp root@127.0.0.1:/home/etudiant/helloOSPF
scp -P 3005 /home/lg-mines/Mines-Ales/Semestre6/ResEtProt/OSPF/OSPFallacieux/reseau/client_server.service root@127.0.0.1:/etc/systemd/system
ssh -p 3005 root@127.0.0.1 "cd /etc/systemd/system ; 
                            g++ /home/etudiant/helloOSPF/client_server.cpp -o client_server -pthread; cp client_server /usr/local/bin/client_server;"
