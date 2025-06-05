asn1c -pdu=all MAP-DialogueInformation.asn MAP-CommonDataTypes.asn MAP-ExtensionDataTypes.asn MAP-TS-Code.asn MAP-BS-Code.asn MAP-SS-Code.asn GSMMAP.asn MAP-CH-DataTypes.asn MAP-MS-DataTypes.asn MAP-SS-DataTypes.asn MAP-OM-DataTypes.asn MAP-ER-DataTypes.asn


./hexstrdump a11b02010102012d301380069156796654218101018206915679667457

the parameter to hexstrdump is from wireshark filter tcap GSM Mobile Applicaton copy as Hex Stream
./myconverter  -iber result.bin -pComponent

./myconverter -iber -H a11b02010102012d301380069156796654218101018206915679667457 -pComponent
./myconverter -iber -H a4028000

https://stackoverflow.com/questions/57533704/how-to-decode-map-invoke-messages-using-asn1c-generated-code


gdb 
r -iber -H a11b02010102012d301380069156796654218101018206915679667457 -pComponent -otext

Next rewrite print for invoke (SEQUENCE_print) to INVOKE_SEQUENCE_print


./myconverter -iber -H a11a02010002013b301204010f0405aa184c36028006917673232987 -pComponent
ac: 0
<Component>
    <invoke>
        <invokeID>0</invokeID>
        <opCode>
            <localValue>59</localValue>
        </opCode>
        <invokeparameter>
            30 12 04 01 0F 04 05 AA 18 4C 36 02 80 06 91 76 
            73 23 29 87
        </invokeparameter>
    </invoke>
</Component>


./myconverter -iber -H 301204010F0405AA184C36028006917673232987 -pUSSD-Arg
ac: 0
<USSD-Arg>
    <ussd-DataCodingScheme>0F</ussd-DataCodingScheme>
    <ussd-String>AA 18 4C 36 02</ussd-String>
    <msisdn>91 76 73 23 29 87</msisdn>
</USSD-Arg>


./myconverter -iber -H a11b02010102012d301380069156796654218101018206915679667457 -pComponent
ac: 0
<Component>
    <invoke>
        <invokeID>1</invokeID>
        <opCode>
            <localValue>45</localValue>
        </opCode>
        <invokeparameter>
            30 13 80 06 91 56 79 66 54 21 81 01 01 82 06 91 
            56 79 66 74 57
        </invokeparameter>
    </invoke>
</Component>


/myconverter -iber -H 301380069156796654218101018206915679667457 -pRoutingInfoForSM-Arg
ac: 0
<RoutingInfoForSM-Arg>
    <msisdn>91 56 79 66 54 21</msisdn>
    <sm-RP-PRI><true/></sm-RP-PRI>
    <serviceCentreAddress>91 56 79 66 74 57</serviceCentreAddress>
</RoutingInfoForSM-Arg>
