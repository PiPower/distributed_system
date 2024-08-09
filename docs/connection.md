In order to connect to network it is needed to follow: \
1. Connect to unix socket to filepath /tmp/network_sim \
2. Clients wait for connection information from channel thread \

Data send to channel thread has following form [target_id(1 byte) | source(1 byte)  | payloadSize(4 bytes)  | payload] \
Channel takes data look up target_id and sends it to apriopriate target (or drops it if target is unavailable) \

