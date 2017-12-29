@echo off	
netsh interface ip delete arpcache
Logic\arp-scan-64.exe -t 192.168.239.1/24
Logic\arp-scan-64.exe -t 192.168.229.1/24
Logic\arp-scan-64.exe -t 192.168.1.15/24
arp -a > Logic\foundIps.txt