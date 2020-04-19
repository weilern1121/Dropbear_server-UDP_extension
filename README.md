# weilern1121-Dropbear_server-UDP_extension

## configurations
1) create keys
- ./dropbearkey -t ecdsa -f dropbear_ecdsa_host_key
- ./dropbearkey -t rsa -f dropbear_rsa_host_key
- ./dropbearkey -t dss -f dropbear_dss_host_key

2) move keys to etc
- sudo mv dropbear_rsa_host_key /etc/dropbear
- same command for 2 others

3) verify insertion:
- ./dropbear -E

## commands
- **server:** sudo ./dropbear -F -E -v -Up <1234>
- **python script:** python runUDP.py
- **ssh client:** ssh localhost -p <1234> <command>

## assignment description
```

```
