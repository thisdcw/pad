
#include "bz.h"

int8u keyVoiceFlag;

void keyVoice(void)
{
	static int8u voiceState;
	static int8u voiceCnt;
    if(keyVoiceFlag&0x80){
        clrbit(keyVoiceFlag,7);
        voiceState=0;
        voiceCnt=0;
    }
	if(keyVoiceFlag&0x01){              //Õý³£°´¼üÒô
		if(voiceState==0){
			BZPORT=BZON;                   //BZ on
			voiceState=1;
		}else{
			if(t10msFlag){
				t10msFlag=0;
				voiceCnt++;
				if(voiceCnt==10){
					voiceCnt=0;
					BZPORT=BZOFF;           //BZ off
					voiceState=0;
					keyVoiceFlag=0;
				}
			}
		}
	}else if(keyVoiceFlag&0x02){        //¾¯¸æÒô
		if(voiceState==0){
			BZPORT=BZON;                   //BZ on
			voiceState=1;
		}else if(voiceState==1){
			if(t10msFlag){
				t10msFlag=0;
				voiceCnt++;
				if(voiceCnt==5){
					voiceCnt=0;
					BZPORT=BZOFF;           //BZ off
					voiceState=2;
				}
			}
		}else if(voiceState==2){
			if(t10msFlag){
				t10msFlag=0;
				voiceCnt++;
				if(voiceCnt==10){
					voiceCnt=0;
					BZPORT=BZON;            //BZ on
					voiceState=3;
				}
			}
		}else if(voiceState==3){
			if(t10msFlag){
				t10msFlag=0;
				voiceCnt++;
				if(voiceCnt==5){
					voiceCnt=0;
					BZPORT=BZOFF;            //BZ off
					voiceState=0;
					keyVoiceFlag=0;
				}
			}
		}
	}else if(keyVoiceFlag&0x04){        //¶ÌÌáÊ¾Òô
        if(voiceState==0){
			BZPORT=BZON;                   //BZ on
			voiceState=1;
		}else{
			if(t10msFlag){
				t10msFlag=0;
				voiceCnt++;
				if(voiceCnt==5){
					voiceCnt=0;
					BZPORT=BZOFF;           //BZ off
					voiceState=0;
					keyVoiceFlag=0;
				}
			}
		}
    }else if(keyVoiceFlag&0x08){        //½áÊøÒô àÖàÖàÖ
    	switch(voiceState){
    	case 0:
			BZPORT=BZON;                   //BZ on
			voiceState=1;
		break;
		case 1:
            if(t10msFlag){
				t10msFlag=0;
				voiceCnt++;
				if(voiceCnt==25){
					voiceCnt=0;
					BZPORT=BZOFF;           //BZ off
					voiceState++;
					if(voiceState==6){
						voiceState=0;
						keyVoiceFlag=0;
					}
				}
			}
        break;
		case 3:
		case 5:
			if(t10msFlag){
				t10msFlag=0;
				voiceCnt++;
				if(voiceCnt==5){
					voiceCnt=0;
					BZPORT=BZOFF;           //BZ off
					voiceState++;
					if(voiceState==6){
						voiceState=0;
						keyVoiceFlag=0;
					}
				}
			}
		break;
		case 2:
		case 4:
			if(t10msFlag){
				t10msFlag=0;
				voiceCnt++;
				if(voiceCnt==10){
					voiceCnt=0;
					BZPORT=BZON;            //BZ on
					voiceState++;
				}
			}
		break;
		default:
		break;
		}
	}else if(keyVoiceFlag&0x10){        //³¤Ïì4Éù
		if(voiceState==0){
			BZPORT=BZON;                   //BZ on
			voiceState=1;
		}else{
			if(t10msFlag){
				t10msFlag=0;
				voiceCnt++;
				if(voiceCnt==50){
					voiceCnt=0;
					BZPORT ^= 1;        //·­×ª×´Ì¬
					voiceState++;
					if(voiceState==6){
						keyVoiceFlag=0;
						voiceState=0;
						BZPORT=BZOFF;       //BZ off
					}
				}
			}
		}
	}else if(keyVoiceFlag&0x20){        //³¤Ïì4Éù
		if(voiceState==0){
			BZPORT=BZON;                   //BZ on
			voiceState=1;
		}else{
			if(t10msFlag){
				t10msFlag=0;
				voiceCnt++;
				if(voiceCnt==35){
					voiceCnt=0;
					BZPORT ^= 1;        //·­×ª×´Ì¬
					voiceState++;
					if(voiceState==8){
						keyVoiceFlag=0;
						voiceState=0;
						BZPORT=BZOFF;       //BZ off
					}
				}
			}
		}
	}
}