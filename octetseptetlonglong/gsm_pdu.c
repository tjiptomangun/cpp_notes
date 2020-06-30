//https://www.developershome.com/sms/gsmAlphabet.asp
//https://www.ascii-code.com/
//gsm-ts_03.38
//https://en.wikipedia.org/wiki/GSM_03.38
//pduconv by matseng at nerdlabs dot org
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define NPC8 '?'
#define NPC7 63
unsigned int gsm_to_basic_charset[] = {
			  //Iso 8589 Latin 1
		0x40, //0 (00) at sign
		0xA3, //1 (01) £ Pound sign
		0x24, //2 (02) Dollar sign
		0xA5, //3 (03) ¥ Yuan/Yen sign
		0xE8, //4 (04) è e grave accent
		0xE9, //5 (05) é acute accent
		0xF9, //6 (06) ù grave accent
		0xEC, //7 (07) ì grave accent
		0xF2, //8 (08) ò grave accent
		0xC7, //9 (09) Ç cedilla
		0x0A, //10 (0A) LF linefeed
		0xD8, //11 (0B) Ø stroke
		0xF8, //12 (0C) ø stroke
		0x0D, //13 (0D) CR carriage return
		0xC5, //14 (0E) Å ring
		0xE5, //15 (0F) å ring
		NPC8, //16 (10) Δ Delta
		0x5F, //17 (11) _ underscore
		NPC8, //18 (12) Φ Phi
		NPC8, //19 (13) Γ Gamma
		NPC8, //20 (14) Λ Lambda
		NPC8, //21 (15) Ω Omega
		NPC8, //22 (16) Π Pi
		NPC8, //23 (17) Ψ Psi
		NPC8, //24 (18) Σ Sigma
		NPC8, //25 (19) Θ Theta
		NPC8, //26 (1A) Ξ Xi
		0x1B, //27 (1B) Escape to extension table	
		0xC6, //28 (1C) Æ AE
		0xE6, //29 (1D) æ ae
		0xDF, //30 (1E) ß sz german (Wi heisz du) -> like beta
		0xC9, //31 (1F) É E accute accent
		0x20, //32 (20) space
		0x21, //33 (21) !
		0x22, //34 (22) "
		0x23, //35 (23) #
		0xA4, //36 (24) ¤ Currency sign
		0x25, //37 (25) %
		0x26, //38 (26) &
		0x27, //39 (27) '
		0x28, //40 (28) (
		0x29, //41 (29) )
		0x2A, //42 (2A) *
		0x2B, //43 (2B) +
		0x2C, //44 (2C) ,
		0x2D, //45 (2D) -
		0x2E, //46 (2E) .
		0x2F, //47 (2F) /
		0x30, //48 (30) 0
		0x31, //49 (31) 1
		0x32, //50 (32) 2
		0x33, //51 (33) 3
		0x34, //52 (34) 4
		0x35, //53 (35) 5
		0x36, //54 (36) 6
		0x37, //55 (37) 7
		0x38, //56 (38) 8
		0x39, //57 (39) 9
		0x3A, //58 (3A) :
		0x3B, //59 (3B) ;
		0x3C, //60 (3C) <
		0x3D, //61 (3D) =
		0x3E, //62 (3E) >
		0x3F, //63 (3F) ?
		0xA1, //64 (40) ¡ inverted exlamation mark
		0x41, //65 (41) A
		0x42, //66 (42) B
		0x43, //67 (43) C
		0x44, //68 (44) D
		0x45, //69 (45) E
		0x46, //70 (46) F
		0x47, //71 (47) G
		0x48, //72 (48) H
		0x49, //73 (49) I
		0x4A, //74 (4A) J
		0x4B, //77 (4B) K
		0x4C, //76 (4C) L
		0x4D, //77 (4D) M
		0x4E, //78 (4E) N
		0x4F, //79 (4F) O
		0x50, //80 (50) P
		0x51, //81 (51) Q
		0x52, //82 (52) R
		0x53, //83 (53) S
		0x54, //84 (54) T
		0x55, //85 (55) U
		0x56, //86 (56) V
		0x57, //87 (57) W
		0x58, //88 (58) X
		0x59, //89 (59) Y
		0x5A, //89 (5A) Z
		0xC4, //91 (5B) Ä A diaeresis
		0xD6, //92 (5C) Ö O diaeresis
		0xD1, //93 (5D) Ñ N tilde
		0xDC, //94 (5E) Ü U diaeresis
		0xA7, //95 (5F) § Sector sign 
		0xBF, //96 (60) ¿ Inverted question mark
		0x61, //97 (61) a 
		0x62, //98 (62) b 
		0x63, //99 (63) c 
		0x64, //100 (64) d 
		0x65, //101 (65) e
		0x66, //102 (66) f
		0x67, //103 (67) g
		0x68, //104 (68) h
		0x69, //105 (69) i
		0x6A, //106 (6A) j
		0x6B, //107 (6B) k
		0x6C, //108 (6C) l
		0x6D, //109 (6D) m
		0x6E, //110 (6E) n
		0x6F, //111 (6F) o
		0x70, //112 (70) p
		0x71, //113 (71) q
		0x72, //114 (72) r
		0x73, //115 (73) s
		0x74, //116 (74) t
		0x75, //117 (75) u
		0x76, //118 (76) v
		0x77, //119 (77) w
		0x78, //120 (78) x
		0x79, //121 (79) y
		0x7A, //122 (7A) z
		0xE4, //123 (7B) ä a diaeresis
		0xF6, //124 (7C) ö o diaeresis
		0xF1, //125 (7D) ñ n tilde
		0xFC, //126 (7E) ü n diaeresis
		0xE0, //127 (7F) à a grave accent 
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
		0x5C, /* 0x2F (1B 2F) \ */
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
		0x80, //0x65
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
};

unsigned int ascii_to_gsm_charset[] ={
                // basic and ext gsm charset
		NPC7,	// 0 (00) start of heading [SOH]                  
		NPC7,	// 1 (01) start of heading [SOH]                  
		NPC7,	// 2 (02) start of text [STX]                     
		NPC7,	// 3 (03) end of text [ETX]                       
		NPC7,	// 4 (04) end of transmission [EOT]               
		NPC7,	// 5 (05) enquiry [ENQ]                           
		NPC7,	// 6 (06) acknowledge [ACK]                       
		NPC7,	// 7 (07) bell [BEL]                              
		NPC7,	// 8 (08) backspace [BS]                          
		NPC7,	// 9 (09) horizontal tab [HT]                     	
		0x0A,	// 10 (0A) LF
		NPC7,	// 11 (0B) VT Vertical Tab
		0x1B0A,	// 12 (0C) Form Feed
		0x0D,	// 13 (0D) CR
		NPC7,	// 14 (0E) shift out [SO]
		NPC7,	// 15 (0F) hift in [SI]
		NPC7,	// 16 (10) data link escape [DLE]
		NPC7,	// 17 (11) device control 1 [DC1]
		NPC7,	// 18 (12) device control 2 [DC2]
		NPC7,	// 19 (13) device control 3 [DC3]
		NPC7,	// 20 (14) device control 4 [DC4]
		NPC7,	// 21 (15) negative acknowledge [NAK]
		NPC7,	// 22 (16) synchronous idle [SYN]
		NPC7,	// 23 (17) end of trans. block [ETB]
		NPC7,	// 24 (18) cancel [CAN]
		NPC7,	// 25 (19) end of medium [EM]
		NPC7,	// 26 (1A) substitute [SUB]
		NPC7,	// 27 (1B) escape [ESC]
		NPC7,	// 28 (1C) file separator [FS]
		NPC7,	// 29 (1D) group separator [GS]
		NPC7,	// 30 (1E) record separator [RS]
		NPC7,	// 31 (1F) unit separator [US]
		0x20,	// 32 (20) space
		0x21,	// 33 (21) ! exclamation mark
		0x22,	// 34 (22) " double quotation mark
		0x23,	// 35 (23) # number sign
		0x02,	// 36 (24) $ dollar sign
		0x25,	// 37 (25) % percent sign
		0x26,	// 38 (26) & ampersand
		0x27,	// 39 (27) ' apostrophe
		0x28,	// 40 (28) ( left parenthesis
		0x29,	// 41 (29) ) right parenthesis
		0x2A,	// 42 (2A) * asterisk
		0x2B,	// 43 (2B) + plus sign
		0x2C,	// 44 (2C) ,	comma
		0x2D,	// 45 (2D) - hyphen
		0x2E,	// 46 (2E) . period
		0x2F,	// 47 (2F) / slash,
		0x30,	// 48 (30) 0	digit 0
		0x31,	// 49 (31) 1	digit 1
		0x32,	// 50 (32) 2	digit 2
		0x33,	// 51 (33) 3	digit 3
		0x34,	// 52 (34) 4	digit 4
		0x35,	// 53 (35) 5	digit 5
		0x36,	// 54 (36) 6	digit 6
		0x37,	// 55 (37) 7	digit 7
		0x38,	// 56 (38) 8	digit 8
		0x39,	// 57 (39) 9	digit 9
		0x3A,	// 58 (3A) : colon
		0x3B,	// 59 (3B) ; semicolon
		0x3C,	// 60 (3C) < less-than sign
		0x3D,	// 61 (3D) = equal sign
		0x3E,	// 62 (3E) > greater-than sign
		0x3F,	// 63 (3F) ? question mark
		0,		// 64 (40) @ commercial at sign
		0x41,	// 65 (41) A uppercase A
		0x42,	// 66 (42) B uppercase B
		0x43,	// 67 (43) C uppercase C
		0x44,	// 68 (44) D uppercase D
		0x45,	// 69 (45) E uppercase E
		0x46,	// 70 (46) F uppercase F
		0x47,	// 71 (47) G uppercase G
		0x48,	// 72 (48) H uppercase H
		0x49,	// 73 (49) I uppercase I
		0x4A,	// 74 (4A) J uppercase J
		0x4B,	// 75 (4B) K uppercase K
		0x4C,	// 76 (4C) L uppercase L
		0x4D,	// 77 (4D) M uppercase M
		0x4E,	// 78 (4E) N uppercase N
		0x4F,	// 79 (4F) O uppercase O
		0x50,	// 80 (50) P uppercase P
		0x51,	// 81 (51) Q uppercase Q
		0x52,	// 82 (52) uppercase R
		0x53,	// 83 (53) uppercase S
		0x54,	// 84 (54) uppercase T
		0x55,	// 85 (55) uppercase U
		0x56,	// 86 (56) uppercase V
		0x57,	// 87 (57) uppercase W
		0x58,	// 88 (58) uppercase X
		0x59,	// 89 (59) uppercase Y
		0x5A,	// 90 (5A) Z uppercase Z
		0x1B3C,	// 91 (5B) [
		0x1B2F, /* 92 (5C) \ */
		0x1B3E,	// 93 (5D) ]
		0x1B14,	// 94 (5E) ^
		0x11,	// 95 (5F) _
		0x2C,	// 96 (60) ` grave
		0x61,	// 97 (61) a
		0x62,	// 98 (62) b
		0x63,	// 99 (63) c
		0x64,	// 100 (64) d
		0x65,	// 101 (65) e
		0x66,	// 102 (66) f
		0x67,	// 103 (67) g
		0x68,	// 104 (68) h
		0x69,	// 105 (69) i
		0x6A,	// 106 (6A) j
		0x6B,	// 107 (6B) k
		0x6C,	// 108 (6C) l
		0x6D,	// 109 (6D) m
		0x6E,	// 110 (6E) n
		0x6F,	// 111 (6F) o
		0x70,	// 112 (70) p
		0x71,	// 113 (71) q
		0x72,	// 114 (72) r
		0x73,	// 115 (73) s
		0x74,	// 116 (74) t
		0x75,	// 117 (75) u
		0x76,	// 118 (76) v
		0x77,	// 119 (77) w
		0x78,	// 120 (78) x
		0x79,	// 121 (79) y
		0x7A,	// 122 (7A) z
		0x1B28,	// 123 (7B) {
		0x1B40,	// 124 (7C) |
		0x1B29,	// 125 (7D) }
		0x1B3D,	// 126 (7E) ~
		NPC7,	// 127 (7F) DEL
		0x1B65,	// 128 (80) Euro
		NPC7,	// 129 (81) 
		NPC7,	// 130 (82) 
		NPC7,	// 131 (83) 
		NPC7,	// 132 (84) 
		NPC7,	// 133 (85) 
		NPC7,	// 134 (86) 
		NPC7,	// 135 (87) 
		0x1B14,	// 136 (88) circumflex, equals to caret
		NPC7,	// 137  (89) -- stick to developers home
		NPC7,	// 138  (8A) 
		NPC7,	// 139  (8B) 
		NPC7,	// 140 (8C) 
		NPC7,	// 141 (8D) 
		NPC7,	// 142 (8E) 
		NPC7,	// 143 (8F) 
		NPC7,	// 144 (90)
		NPC7,	// 145 (91)
		NPC7,	// 146 (92) 
		NPC7,	// 147 (93) 
		NPC7,	// 148 (94)
		NPC7,	// 149 (95)
		NPC7,	// 150 (96)
		NPC7,	// 151 (97)
		NPC7,	// 152 (98)
		NPC7,	// 153 (99)
		NPC7,	// 154 (9A)
		NPC7,	// 155 (9B)
		NPC7,	// 156 (9C)
		NPC7,	// 157 (9D)
		NPC7,	// 158 (9E)
		NPC7,	// 159 (9F)
		NPC7,	// 160 (A0)
		0x40,	// 161 (A1) ¡ inverted exlamation mark
		NPC7,	// 162 (A2)
		0x01,	// 163 (A3) £ Pound sign
		0x24,	// 164 (A4) ¤ Currency sign
		0x03,	// 165 (A5) ¥ Yuan/Yen sign
		NPC7,	// 166 (A6)
		0x5F,	// 167 (A7) § Sector sign 
		NPC7,	// 168 (A8)
		NPC7,	// 169 (A9)
		NPC7,	// 170 (AA)
		NPC7,	// 171 (AB)
		NPC7,	// 172 (AC)
		NPC7,	// 173 (AD)
		NPC7,	// 174 (AE)
		NPC7,	// 175 (AF)
		NPC7,	// 176 (B0)
		NPC7,	// 177 (B1)
		NPC7,	// 178 (B2)
		NPC7,	// 179 (B3)
		NPC7,	// 180 (B4)
		NPC7,	// 181 (B5)
		NPC7,	// 182 (B6)
		NPC7,	// 183 (B7)
		NPC7,	// 184 (B8)
		NPC7,	// 185 (B9)
		NPC7,	// 186 (BA)
		NPC7,	// 187 (BB)
		NPC7,	// 188 (BC)
		NPC7,	// 189 (BD)
		NPC7,	// 190 (BE)
		0x60,	// 191 (BF) ¿ Inverted question mark
		NPC7,	// 192 (C0)
		NPC7,	// 193 (C1)
		NPC7,	// 194 (C2)
		NPC7,	// 195 (C3)
		0x5B,	// 196 (C4) Ä A diaeresis
		0x0E,	// 197 (C5) Å ring
		0x1C,	// 198 (C6) Æ AE
		0x09,	// 199 (C7) Ç cedilla
		NPC7,	// 200 (C8) 
		0x1F,	// 201 (C9) É E accute accent
		NPC7,	// 202 (CA)
		NPC7,	// 203 (CB)
		NPC7,	// 204 (CC)
		NPC7,	// 205 (CD)
		NPC7,	// 206 (CE)
		NPC7,	// 207 (CF)
		NPC7,	// 208 (D0)
		0x5D,	// 209 (D1) Ñ N tilde
		NPC7,	// 210 (D2)
		NPC7,	// 211 (D3)
		NPC7,	// 212 (D4)
		NPC7,	// 213 (D5)
		0x5C,	// 214 (D6) Ö O diaeresis
		NPC7,	// 215 (D7)
		0x0B,	// 216 (D8) Ø stroke
		NPC7,	// 217 (D9)
		NPC7,	// 218 (DA)
		NPC7,	// 219 (DB)
		0x5E,	// 220 (DC) Ü U diaeresis
		NPC7,	// 221 (DD)
		NPC7,	// 222 (DE)
		0x1E,	// 223 (DF) ß sz german (Wi heisz du) -> like beta
		0x7F,	// 224 (E0) à a grave accent 
		NPC7,	// 225 (E1)
		NPC7,	// 226 (E2)
		NPC7,	// 227 (E3)
		0x7B,	// 228 (E4) ä a diaeresis
		0x0F,	// 229 (E5) å ring
		0x1D,	// 230 (E6) æ ae
		NPC7,	// 231 (E7)
		0x04,	// 232 (E8) è e grave accent
		0x05,	// 233 (E9) é acute accent
		NPC7,	// 234 (EA)
		NPC7,	// 235 (EB)
		0x07,	// 236 (EC) ì grave accent
		NPC7,	// 237 (ED)
		NPC7,	// 238 (EE)
		NPC7,	// 239 (EF)
		NPC7,	// 240 (F0)
		0x7D,	// 241 (F1) ñ n tilde
		0x08,	// 242 (F2) ò grave accent
		NPC7,	// 243 (F3)
		NPC7,	// 244 (F4)
		NPC7,	// 245 (F5)
		0x7C,	// 246 (F6) ö o diaeresis
		NPC7,	// 247 (F7)
		0x0C,	// 248 (F8) ø stroke
		0x06,	// 249 (F9) ù grave accent
		NPC7,	// 250 (FA)
		NPC7,	// 251 (FB)
		0x7E,	// 252 (FC) ü n diaeresis
		NPC7,	// 253 (FD) 
		NPC7,	// 254 (FE) 
		NPC7,	// 255 (FF) 
};

/**
 * NAME						: ascii_to_gsm7
 * DESCRIPTION				: translate ascii string to gsm 7 unpacked
 * INPUT
 *		input				: ascii string input
 *		ascii_to_gsm_map	: map used to translate
 *		
 */
int ascii_to_gsm7(unsigned char *input, unsigned char *output, int input_length, int output_buf_max) {
	int in_idx = 0;
	int out_idx = 0;
	unsigned int map_val;

	for(in_idx = 0; in_idx < input_length && out_idx < output_buf_max; in_idx++, out_idx++) {
		map_val = ascii_to_gsm_charset[input[in_idx]];
		if (map_val >> 8 == 0x1B) {
			output[out_idx++] = 0x1B;
		}
		output[out_idx] = map_val & 0x00FF;
	} 
	return out_idx;
}

int gsm7_to_ascii(unsigned char *input, unsigned char *output, int input_length, int output_buf_max) {
	int in_idx = 0;
	int out_idx = 0;
	unsigned int map_val;
	for(in_idx = 0; in_idx < input_length && out_idx < output_buf_max; in_idx++, out_idx++) {
		map_val = gsm_to_basic_charset[input[in_idx]];
		if (map_val == 0x1B) {
			in_idx ++;
			map_val = gsm_to_basic_char_ext[input[in_idx]];
		}
		output[out_idx] = map_val & 0x00FF; 
	}
	return out_idx;
}

void print_bytes(unsigned char * in, int inlength) {
	for(int i = 0; i < inlength; i++) {
		fprintf(stdout, "%02X ", in[i]);
	}
	fprintf(stdout, "\n");
}

int pack_gsm7_bit(unsigned char *pinput, unsigned char *output, int input_length, int output_buf_max) {
	int odx = 0;
	int idx;
	int cidx = 0;
	unsigned char buf;
	unsigned char *input = pinput;
	for (idx = 0; idx < input_length && odx < output_buf_max; idx++, odx++) {
		buf^=buf;
		fprintf(stdout, "cidx = %d  idx  = %d input[cidx] = %02X pinput[idx] = %02X input[cidx + 1] = %02X pinput[idx + 1] = %02X ==> ",
			cidx, idx, input[cidx], pinput[idx], input[cidx + 1], pinput[idx + 1]);
		if (cidx %7 ==  0) {
			output[odx] = input[cidx];
			buf = input[cidx + 1] & 0x01;
			buf <<=7;
			output[odx] |= buf;
			input[cidx + 1] >>= 1;
			cidx ++;
		}
		else if (cidx % 7 == 1) {
			output[odx] = input[cidx];
			buf = input[cidx + 1] & 0x03;
			buf <<=6;
			output[odx] |= buf;	
			input[cidx + 1] >>= 2; 
			cidx ++;
		}
		else if (cidx % 7 == 2) {
			output[odx] = input[cidx];
			buf = input[cidx + 1] & 0x07;
			buf <<=5;
			output[odx] |= buf;	
			input[cidx + 1] >>= 3; 
			cidx ++;
		}
		else if (cidx % 7 == 3) {
			output[odx] = input[cidx];
			buf = input[cidx + 1] & 0x0F;
			buf <<=4;
			output[odx] |= buf;	
			input[cidx + 1] >>= 4; 
			cidx ++;
		}
		else if (cidx % 7 == 4) {
			output[odx] = input[cidx];
			buf = input[cidx + 1] & 0x1F;
			buf <<=3;
			output[odx] |= buf;	
			input[cidx + 1] >>= 5; 
			cidx ++;
		}
		else if (cidx % 7 == 5) {
			output[odx] = input[cidx];
			buf = input[cidx + 1] & 0x3F;
			buf <<=2;
			output[odx] |= buf;	
			input[cidx + 1] >>= 6; 
			cidx ++;
		}
		else if (cidx % 7 == 6) {
			output[odx] = input[cidx];
			buf = input[cidx + 1] & 0x7F;
			buf <<=1;
			output[odx] |= buf;	
			input[cidx + 1] >>= 7;
			// input cidx+1(7) is empty now
			// just move input one byte
			// reset cidx to 0;
			input  = &pinput[idx+2];
			idx ++;
			cidx = 0;
			
		} 
		fprintf(stdout, "len  = %d ==> ", odx + 1);
		for (int oo = 0; oo < odx + 1; oo++) {
			fprintf(stdout, "%02X ", output[oo]);
		}
		fprintf(stdout, "\n");
	}
	return odx;
}

/**
 * NAME					: unpack_gsm7_bit
 * DESCRIPTION			: unpack pdu packed
 * INPUT
 *		pinput			: input byte stream
 *		input_length	: number of input in septet
 * OUTPUT
 *		output			: where to store output
 *		output_buf_max	: maximum number of bytes to store output
 * RETURNS
 *		<= 0			: error
 *		>0				: length of bytes copied
 *	
 */
int unpack_gsm7_bit(unsigned char *input, unsigned char *output, unsigned int input_length, int output_buf_max) {
	int m = input_length;
	int n = ((m + 1) * 7) >> 3;
	int padd = n % 7;
	while (m >= 0) {
		int o_m = m - 1;
		int i_n = n - 1;
		if (padd) {
			output[o_m] = input[i_n];
			output[o_m] <<= (padd - 1);
			output[o_m] &= 0x7F; 
			output[o_m] |= (input[i_n - 1] >> (8 - (padd - 1)));
		}
		else {
			output[o_m] = input[i_n];
			output[o_m] >>=1;
			output[o_m] &= 0x7F;
			m-= 1;
			o_m-= 1;
			unsigned char tmp;
			output[o_m] = input[i_n];
			output[o_m] <<= 6;
			output[o_m] &= 0x7F;
			tmp = input[i_n - 1] >> (8 - 6);
			output[o_m] |= tmp;
		}
		m--;
		n = ((m + 1) * 7) >> 3;
		padd = n % 7;
	}
	fprintf(stdout, "unpacked len  = %d ==> ", input_length);
	for (int oo = 0; oo < input_length; oo++) {
		fprintf(stdout, "%02X ", output[oo]);
	}
	fprintf(stdout, "\n");
	return input_length;
}

void test_pack_unpack_gsm_7(char *input_01) {
	unsigned char out_gsm7[300] = {0};
	unsigned char out_ascii[300] = {0};
	unsigned char out_gsm7packed[300] = {0};
	unsigned char out_gsm7unpacked[300] = {0};
	//char *input_01 = "Hello World!\n1234^{}£¥\xE0Z|";
	//char *input_01 = "ABCDEFGHIJK";
	int length_out = ascii_to_gsm7((unsigned char *)input_01, out_gsm7, strlen((char *)input_01), 300);
	int gsm7_length = length_out;
	fprintf(stdout, "input\n%s\n", input_01);
	fprintf(stdout, "hex\n");
	print_bytes((unsigned char *)input_01, strlen((char*)input_01));
	fprintf(stdout, "output\n");
	print_bytes(out_gsm7, length_out);
	fprintf(stdout, "reverse\n");
	length_out = gsm7_to_ascii(out_gsm7, out_ascii, length_out, 300);
	print_bytes(out_ascii, length_out);
	length_out = pack_gsm7_bit(out_gsm7, out_gsm7packed, gsm7_length, 300);
	length_out = unpack_gsm7_bit(out_gsm7packed, out_gsm7unpacked, gsm7_length, 300);
	
}


int main(int argc,	char **argv) {
	char *in_char = "ABCDEFGHIJKLMN";
	char buff[100] = {0};
	int i = 0;
	int len = strlen(in_char);

	for (i = 1; i <= len; i++) {
		memcpy(buff, in_char, i); 
		test_pack_unpack_gsm_7(buff);
		memset (buff, 0, sizeof(buff));
		fprintf(stdout, "====================================\n");
	}

	memset(buff, 0, sizeof(buff));
	char *in_char2 = "Hello World of Pdu !!!!";
	len = strlen(in_char2);
	for (i = 1; i <= len; i++) {
		memcpy(buff, in_char2, i); 
		test_pack_unpack_gsm_7(buff);
		memset (buff, 0, sizeof(buff));
		fprintf(stdout, "====================================\n");
	}
}
