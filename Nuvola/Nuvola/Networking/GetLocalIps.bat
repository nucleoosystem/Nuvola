netsh interface ip delete arpcache
Logic\arp-scan-64.exe -t 192.168.1.18/24
arp -a > Networking\foundIps.txt