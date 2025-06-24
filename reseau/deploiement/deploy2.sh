#!/bin/bash

ssh -p 3001 root@127.0.0.1 "cd /etc/systemd/system ; 
                            sudo systemctl daemon-reexec ; sudo systemctl daemon-reload ; sudo systemctl start client_server.service;"
                            
ssh -p 3002 root@127.0.0.1 "cd /etc/systemd/system ; 
                            sudo systemctl daemon-reexec ; sudo systemctl daemon-reload ; sudo systemctl start client_server.service;"

ssh -p 3003 root@127.0.0.1 "cd /etc/systemd/system ; 
                            sudo systemctl daemon-reexec ; sudo systemctl daemon-reload ; sudo systemctl start client_server.service;"

ssh -p 3004 root@127.0.0.1 "cd /etc/systemd/system ; 
                            sudo systemctl daemon-reexec ; sudo systemctl daemon-reload ; sudo systemctl start client_server.service;"

ssh -p 3005 root@127.0.0.1 "cd /etc/systemd/system ; 
                            sudo systemctl daemon-reexec ; sudo systemctl daemon-reload ; sudo systemctl start client_server.service;"
