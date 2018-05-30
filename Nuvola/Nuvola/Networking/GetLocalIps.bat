netsh interface ip delete arpcache
Logic\arp-scan-64.exe -t 192.168.1.89/22
Logic\arp-scan-64.exe -t 0.0.0.0/0
arp -a > Networking\foundIps.txt