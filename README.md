# ISA-NetFlow-generation


zadani:

Popis:
V rámci projektu implementujte NetFlow exportér, který ze zachycených síťových dat ve formátu pcap vytvoří záznamy NetFlow, které odešle na kolektor.

Použití:
Program musí podporovat následující syntax pro spuštění:

./flow [-f <file>] [-c <netflow_collector>[:<port>]] [-a <active_timer>] [-i <inactive_timer>] [-m <count>]

kde

-f <file> jméno analyzovaného souboru nebo STDIN,

-c <neflow_collector:port> IP adresa, nebo hostname NetFlow kolektoru. volitelně i UDP port (127.0.0.1:2055, pokud není specifikováno),

-a <active_timer> - interval v sekundách, po kterém se exportují aktivní záznamy na kolektor (60, pokud není specifikováno),

-i <seconds> - interval v sekundách, po jehož vypršení se exportují neaktivní záznamy na kolektor (10, pokud není specifikováno),

-m <count> - velikost flow-cache. Při dosažení max. velikosti dojde k exportu nejstaršího záznamu v cachi na kolektor (1024, pokud není specifikováno).

Všechny parametry jsou brány jako volitelné. Pokud některý z parametrů není uveden, použije se místo něj výchozí hodnota.

Příklad použití:

./flow -f input.pcap -c 192.168.0.1:2055

Implementace:

Implementujte v jazyku C/C++, za pomoci knihovny libpcap.

Upřesnění zadání:

Jako export stačí použít NetFlow v5. Pokud byste implementovali v9 se šablonami, bude to bonusově zohledněno v hodnocení projektu.
Pro vytváření flow stačí podpora protokolů TCP, UDP, ICMP.
Informace, které neznáte (srcAS, dstAS, next-hop, aj.) nastavte jako nulové.
Při exportování používejte původní časové značky zachycené komunikace.
Pro testování můžete využít nástroje ze sady nfdump (nfdump, nfcapd, nfreplay, ...).
Pro vytvoření vlastního testovacího souboru můžete použít program tcpdump.
Exportované NetFlow data by měla být čitelná nástrojem nfdump.
Odevzdání:

Odevzdaný projekt musí obsahovat:

soubor se zdrojovým kódem,
funkční Makefile pro překlad zdrojového souboru,
dokumentaci (soubor manual.pdf), která bude obsahovat uvedení do problematiky, návrhu aplikace, popis implementace, základní informace o programu, návod na použití. V dokumentaci se očekává následující: titulní strana, obsah, logické strukturování textu, přehled nastudovaných informací z literatury, popis zajímavějších pasáží implementace, použití vytvořených programů a literatura.
soubor flow.1 ve formátu a syntaxi manuálové stránky - viz https://liw.fi/manpages/
Vypracovaný projekt uložený v archívu .tar a se jménem xlogin00.tar odevzdejte elektronicky přes IS. Soubor nekomprimujte.

Spuštění, testování:

Všechny nezbytné úkony pro přípravu spuštění Vaší aplikace musí proběhnout zadáním příkazu make, ať už si vyberete kterýkoliv jazyk.

Doporučená literatura:

Studijní materiály k předmětu ISA - NetFlow
NetFlow na Wikipedia.org - https://en.wikipedia.org/wiki/NetFlow
Formát NetFlow datagramu - http://www.cisco.com/c/en/us/td/docs/net_mgmt/netflow_collection_engine/3-6/user/guide/format.html#wp1003394 [Table B-3 a Table B-4]
man nfdump
man nfcapd
man libpcap
