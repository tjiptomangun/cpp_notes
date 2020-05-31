//https://www.developershome.com/sms/gsmAlphabet.asp
//https://www.ascii-code.com/
//gsm-ts_03.38
//https://en.wikipedia.org/wiki/GSM_03.38
//pduconv by matseng at nerdlabs dot org

#define NPC8 '?'
#define NPC7 63
unsigned int gsm_to_basic_charset[] = {
			  //Iso 8589 Latin 1
		0x40,  //0 at sign
		0xA3, //1 
		0x24,  //2 Dollar sign
		0xA5, //3 ¥ Yuan/Yen sign
		0xE8, //4 è e grave accent
		0xE9, //5 é acute accent
		0xF9, //6 ù grave accent
		0xEC, //7 ì grave accent
		0xF2, //8 ò grave accent
		0xC7, //9 Ç cedilla
		0x0A, //10 LF linefeed
		0xD8, //11 Ø stroke
		0xF8, //12 ø stroke
		0x0D, //13 CR carriage return
		0xC5, //14 Å ring
		0xE5, //15 å ring
		NPC8, //16 Δ Delta
		0x5F, //17 _ underscore
		NPC8, //18 Φ Phi
		NPC8, //19 Γ Gamma
		NPC8, //20 Λ Lambda
		NPC8, //21 Ω Omega
		NPC8, //22 Π Pi
		NPC8, //23 Ψ Psi
		NPC8, //24 Σ Sigma
		NPC8, //25 Θ Theta
		NPC8, //26 Ξ Xi
		0x1B, //27 Escape to extension table	
		0xC6, //28 Æ AE
		0xE6, //29 æ ae
		0xDF, //30 ß sz german (Wi heisz du) -> like beta
		0xC9, //31 É E accute accent
		0x20, //32 space
		0x21, //33 !
		0x22, //34 "
		0x23, //35 #
		0xA4, //36 Currency sign
		0x25, //37 %
		0x26, //38 &
		0x27, //39 '
		0x28, //40 (
		0x29, //41 )
		0x2A, //42 *
		0x2B, //43 +
		0x2C, //44 ,
		0x2D, //45 -
		0x2E, //46 .
		0x2F, //47 /
		0x30, //48 0
		0x31, //49 1
		0x32, //50 2
		0x33, //51 3
		0x34, //52 4
		0x35, //53 5
		0x36, //54 6
		0x37, //55 7
		0x38, //56 8
		0x39, //57 9
		0x3A, //58 :
		0x3B, //59 ;
		0x3C, //60 <
		0x3D, //61 =
		0x3E, //62 >
		0x3F, //63 ?
		0xA1, //64 ¡ inverted exlamation mark
		0x41, //65 A
		0x42, //66 B
		0x43, //67 C
		0x44, //68 D
		0x45, //69 E
		0x46, //70 F
		0x47, //71 G
		0x48, //72 H
		0x49, //73 I
		0x4A, //74 J
		0x4B, //77 K
		0x4C, //76 L
		0x4D, //77 M
		0x4E, //78 N
		0x4F, //79 O
		0x50, //80 P
		0x51, //81 Q
		0x52, //82 R
		0x53, //83 S
		0x54, //84 T
		0x55, //85 U
		0x56, //86 V
		0x57, //87 W
		0x58, //88 X
		0x59, //89 Y
		0x5A, //89 Z
		0xC4, //91 Ä A diaeresis
		0xD6, //92 Ö O diaeresis
		0xD1, //93 Ñ N tilde
		0xDC, //94 Ü U diaeresis
		0xA7, //95 § Sector sign 
		0xBF, //96 ¿ Inverted question mark
		0x61, //97 a 
		0x62, //98 b 
		0x63, //99 c 
		0x64, //100 d 
		0x65, //101 e
		0x66, //102 f
		0x67, //103 g
		0x68, //104 h
		0x69, //105 i
		0x6A, //106 j
		0x6B, //107 k
		0x6C, //108 l
		0x6D, //109 m
		0x6E, //110 n
		0x6F, //111 o
		0x70, //112 p
		0x71, //113 q
		0x72, //114 r
		0x73, //115 s
		0x74, //116 t
		0x75, //117 u
		0x76, //118 v
		0x77, //119 w
		0x78, //120 x
		0x79, //121 y
		0x7A, //122 z
		0xE4, //123 ä a diaeresis
		0xF6, //124 ö o diaeresis
		0xF1, //125 ñ n tilde
		0xFC, //126 ü n diaeresis
		0xE0, //127 à a grave accent 
};

unsigned int gsm_to_basic_char_ext[] = {
		NPC8, //0x00
		NPC8, //0x01
		NPC8, //0x02
		NPC8, //0x03
		NPC8, //0x04
		NPC8, //0x05
		NPC8, //0x06
		NPC8, //0x07
		NPC8, //0x08
		NPC8, //0x09
		0x0C, //0x0A (1B 0A) Form Feed
		NPC8, //0x0B
		NPC8, //0x0C
		NPC8, //0x0D
		NPC8, //0x0E
		NPC8, //0x0F
		NPC8, //0x10
		NPC8, //0x11
		NPC8, //0x12
		NPC8, //0x13
		0x5E, //0x14 (1B 14) ^
		NPC8, //0x15
		NPC8, //0x16
		NPC8, //0x17
		NPC8, //0x18
		NPC8, //0x19
		NPC8, //0x1A
		NPC8, //0x1B
		NPC8, //0x1C
		NPC8, //0x1D
		NPC8, //0x1E
		NPC8, //0x1F
		NPC8, //0x20
		NPC8, //0x21
		NPC8, //0x22
		NPC8, //0x23
		NPC8, //0x24
		NPC8, //0x25
		NPC8, //0x26
		NPC8, //0x27
		0x7B, //0x28 (1B 28) {
		0x7D, //0x29 (1B 29) }
		NPC8, //0x2A
		NPC8, //0x2B
		NPC8, //0x2C
		NPC8, //0x2D
		NPC8, //0x2E
		0x5C, //0x2F (1B 2F) \
		NPC8, //0x30
		NPC8, //0x31
		NPC8, //0x32
		NPC8, //0x33
		NPC8, //0x34
		NPC8, //0x35
		NPC8, //0x36
		NPC8, //0x37
		NPC8, //0x38
		NPC8, //0x39
		NPC8, //0x3A
		NPC8, //0x3B 
		0x5B, //0x3C (1B 3C) [
		0x7E, //0x3D (1B 3D) ~
		0x5D, //0x3E (1B 3E) ]
		NPC8, //0x3F
		0x7C, //0x40 (1B 40) |
		NPC8, //0x41
		NPC8, //0x42
		NPC8, //0x43
		NPC8, //0x44
		NPC8, //0x45
		NPC8, //0x46
		NPC8, //0x47
		NPC8, //0x48
		NPC8, //0x49
		NPC8, //0x4A
		NPC8, //0x4B
		NPC8, //0x4C
		NPC8, //0x4D
		NPC8, //0x4E
		NPC8, //0x4F
		NPC8, //0x50
		NPC8, //0x51
		NPC8, //0x52
		NPC8, //0x53
		NPC8, //0x54
		NPC8, //0x55
		NPC8, //0x56
		NPC8, //0x57
		NPC8, //0x58
		NPC8, //0x59
		NPC8, //0x5A
		NPC8, //0x5B
		NPC8, //0x5C
		NPC8, //0x5D
		NPC8, //0x5E
		NPC8, //0x5F
		NPC8, //0x60
		NPC8, //0x61
		NPC8, //0x62
		NPC8, //0x63
		NPC8, //0x64
		NPC8, //0x65
		NPC8, //0x66
		NPC8, //0x67
		NPC8, //0x68
		NPC8, //0x69
		NPC8, //0x6A
		NPC8, //0x6B
		NPC8, //0x6C
		NPC8, //0x6D
		NPC8, //0x6E
		NPC8, //0x6F
		NPC8, //0x70
		NPC8, //0x71
		NPC8, //0x72
		NPC8, //0x73
		NPC8, //0x74
		NPC8, //0x75
		NPC8, //0x76
		NPC8, //0x77
		NPC8, //0x78
		NPC8, //0x79
		NPC8, //0x7A
		NPC8, //0x7B
		NPC8, //0x7C
		NPC8, //0x7D
		NPC8, //0x7E
		NPC8, //0x7F
	
}

unsigned char ascii_to_gsm_charset[] {
                // basic and ext gsm charset
    NPC7,       // 1	start of heading [SOH]                  
    NPC7,       // 2	start of text [STX]                     
    NPC7,       // 3	end of text [ETX]                       
    NPC7,       // 4	end of transmission [EOT]               
    NPC7,       // 5	enquiry [ENQ]                           
    NPC7,       // 6	acknowledge [ACK]                       
    NPC7,       // 7	bell [BEL]                              
    NPC7,       // 8	backspace [BS]                          
    NPC7,       // 9	horizontal tab [HT]                     	
    0x0A,	// 10	LF
    NPC7,	// 11	VT Vertical Tab
    0x1B0A,	// 12	Form Feed
    0x0D,	// 13	CR
    NPC7,       // 14   shift out [SO]                          
    NPC7,       // 15   shift in [SI]                           
    NPC7,       // 16   data link escape [DLE]                  
    NPC7,       // 17   device control 1 [DC1]                  
    NPC7,       // 18   device control 2 [DC2]                  
    NPC7,       // 19   device control 3 [DC3]                  
    NPC7,       // 20   device control 4 [DC4]                  
    NPC7,       // 21   negative acknowledge [NAK]              
    NPC7,       // 22   synchronous idle [SYN]                  
    NPC7,       // 23   end of trans. block [ETB]               
    NPC7,       // 24   cancel [CAN]                            
    NPC7,       // 25   end of medium [EM]                      
    NPC7,       // 26   substitute [SUB]                        
    NPC7,       // 27   escape [ESC]                            
    NPC7,       // 28   file separator [FS]                     
    NPC7,       // 29   group separator [GS]                    
    NPC7,       // 30   record separator [RS]                   
    NPC7,       // 31   unit separator [US]                     
    32,         // 32      space                                   
    33,         // 33    ! exclamation mark                        
    34,         // 34    " double quotation mark                   
    35,         // 35    # number sign                             
    2,          // 36    $ dollar sign                             
    37,         // 37    % percent sign                            
    38,         // 38    & ampersand                               
    39,         // 39    ' apostrophe                              
    40,         // 40    ( left parenthesis                        
    41,         // 41    ) right parenthesis                       
    42,         // 42    * asterisk                                
    43,         // 43    + plus sign                               
    44,         // 44    , comma                                   
    45,         // 45    - hyphen                                  
    46,         // 46    . period                                  
    47,         // 47    / slash,                                  
    48,         // 48    0 digit 0                                 
    49,         // 49    1 digit 1                                 
    50,         // 50    2 digit 2                                 
    51,         // 51    3 digit 3                                 
    52,         // 52    4 digit 4                                 
    53,         // 53    5 digit 5                                 
    54,         // 54    6 digit 6                                 
    55,         // 55    7 digit 7                                 
    56,         // 56    8 digit 8                                 
    57,         // 57    9 digit 9                                 
    58,         // 58    : colon                                   
    59,         // 59    ; semicolon                               
    60,         // 60    < less-than sign                          
    61,         // 61    = equal sign                              
    62,         // 62    > greater-than sign                       
    63,         // 63    ? question mark                           
    0,          // 64    @ commercial at sign                      
    65,         // 65    A uppercase A                             
    66,         // 66    B uppercase B                             
    67,         // 67    C uppercase C                             
    68,         // 68    D uppercase D                             
    69,         // 69    E uppercase E                             
    70,         // 70    F uppercase F                             
    71,         // 71    G uppercase G                             
    72,         // 72    H uppercase H                             
    73,         // 73    I uppercase I                             
    74,         // 74    J uppercase J                             
    75,         // 75    K uppercase K                             
    76,         // 76    L uppercase L                             
    77,         // 77    M uppercase M                             
    78,         // 78    N uppercase N                             
    79,         // 79    O uppercase O                             
    80,         // 80    P uppercase P                             
    81,         // 81    Q uppercase Q                             
    82,         // 82    R uppercase R                             
    83,         // 83    S uppercase S                             
    84,         // 84    T uppercase T                             
    85,         // 85    U uppercase U                             
    86,         // 86    V uppercase V                             
    87,         // 87    W uppercase W                             
    88,         // 88    X uppercase X                             
    89,         // 89    Y uppercase Y                             
    90,         // 90    Z uppercase Z                             


    
}



int main(int argc, char **argv) {
}
