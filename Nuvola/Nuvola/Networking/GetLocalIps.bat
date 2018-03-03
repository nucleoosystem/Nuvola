netsh interface ip delete arpcache
Logic\arp-scan-64.exe -t 192.168.8.105/22
arp -a > Networking\foundIps.txt