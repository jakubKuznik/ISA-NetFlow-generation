# ISA-NetFlow-generation


##NAME
       flow - generate netflows packets from pcap

##SYNOPSIS
       flow [-f FILENAME] [-c COLLECTOR[:PORT]] [-i TIMER] [-a TIMER] [-m COUNT] [-h]

##DESCRIPTION
       flow  Generate  NetFlow packets from pcap. Packets are sent to the collector which can be specified. It pro‐
       cesses TCP, UDP, and ICMP. Flow connection will be exported whenever the TCP fin or rst flag  occurs  or  if
       the inactive or active timer applies or if the flow cache is full the oldest flow will be exported.

       The  inactive timer is applied whenever the last packet in flow was not there for the inactive timer period.
       The active timer is applied whenever the last first packet in flow was not there for the  active  timer  pe‐
       riod.

##OPTIONS
       -f FILENAME
              Set the input file that has to be pcap. Default one is stdin.

       -c COLECTOR[:PORT]
              Set  the destination address of netflow collector, it could be both domain name or ip adress. Default
              is 127.0.0.1 and port 2055.

       -i TIMER
              Set the inactive timer in seconds. Default is 10s.

       -a TIMER
              Set the active timer in seconds. Default is 10s.

       -m TIMER
              Set the size of flow cache. That means how many flows can program handle at once.  Default is 1024.

       -h     Print help message.


