Need to do these two followings

asn1c -pdu=MAP-DialoguePDU MAP-DialogueInformation.asn MAP-CommonDataTypes.asn MAP-ExtensionDataTypes.asn MAP-TS-Code.asn MAP-BS-Code.asn MAP-SS-Code.asn GSMMAP.asn MAP-CH-DataTypes.asn MAP-MS-DataTypes.asn MAP-SS-DataTypes.asn MAP-OM-DataTypes.asn MAP-ER-DataTypes.asn
asn1c -pdu=auto MAP-DialogueInformation.asn MAP-CommonDataTypes.asn MAP-ExtensionDataTypes.asn MAP-TS-Code.asn MAP-BS-Code.asn MAP-SS-Code.asn GSMMAP.asn MAP-CH-DataTypes.asn MAP-MS-DataTypes.asn MAP-SS-DataTypes.asn MAP-OM-DataTypes.asn MAP-ER-DataTypes.asn
to get Makefile that has default pdutype, otherwise converter sample won't compile

make -f Makefile.am.sample 
or 
make



./hexstrdump a11b02010102012d301380069156796654218101018206915679667457

the parameter to hexstrdump is from wireshark filter tcap GSM Mobile Applicaton copy as Hex Stream
./converter_sample -iber result.bin -pComponentc

./hexstrdump  060101070c135604930012047683041041080c134d040600120476830410600d0ba1090607040000010013020e222820060704000001010101a015a01380099625080100000681f181069176830410601402078000
