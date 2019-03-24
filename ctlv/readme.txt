libctlv.so

About
-----
libctlv is library used to deserialize and serialize xlink tlv internal message.
It has one structure CXTlv, that can be used to achieve those tasks.  

Example
-------
#define MAX_BUFFER_SIZE 1024
int ret = 0;
int returnLength = 0;
unsigned char *stream = uIncomingData;
unsigned char NewTag [20];
unsigned char outData[40]
unsigned char newData [] = {0xFA, 0xCE, 0xB0, 0x0C, 0xBA, 0xBE, 0x04}
memset (newTag, 0, 20);
HtoB (newTag, "FEEDDADEADF154ABADF00D04", strlen("FEEDDADEADF154ABADF00D04)");
int streamlength = nIncomingDataLength;
CXTlv * p = NewDefaultCXTlv ();
p->Deserialize (p, stream, streamlength);
p->Serialize (p, data, MAX_BUFFER_SIZE);

/*validate tag is primitive, will add tags in tree if not exists*/
ret = p->SetDataFixedL (p, newTag, newData, 7); 
if (ret)
{
	/* validate tag existence, error if some of them not in tree*/
	ret = p->GetDataVarL (p, newTag, outData, 2, 4, &returnLength);
	if (ret) 
		fprintf ("freedom, no error")
	else
		fprintf ("error =%d", ret); 
}
else
{
	fprintf ("error =%d", ret);
} 
p->Cleanup(p); /*releases and frees tlv tree's object*/
free (p);

Notes
-----
Read ctlv.h for details about functions on this structure.
